#define __TU__ "conn_listener"

#include "conn_listener.hh"

#include <sys/epoll.h>

#include "std_comp.hh"
#include "errcode.hh"
#include "log.hh"
#include "common.hh"
#include "ipc_msg.hh"
#include "ipc_conn_handler.hh"

namespace gstat = /* DoNotDeleteMe */ tgfstat;
namespace gstat_c = /* DoNotDeleteMe */ tgfstat::c;

tgf::ConnListener::ConnListener ()
{
  tulogfi ("ctor");
  // should cleanup fd in thread code
}

tgf::ConnListener::~ConnListener ()
{
  tulogfi ("dtor");
  // should cleanup fd in thread code
}

void
handle_lazy (int sockFD)
{
  tulogfd (6661);
  UniSockAddr caddr;
  UniSockAddrPtr caddrPtr = &caddr;
  socklen_t addrLen = sizeof (UniSockAddr);
  // int accFD = accept (sockFD, rcast<SockAddrPtr> (&caddr), &addrLen);
  // Not Necessarily Nonblock For accept Currently:
  int accFD = accept4 (sockFD, ::rcast<SockAddrPtr> (&caddr), &addrLen, SOCK_NONBLOCK);
  if (accFD == -1)
    {
      if (errno == EAGAIN || errno == EWOULDBLOCK)
	{
	  // tulogfe ("accept", "conn lost or has been handled");
	}
      else
	{
	  tulogfe (9992, tgf::EC::CONNLISTENER_SYSCALL_ACCEPT, strerror (errno));
	  // tgf::exitMain (tgf::EC::CONNLISTENER_SYSCALL_ACCEPT);
	}
    }
  {
    tulogfd (6662);
    tgf::IpcConnHandler handler = tgf::IpcConnHandler (accFD);
    std::jthread t (handler);
    tgf::ensure::is_valid_thread (t, tgf::EC::CONNLISTENER_SYSCALL_THREAD);
    t.detach ();
  }
}

void
handle_immd (int sockFD)
{
  tulogfd (6663);
  UniSockAddr caddr;
  UniSockAddrPtr caddrPtr = &caddr;
  socklen_t addrLen = sizeof (UniSockAddr);
  int accFD = accept (sockFD, rcast<SockAddrPtr> (&caddr), &addrLen);
  if (accFD == -1)
    {
      tulogfe (9991, strerror (errno));
      if (errno == EAGAIN || errno == EWOULDBLOCK)
	{
	  // tulogfe ("accept", "conn lost or has been handled");
	}
      else
	{
	  tulogfe (9995, tgf::EC::CONNLISTENER_SYSCALL_ACCEPT, strerror (errno));
	  // tgf::exitMain (tgf::EC::CONNLISTENER_SYSCALL_ACCEPT);
	}
    }
  {
    tulogfd (6664);
    tgf::IpcConnHandler handler = tgf::IpcConnHandler (accFD, true);
    handler ();
  }
}

void
tgf::ConnListener::operator() ()
{
  if (sock_lazy_ < 0 || sock_immd_ < 0)
    {
      tulogfe (tgf::EC::CONNLISTENER_SOCKREADY);
      tgf::exitMain (tgf::EC::CONNLISTENER_SOCKREADY);
    }

  int ep_fd = epoll_create1 (0);
  if (ep_fd == -1)
    {
      tulogfe (tgf::EC::CONNLISTENER_SYSCALL_EPCREAT, strerror (errno));
      tgf::exitMain (tgf::EC::CONNLISTENER_SYSCALL_EPCREAT);
    }

  {
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = sock_lazy_;
    if (epoll_ctl (ep_fd, EPOLL_CTL_ADD, sock_lazy_, &ev) == -1)
      {
	tulogfe (tgf::EC::CONNLISTENER_SYSCALL_EPCTL, strerror (errno));
	tgf::exitMain (tgf::EC::CONNLISTENER_SYSCALL_EPCTL);
      }
  }

  {
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = sock_immd_;
    if (epoll_ctl (ep_fd, EPOLL_CTL_ADD, sock_immd_, &ev) == -1)
      {
	tulogfe (tgf::EC::CONNLISTENER_SYSCALL_EPCTL2, strerror (errno));
	tgf::exitMain (tgf::EC::CONNLISTENER_SYSCALL_EPCTL2);
      }
  }

  constexpr int N_EVENT_PER_LOOP = 1;
  constexpr int N_MS_TIMEOUT = 100;
  struct epoll_event rd_events[N_EVENT_PER_LOOP];

  while (!gstat_c::tryshutwk::conn_listener.load (mo::relaxed))
    {
      int ret = epoll_wait (ep_fd, rd_events, N_EVENT_PER_LOOP, N_MS_TIMEOUT);
      if (ret == -1)
	{
	  tulogfe (tgf::EC::CONNLISTENER_SYSCALL_EPWAIT, strerror (errno));
	  tgf::exitMain (tgf::EC::CONNLISTENER_SYSCALL_EPWAIT);
	}
      else if (ret == 0)
	{
	  // tulogfd("epoll_wait timeout"); // io-intense
	  continue;
	}
      else if (rd_events[0].events & EPOLLIN)
	{
	  if (rd_events[0].data.fd == sock_lazy_)
	    {
	      handle_lazy (sock_lazy_);
	    }
	  if (rd_events[0].data.fd == sock_immd_)
	    {
	      handle_immd (sock_immd_);
	    }
	}
    } // while

  gstat_c::tryshutwk::conn_listener_succ.store (true, mo::relaxed);
  gstat_c::tryshutwk::conn_listener_succ.notify_all ();
  tulogfi ("shutdown V");
}
