#define __TU__ "ipc_msg"

#include "log.hh"
#include "errcode.hh"
#include "common.hh"
#include "ipc_msg.hh"
#include "std_comp.hh"

#include <assert.h>

std::ostream &
tgf::operator<< (std::ostream &os, const tgf::IpcMsgType &self)
{
  switch (self)
    {
    case Ignore:
      os << "Ignore";
      break;
    case Error:
      os << "Error";
      break;
    case ErrorReadable:
      os << "ErrorReadable";
      break;
    case Success:
      os << "Success";
      break;
    case SuccessReadable:
      os << "SuccessReadable";
      break;
    case SuccessBinary:
      os << "SuccessBinary";
      break;
    case ShutdownIpcSrv:
      os << "ShutdownIpcSrv";
      break;
    case ReportStatus:
      os << "ReportStatus";
      break;
    case UpColl:
      os << "UpColl";
      break;
    case ShutdownColl:
      os << "ShutdownColl";
      break;
    default:
      os << "UNKNOWN";
      break;
    }
  return os;
}

std::ostream &
tgf::operator<< (std::ostream &os, const tgf::IpcMsg &self)
{
  os << "IpcMsg(";
  os << "type:" << self.__type;
  os << ",";
  os << "extargs_len:" << self.__extargs_len;
  os << ",";
  if (self.__extargs)
    {
      os << "extargs:" << as_hex_list (*self.__extargs);
      os << ",";
    }
  else
    {
      os << "extargs:" << "-";
      os << ",";
    }
  os << "data_len:" << self.__data_len_;
  os << ",";
  os << "data:" << as_hex_list (self.data ());
  os << ",";
  os << ")";
  return os;
}

tgf::IpcMsg::IpcMsg (char buffer[NBYTE_IPCMSG_ALL])
{
  // default ctor
  __type = IpcMsgType::Ignore;
  memset (__data, 0, NBYTE_IPCMSG_ALL);
  __data_len_ = 0;
  __is_parsed = false;

  int blk1len = NBYTE_IPCMSG_MAGIC;
  int blk2len = NBYTE_IPCMSG_TYPE;
  int blk3len = NBYTE_IPCMSG_EXTARGS;

  u32 magic = 0;
  u16 typ = 0;
  decltype (__extargs_len) extlen = 0;
  for (int i = 0; i < NBYTE_IPCMSG_ALL; i++)
    {
      if (i < blk1len)
	{
	  magic = (magic << 8) | scast<u8> (buffer[i]);
	}
      else
	{
	  if (magic == 0x746766)
	    {
	      __is_parsed = true;
	    }
	  else
	    {
	      tgf::logfe ( "ctor", "not tgf message", magic);
	      break;
	    }
	  assert (i >= blk1len);
	  if (i < blk1len + blk2len)
	    {
	      typ = (typ << 8) | scast<u8> (buffer[i]);
	    }
	  else if (i < blk1len + blk2len + blk3len)
	    {
	      extlen = (extlen << 8) | scast<u8> (buffer[i]);
	    }
	}
    }

  if (__is_parsed)
    {
      memcpy (__data, buffer, NBYTE_IPCMSG_ALL);
      __type = scast<IpcMsgType> (typ);
      __data_len_ = NBYTE_IPCMSG_ALL;
      __extargs_len = extlen;
      __extargs = nullopt;
    }
}

void
tgf::IpcMsg::serialize_into (char buf[NBYTE_IPCMSG_ALL]) noexcept
{
  memcpy (buf, __data, NBYTE_IPCMSG_ALL);
}

void
tgf::IpcMsg::set_extargs (const u8 *obuf, size_t obuflen) noexcept
{
  assert (obuf != nullptr);
  assert (obuflen == __extargs_len);
  // assert (obuflen <= (1 << (8 * sizeof (decltype (__extargs_len)) - 1)));
  __extargs = vector<u8> (obuf, obuf + obuflen);
}

// ------------------------------ AUX ------------------------------

tgf::IpcMsg
tgf::new_ipcmsg (const tgf::IpcMsgType typ, const u32 extlen,
		 const char args[tgf::NBYTE_IPCMSG_ARGS]) noexcept
