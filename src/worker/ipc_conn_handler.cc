#define __TU__ "ipc_conn_handler"

#include "ipc_conn_handler.hh"

#include "std_comp.hh"
#include "errcode.hh"
#include "common.hh"
#include "log.hh"
#include "ipc_msg.hh"
#include "coll_initer.hh"

#include <iterator>
#include <cstring>
#include <thread>
#include <chrono>
#include <sys/epoll.h>

namespace gstat = /* DoNotDeleteMe */ tgfstat;
namespace gstat_c = /* DoNotDeleteMe */ tgfstat::c;
namespace gstat_d = /* DoNotDeleteMe */ tgfstat::c::d;

tgf::IpcConnHandler::IpcConnHandler () { tulogfi ("ctor"); }

tgf::IpcConnHandler::IpcConnHandler (int fd, bool immd)
{
  tulogfi ("ctor2");
  connfd_ = fd;
  conn_handled = false;
  immd_ = immd;
}

tgf::IpcConnHandler::~IpcConnHandler ()
{
  tulogfi ("dtor");
  if (connfd_ >= 0 && conn_handled)
    {
      close (connfd_);
    }
}

void
rhandle_rstatus (int connfd_)
{
  ostringstream oss;

  bool is_auth = tgfstat::userdata.get_auth_hint ();
  bool is_coll_up = gstat_c::coll_initer_up.load (mo::relaxed);
  bool is_pause = gstat_d::pause_do_csm_mq.load (mo::relaxed);
  u32 n_msg_total = gstat_d::it_msg_total.load (mo::relaxed);
  u32 n_msg_coll = gstat_d::it_msg_coll.load (mo::relaxed);
  vector<tgf::TgMsg>::size_type mqsz = 0;
  {
    lock_guard<mutex> mq_guard (tgfstat::c::d::mq_lock);
    mqsz = tgfstat::c::d::mq.size ();
  }
  tgf::Lang lang = tgfstat::userdata.get_pref_lang ();
  oss << "     Authorized : " << (is_auth ? "YES" : "NO") << endl;
  oss << "      Collector : " << (is_coll_up ? "UP" : "DOWN") << endl;
  oss << "   Focus Paused : " << (is_pause ? "YES" : "NO") << endl;
  oss << "      Fwd/Total : " << (n_msg_coll) << "/" << (n_msg_total) << endl;
  oss << "        MQ Size : " << (mqsz) << endl;
  oss << "         Locale : " << tgf::lang_to_cstr (tgf::HOST_LANG) << endl;

  string rpl = oss.str ();

  tgf::IpcMsg m = tgf::new_ipcmsg_successr (rcast<const u8 *> (rpl.c_str ()),
					    rpl.length ());
  tulogfd (6661, m);
  {
    int nsend = send (connfd_, m.data ().data (), tgf::NBYTE_IPCMSG_ALL, 0);
    if (nsend == -1)
      {
	tulogfe (9991, tgf::EC::IPCCONNHAND_SYSCALL_SEND, strerror (errno));
      }
    tulogfd (6662, nsend);
  }
  {
    int nsend = send (connfd_, rpl.c_str (), m.extargs_len (), 0);
    if (nsend == -1)
      {
	tulogfe (9991, tgf::EC::IPCCONNHAND_SYSCALL_SEND2, strerror (errno));
      }
    tulogfd (6663, nsend);
  }
}

