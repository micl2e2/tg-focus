#define __TU__ "ipc_client"

#include "ipc_client.hh"

#include "std_comp.hh"
#include "log.hh"
#include "common.hh"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <endian.h>

namespace impl {

int
creat_in_client (uint16_t port)
{
  InSockAddr saddr;
  int tmpfd = -1;

  tmpfd = socket (AF_INET, SOCK_STREAM, 0);
  if (tmpfd == -1)
    {
      int ec = tgf::EC::IPCCLIENT_SYSCALL_SOCKET;
      tulogfe (9991, ec, strerror (errno));
      return -10 - ec;
    }

  init_sockaddr: {
    memset (&saddr, 0, sizeof (InSockAddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htobe16 (port);
    inet_pton (AF_INET, "127.0.0.1", &saddr.sin_addr);
  }

  // // Set up the server address structure
  // memset (&saddr, 0, sizeof (saddr));
  // saddr.sun_family = AF_INET;
  // strncpy (saddr.sun_path, unfpath, sizeof (saddr.sun_path) - 1);

  // Connect to the server
  if (connect (tmpfd, rcast<SockAddrPtr> (&saddr), sizeof (saddr)) == -1)
    {
      int ec = tgf::EC::IPCCLIENT_SYSCALL_CONNECT;
      tulogfe (9992, ec, strerror (errno));
      return -10 - ec;
    }

  return tmpfd;
}

int
creat_un_client (const char *unfpath)
{
  UniSockAddr saddr;
  int tmpfd = -1;

  tmpfd = socket (AF_UNIX, SOCK_STREAM, 0);
  if (tmpfd == -1)
    {
      int ec = tgf::EC::IPCCLIENT_SYSCALL_SOCKET2;
      tulogfe (ec, strerror (errno));
      return -10 - ec;
      // tgf::exitMain (tgf::EC::IPCCLIENT_SYSCALL_SOCKET2);
    }

  // Set up the server address structure
  memset (&saddr, 0, sizeof (saddr));
  saddr.sun_family = AF_UNIX;
  strncpy (saddr.sun_path, unfpath, sizeof (saddr.sun_path) - 1);

  // Connect to the server
  if (connect (tmpfd, rcast<SockAddrPtr> (&saddr), sizeof (saddr)) == -1)
    {
      int ec = tgf::EC::IPCCLIENT_SYSCALL_CONNECT2;
      tulogfe (9993, ec, strerror (errno));
      return -10 - ec;
      // tgf::exitMain (tgf::EC::IPCCLIENT_SYSCALL_CONNECT2);
    }

  return tmpfd;
}

} // namespace impl

tgf::IpcClient::IpcClient ()
{
  tulogfi ("ctor");

  __csock = -1;
  __ec = tgf::EC::NOERR;

  // __csock = impl::creat_un_client (SOCKET_PATH_IMMD);
  int sk = impl::creat_in_client (29799);

  tulogfd (66681, sk);

  if (sk < -10)
    {
      __ec = scast<tgf::ErrCode> (-(sk + 10));
      __csock = -1;
    }
  else
    {
      __ec = tgf::EC::NOERR;
      __csock = sk;
    }
}

tgf::IpcClient::~IpcClient ()
{
  tulogfi ("dtor");

  if (__csock >= 0)
    {
      close (__csock);
    }
}

std::optional<tgf::IpcMsg>
tgf::IpcClient::send_ipcmsg_sync (tgf::IpcMsg &&msg) noexcept
{
  char buf[NBYTE_IPCMSG_ALL];
  tulogfd (6661);

  if (!msg.well_formed ())
    {
      // reaching here means bugs
      tulogfe (9994, tgf::EC::IPCCLIENT_MUSTVALIDIPCMSG, strerror (errno));
      __ec = tgf::EC::IPCCLIENT_MUSTVALIDIPCMSG;
      // tgf::exitMain (tgf::EC::IPCCLIENT_MUSTVALIDIPCMSG);
      return nullopt;
    }

  // msg ready to send
  tulogfd (6662, as_hex_list (msg.data ()));

  msg.serialize_into (buf);

  // msg ready to send, after serialized
  tulogfd (6663, as_hex_list (vector<u8> (buf, buf + NBYTE_IPCMSG_ALL)));

  ssize_t nsend = send (__csock, buf, NBYTE_IPCMSG_ALL, 0);
  if (nsend == -1)
    {
      tulogfe (9995, tgf::EC::IPCCLIENT_SYSCALL_WRITE, strerror (errno));
      __ec = tgf::EC::IPCCLIENT_SYSCALL_WRITE;
      return nullopt;
    }
  tulogfd (6664, nsend);

  memset (buf, 0, NBYTE_IPCMSG_ALL);
  // ssize_t nread_total = 0;
  // while (nread_total < NBYTE_IPCMSG_ALL)
  //   {
  ssize_t nrecv = recv (__csock, buf, NBYTE_IPCMSG_ALL, 0);
  if (nrecv == -1)
    {
      tulogfe (9996, tgf::EC::IPCCLIENT_SYSCALL_READ, strerror (errno));
      __ec = tgf::EC::IPCCLIENT_SYSCALL_READ;
      return nullopt;
    }
  // nread_total += nread;
  // }

  tulogfd (6665);

  IpcMsg msg_rsp = IpcMsg (buf);
  if (msg_rsp.well_formed ())
    {
      if (msg_rsp.extargs_len () > 0)
	{
	  const size_t nbyte_extargs = msg_rsp.extargs_len ();
	  char *buf_extargs = scast<char *> (calloc (nbyte_extargs, 1));
	  ssize_t nread_total = 0;
	  while (nread_total < nbyte_extargs)
	    {
	      ssize_t nread = read (__csock, buf_extargs, nbyte_extargs);
	      if (nread == -1)
		{
		  tulogfe (9997, tgf::EC::IPCCLIENT_SYSCALL_READ,
			   strerror (errno));
		  __ec = tgf::EC::IPCCLIENT_SYSCALL_READ;
		  return nullopt;
		}
	      nread_total += nread;
	    }
	  msg_rsp.set_extargs (rcast<u8 *> (buf_extargs), nbyte_extargs);
	  // tulogfd ( "got extargs:", string (buf_extargs));
	  tulogfd (6666, msg_rsp);
	}

      return msg_rsp;
    }

  tulogfd (6667);

  return nullopt;
}
