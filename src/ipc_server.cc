#define __TU__ "ipc_server"

#include "ipc_server.hh"
#include "log.hh"
#include "common.hh"
#include "std_comp.hh"
#include <thread>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <endian.h>

namespace gstat = /* DoNotDeleteMe */ tgfstat;
namespace gstat_c = /* DoNotDeleteMe */ tgfstat::c;

namespace impl {

int
creat_in_server (u16 port)
{
  int tmpfd = socket (AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);

  if (tmpfd == -1)
    {
      int ec = tgf::EC::IPCSERVER_SYSCALL_SOCKET;
      tulogfe (ec, strerror (errno));
      return -10 - ec;
      // tgf::exitMain (tgf::EC::IPCSERVER_SYSCALL_SOCKET);
    }

  i32 optval = 1;
  i32 level = SOL_SOCKET;
  i32 optname = SO_REUSEADDR;
  if ((setsockopt (tmpfd, level, optname, &optval, sizeof (i32))) == -1)
    {
      int ec = tgf::EC::IPCSERVER_SYSCALL_SOCKOPT;
      tulogfe (ec, strerror (errno));
      return -10 - ec;
      // tgf::exitMain (tgf::EC::IPCSERVER_SYSCALL_SOCKOPT);
    }

  InSockAddr saddr;
  InSockAddrPtr saddrp = &saddr;

  init_sockaddr : {
    memset (saddrp, 0, sizeof (InSockAddr));
    saddrp->sin_family = AF_INET;
    saddrp->sin_port = htobe16 (port);
    inet_pton (AF_INET, "127.0.0.1", &saddrp->sin_addr);
  }

  if (bind (tmpfd, rcast<SockAddrPtr> (saddrp), sizeof (UniSockAddr)) == -1)
    {
      int ec = tgf::EC::IPCSERVER_SYSCALL_BIND;
      tulogfe (ec, strerror (errno));
      return -10 - ec;
      // tgf::exitMain (tgf::EC::IPCSERVER_SYSCALL_BIND);
    }

  if (listen (tmpfd, 16) == -1)
    {
      int ec = tgf::EC::IPCSERVER_SYSCALL_LISTEN;
      tulogfe (ec, strerror (errno));
      return -10 - ec;
      // tgf::exitMain (tgf::EC::IPCSERVER_SYSCALL_LISTEN);
    }

  return tmpfd;
}

int
create_unsock_server (const char *fpath_unsock)
{
  int tmpfd = socket (AF_UNIX, SOCK_STREAM | SOCK_NONBLOCK, 0);
  // int tmpfd = socket (AF_UNIX, SOCK_STREAM, 0);
  if (tmpfd == -1)
    {
      int ec = tgf::EC::IPCSERVER_SYSCALL_SOCKET2;
      tulogfe (ec, strerror (errno));
      return -10 - ec;
      // tgf::exitMain (tgf::EC::IPCSERVER_SYSCALL_SOCKET2);
    }

  UniSockAddr saddr;
  UniSockAddrPtr saddrp = &saddr;

  init_sockaddr : {
    memset (saddrp, 0, sizeof (UniSockAddr));
    saddrp->sun_family = AF_UNIX;
    strncpy (saddrp->sun_path, fpath_unsock, sizeof (saddrp->sun_path) - 1);
    // tgf::exitMain (tgf::ErrCode::InitSockAddr);
  }

  if (bind (tmpfd, ::rcast<SockAddrPtr> (saddrp), sizeof (UniSockAddr)) == -1)
    {
      int ec = tgf::EC::IPCSERVER_SYSCALL_BIND2;
      tulogfe (ec, strerror (errno));
      return -10 - ec;
      // tgf::exitMain (tgf::EC::IPCSERVER_SYSCALL_BIND2);
    }

  if (listen (tmpfd, 16) == -1)
    {
      int ec = tgf::EC::IPCSERVER_SYSCALL_LISTEN2;
      tulogfe (ec, strerror (errno));
      return -10 - ec;
      // tgf::exitMain (tgf::EC::IPCSERVER_SYSCALL_LISTEN2);
    }

  return tmpfd;
}

} // namespace impl

tgf::IpcServer::IpcServer ()
{
  tulogfi ("ctor");
  __ssock_lazy = -1;
  __ssock_immd = -1;
  __domain = -1;

  int sk1 = impl::creat_in_server (29798);
  int sk2 = impl::creat_in_server (29799);

  if (sk1 < -10)
    __ec = scast<tgf::ErrCode> (-(sk1 + 10));
  else if (sk2 < -10)
    __ec = scast<tgf::ErrCode> (-(sk2 + 10));
  else
    {
      __ssock_lazy = sk1;
      __ssock_immd = sk2;
      __domain = AF_INET;
      // __ssock_lazy = impl::create_unsock_server (SOCKET_PATH_LAZY);
      // __ssock_immd = impl::create_unsock_server (SOCKET_PATH_IMMD);
      // __domain = AF_UNIX;
    }
}

tgf::IpcServer::~IpcServer ()
{
  tulogfi ("dtor", __ssock_lazy, __ssock_immd);
  if (__ssock_lazy >= 0)
    {
      close (__ssock_lazy);
      if (__domain == AF_UNIX)
	unlink (SOCKET_PATH_LAZY);
    }
  if (__ssock_immd >= 0)
    {
      close (__ssock_immd);
      if (__domain == AF_UNIX)
	unlink (SOCKET_PATH_IMMD);
    }
}