void
tgf::IpcConnHandler::operator() ()
{
  char buffer[NBYTE_IPCMSG_ALL];
  ssize_t nread_total = 0;

  int ep_fd = epoll_create1 (0);
  if (ep_fd == -1)
    {
      tulogfe (9992, tgf::EC::IPCCONNHAND_SYSCALL_EPCREAT, strerror (errno));
      return;
    }

  {
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = connfd_;
    if (epoll_ctl (ep_fd, EPOLL_CTL_ADD, connfd_, &ev) == -1)
      {
	tulogfe (9993, tgf::EC::IPCCONNHAND_SYSCALL_EPCTL, strerror (errno));
	return;
      }
  }

  constexpr int N_EVENT_PER_LOOP = 1;
  constexpr int N_MS_TIMEOUT = 100;
  bool giveup_probe = false;
  struct epoll_event rd_events[N_EVENT_PER_LOOP];

  while (nread_total != NBYTE_IPCMSG_ALL && !giveup_probe)
    {
      // note:timeout useless here
      int ret = epoll_wait (ep_fd, rd_events, N_EVENT_PER_LOOP, N_MS_TIMEOUT);
      if (ret == -1)
	{
	  tulogfe (9994, tgf::EC::IPCCONNHAND_SYSCALL_EPWAIT, strerror (errno));
	  return;
	}
      else if (ret == 0)
	{
	  // tulogfd ("epoll_wait timeout");
	  continue;
	}
      else if ((rd_events[0].events & EPOLLIN)
	       && rd_events[0].data.fd == connfd_)
	{
	  ssize_t nread = read (connfd_, buffer, NBYTE_IPCMSG_ALL);
	  if (nread == -1)
	    {
	      tulogfe (9995, tgf::EC::IPCCONNHAND_SYSCALL_READ,
		       strerror (errno));
	      return;
	    }
	  giveup_probe = (nread == 0); // EOF
	  nread_total += nread;
	  tulogfd (6665, nread, nread_total);
	}
    }

  tulogfd (6667, as_hex_list (
		   std::vector<uint8_t> (buffer, buffer + NBYTE_IPCMSG_ALL)));

  if (giveup_probe)
    {
      IpcMsg m = new_ipcmsg_error ();
      // const char resp[] = "Good (shutdown)\n";
      int nwrite = write (connfd_, m.__data, NBYTE_IPCMSG_ALL);
      tulogfd (6668, nwrite);
      // const char resp[] = "BAD COMMAND\n";
      // write (connfd_, resp, sizeof (resp));

      conn_handled = true;
    }
  else
    {
      tgf::IpcMsg msg = tgf::IpcMsg (buffer);
      tulogfd (6669, msg.typ (), as_hex_list (msg.data ()));
      if (immd_)
	{
	  // TODO: should be in seperate place
	  if (msg.typ () == IpcMsgType::Ignore)
	    {
	      IpcMsg m = new_ipcmsg_success ();
	      write (connfd_, m.__data, NBYTE_IPCMSG_ALL);
	    }

	  if (msg.typ () == IpcMsgType::ShutdownIpcSrv)
	    {
	      if (gstat_c::coll_initer_up.load (mo::relaxed))
		{
		  gstat_c::tryshutwk::coll_initer.store (true, mo::relaxed);
		  gstat_c::tryshutwk::coll_initer.notify_all ();
		}
	      else
		{
		  gstat_c::tryshutwk::coll_initer_succ.store (true,
							      mo::relaxed);
		  gstat_c::tryshutwk::coll_initer_succ.notify_all ();
		}

	      tulogfd (77883);
	      // !!!BLOCKING
	      gstat_c::tryshutwk::coll_initer_succ.wait (false, mo::relaxed);
	      tulogfd (77884);

	      IpcMsg m = new_ipcmsg_success ();
	      tulogfd (6661, m.data ().size (), as_hex_list (m.data ()));

	      write (connfd_, m.__data, NBYTE_IPCMSG_ALL);
	      // IpcSrc is essentially parent of all wk, hence must trigger
	      // after send
	      gstat_c::tryshutwk::ipcsrv_initer.store (true, mo::relaxed);
	      gstat_c::tryshutwk::ipcsrv_initer.notify_all ();
	    }

	  if (msg.typ () == IpcMsgType::ReportStatus)
	    {
	      rhandle_rstatus (connfd_);
	    }

	  if (msg.typ () == IpcMsgType::UpColl)
	    {
	      if (!gstat_c::coll_initer_up.load (mo::relaxed))
		{
		  gstat_c::tryshutwk::coll_producer.store (false, mo::relaxed);
		  gstat_c::tryshutwk::coll_producer.notify_all ();
		  gstat_c::tryshutwk::coll_producer_succ.store (false,
								mo::relaxed);
		  gstat_c::tryshutwk::coll_producer_succ.notify_all ();
		  gstat_c::tryshutwk::coll_consumer.store (false, mo::relaxed);
		  gstat_c::tryshutwk::coll_consumer.notify_all ();
		  gstat_c::tryshutwk::coll_consumer_succ.store (false,
								mo::relaxed);
		  gstat_c::tryshutwk::coll_consumer_succ.notify_all ();
		  gstat_c::tryshutwk::coll_switcher.store (false, mo::relaxed);
		  gstat_c::tryshutwk::coll_switcher.notify_all ();
		  gstat_c::tryshutwk::coll_switcher_succ.store (false,
								mo::relaxed);
		  gstat_c::tryshutwk::coll_switcher_succ.notify_all ();
		  gstat_c::tryshutwk::coll_initer.store (false, mo::relaxed);
		  gstat_c::tryshutwk::coll_initer.notify_all ();
		  gstat_c::tryshutwk::coll_initer_succ.store (false,
							      mo::relaxed);
		  gstat_c::tryshutwk::coll_initer_succ.notify_all ();

		  tgf::CollIniter worker = tgf::CollIniter ();
		  jthread t (worker);
		  tgf::ensure::is_valid_thread (
		    t, tgf::EC::TGFOCUS_SYSCALL_THREAD3);
		  t.detach ();
		}

	      IpcMsg m = new_ipcmsg_success ();
	      tulogfd (66678, m.data ().size (), as_hex_list (m.data ()));
	      send (connfd_, m.__data, NBYTE_IPCMSG_ALL, 0);
	    }

	  if (msg.typ () == IpcMsgType::ShutdownColl)
	    {
	      if (gstat_c::coll_initer_up.load (mo::relaxed))
		{
		  gstat_c::tryshutwk::coll_initer.store (true, mo::relaxed);
		  gstat_c::tryshutwk::coll_initer.notify_all ();
		  gstat_c::tryshutwk::coll_initer_succ.wait (false,
							     mo::relaxed);
		}
	      if (true)
		{
		  IpcMsg m = new_ipcmsg_success ();
		  tulogfd (66688, m.data ().size (), as_hex_list (m.data ()));
		  send (connfd_, m.__data, NBYTE_IPCMSG_ALL, 0);
		}
	    }
	}
      else
	{
	  gstat_c::q_ipcmsg.enqueue (std::move (msg));
	  IpcMsg m = new_ipcmsg_success ();
	  write (connfd_, m.__data, NBYTE_IPCMSG_ALL);
	}

      conn_handled = true;
    }

  gstat_c::tryshutwk::ipcconn_handler_succ.store (true, mo::relaxed);
  gstat_c::tryshutwk::ipcconn_handler_succ.notify_all ();
  tulogfi ("shutdown V");
}
