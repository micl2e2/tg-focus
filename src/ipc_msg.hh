#ifndef hh_ipc
#define hh_ipc

#include <ostream>
#include <stdint.h>
#include <string.h>
#include <vector>

#include "std_comp.hh"

namespace tgf {

enum IpcMsgType
{
  Ignore = 0x0101,
  Error = 0x0102,
  ErrorReadable = 0x0103,
  Success = 0x0104,
  SuccessReadable = 0x0105,
  SuccessBinary = 0x0106,
  ShutdownIpcSrv = 0x0107,
  ReportStatus = 0x0108,
  UpColl = 0x0109,
  ShutdownColl = 0x010a,
};

std::ostream &
operator<< (std::ostream &os, const IpcMsgType &self);

// ------------------------------- - -------------------------------

constexpr int NBYTE_IPCMSG_ALL = 64;
constexpr int NBYTE_IPCMSG_EXTARGS = 4;
constexpr int NBYTE_IPCMSG_TYPE = 2;
constexpr int NBYTE_IPCMSG_MAGIC = 3;
constexpr int NBYTE_IPCMSG_ARGS = NBYTE_IPCMSG_ALL - NBYTE_IPCMSG_MAGIC
				  - NBYTE_IPCMSG_TYPE - NBYTE_IPCMSG_EXTARGS;

// ------------------------------- - -------------------------------

class IpcMsg
{
  IpcMsgType type_;
  u8 data_[NBYTE_IPCMSG_ALL];
  size_t data_len_;
  u32 __extargs_len;		   // implying <= 4gb
  optional<vector<u8> > __extargs; // NOTE: will never be transferred
  bool is_parsed_;

public:
  IpcMsg (char buffer[NBYTE_IPCMSG_ALL]);
  ~IpcMsg () {}
  inline bool well_formed () { return is_parsed_; };
  inline IpcMsgType typ () noexcept { return type_; }
  inline vector<u8> args () noexcept
  {
    int blk1len = NBYTE_IPCMSG_MAGIC;
    int blk2len = NBYTE_IPCMSG_TYPE;
    int blk3len = NBYTE_IPCMSG_EXTARGS;

    return vector<u8> (data_ + (blk1len + blk2len + blk3len),
		       data_ + data_len_);
  }
  inline size_t extargs_len () noexcept
  {
    return scast<size_t> (__extargs_len);
  }
  inline optional<vector<u8> > extargs () noexcept { return __extargs; }
  inline std::vector<uint8_t> data () const noexcept
  {
    return std::vector<uint8_t> (data_, data_ + data_len_);
  }
  void serialize_into (char buf[NBYTE_IPCMSG_ALL]) noexcept;
  void set_extargs (const u8 *obuf, size_t obuflen) noexcept;

  friend std::ostream &operator<< (std::ostream &os, const tgf::IpcMsg &self);
  friend class IpcConnHandler;
  friend IpcMsg new_ipcmsg (const IpcMsgType typ, const u32 extlen,
			    const char args[tgf::NBYTE_IPCMSG_ARGS]) noexcept;
};

std::ostream &
operator<< (std::ostream &os, const tgf::IpcMsg &self);

IpcMsg
new_ipcmsg (const IpcMsgType typ, const u32 extlen,
	    const char args[tgf::NBYTE_IPCMSG_ARGS]) noexcept;

IpcMsg
new_ipcmsg (const IpcMsgType typ,
	    const char args[tgf::NBYTE_IPCMSG_ARGS]) noexcept;

// shutdown ipc server
IpcMsg
new_ipcmsg_shutipcsrv () noexcept;

IpcMsg
new_ipcmsg_rstatus () noexcept;

IpcMsg
new_ipcmsg_upcoll () noexcept;

// shutdown collector
IpcMsg
new_ipcmsg_shutcoll () noexcept;

IpcMsg
new_ipcmsg_successr (const u8 *obuf, const size_t obuflen) noexcept;

IpcMsg
new_ipcmsg_success () noexcept;

IpcMsg
new_ipcmsg_error () noexcept;

} // namespace tgf

#endif