/*
 * ipc message's current initialization impl:
 * magic-dependent, type-independent, block-length-dependent,
 */
{
  char buf[tgf::NBYTE_IPCMSG_ALL];
  char *bufp = buf;

  memset (buf, 0, NBYTE_IPCMSG_ALL);

  { // magic
    char from[] = {0x74, 0x67, 0x66};
    memcpy (bufp, from, std::size (from));
    bufp += std::size (from);
  }

  { // typ
    u16 typ_as_num = scast<u16> (typ);
    char b2 = scast<char> (typ_as_num);
    char b1 = scast<char> (typ_as_num >> 8);
    char from[] = {b1, b2};
    memcpy (bufp, from, sizeof (from));
    bufp += sizeof (from);
  }

  { // extlen
    u8 b4 = scast<u8> (extlen);
    u8 b3 = scast<u8> (extlen >> 8);
    u8 b2 = scast<u8> (extlen >> 16);
    u8 b1 = scast<u8> (extlen >> 24);
    u8 from[] = {b1, b2, b3, b4};
    memcpy (bufp, rcast<char *> (from), sizeof (from));
    bufp += sizeof (from);
  }

  memcpy (bufp, args, tgf::NBYTE_IPCMSG_ARGS);

  tgf::IpcMsg msg = tgf::IpcMsg (buf);

  tulogfd ("new_ipcmsg", as_hex_list (msg.data ()));

  if (!msg.well_formed ())
    {
      // reaching here means bugs
      tulogfe (tgf::EC::IPCMSG_MUSTVALIDIPCMSG, strerror (errno));
      tgf::exitMain (tgf::EC::IPCMSG_MUSTVALIDIPCMSG); // TODO: do not exit
    }

  return msg;
}

tgf::IpcMsg
tgf::new_ipcmsg (const tgf::IpcMsgType typ,
		 const char args[tgf::NBYTE_IPCMSG_ARGS]) noexcept
{
  return tgf::new_ipcmsg (typ, 0, args);
}

tgf::IpcMsg
tgf::new_ipcmsg_shutipcsrv () noexcept
{
  char args[tgf::NBYTE_IPCMSG_ARGS];
  memset (args, 0, tgf::NBYTE_IPCMSG_ARGS);
  return tgf::new_ipcmsg (tgf::IpcMsgType::ShutdownIpcSrv, args);
}

tgf::IpcMsg
tgf::new_ipcmsg_rstatus () noexcept
{
  char args[tgf::NBYTE_IPCMSG_ARGS];
  memset (args, 0, tgf::NBYTE_IPCMSG_ARGS);
  const char *s = "auth:?";
  memcpy (args, s, strlen (s));
  return tgf::new_ipcmsg (tgf::IpcMsgType::ReportStatus, args);
}


tgf::IpcMsg
tgf::new_ipcmsg_upcoll () noexcept
{
  char args[tgf::NBYTE_IPCMSG_ARGS];
  memset (args, 0, tgf::NBYTE_IPCMSG_ARGS);
  return tgf::new_ipcmsg (tgf::IpcMsgType::UpColl, args);
}

tgf::IpcMsg
tgf::new_ipcmsg_shutcoll () noexcept
{
  char args[tgf::NBYTE_IPCMSG_ARGS];
  memset (args, 0, tgf::NBYTE_IPCMSG_ARGS);
  return tgf::new_ipcmsg (tgf::IpcMsgType::ShutdownColl, args);
}


tgf::IpcMsg
tgf::new_ipcmsg_successr (const u8 *obuf, const size_t obuflen) noexcept
{
  char args[tgf::NBYTE_IPCMSG_ARGS];
  memset (args, 0, tgf::NBYTE_IPCMSG_ARGS);
  tgf::IpcMsg msg
    = tgf::new_ipcmsg (tgf::IpcMsgType::SuccessReadable, obuflen, args);
  // extargs not meant to be set here
  // msg.set_extargs (obuf, obuflen);
  return msg;
}

tgf::IpcMsg
tgf::new_ipcmsg_success () noexcept
{
  char args[tgf::NBYTE_IPCMSG_ARGS];
  memset (args, 0, tgf::NBYTE_IPCMSG_ARGS);
  return tgf::new_ipcmsg (tgf::IpcMsgType::Success, args);
}

tgf::IpcMsg
tgf::new_ipcmsg_error () noexcept
{
  char args[tgf::NBYTE_IPCMSG_ARGS];
  memset (args, 0, tgf::NBYTE_IPCMSG_ARGS);
  return tgf::new_ipcmsg (tgf::IpcMsgType::Error, args);
}
