#ifndef hh_chatcmd
#define hh_chatcmd

#include "std_comp.hh"
#include "user_data.hh"

namespace tgf {

#define concat2(a, b) a " " b
#define concat3(a, b, c) a " " b " " c
#define concat4(a, b, c, d) a " " b " " c " " d
#define concat5(a, b, c, d, e) a " " b " " c " " d " " e
#define concat6(a, b, c, d, e, f) a " " b " " c " " d " " e " " f

#define CHATCMD_PAUSE "pause"

#define CHATCMD_RESUME "resume"

#define CHATCMD_FILTERS "filters"

#define CHATCMD_RAWFILTERS "rawfilters"

#define CHATCMD_EDITF "filter.edit"
#define CHATCMD_EDITF_ARG1 "<filter-id>"
#define CHATCMD_EDITF_ARG2 "<property>"
#define CHATCMD_EDITF_ARG3 "<action>"
#define CHATCMD_EDITF_ARG4 "<double-quoted-value>"
#define CHATCMD_EDITF_USAGE                                                    \
  concat6 ("Usage:\n", CHATCMD_EDITF, CHATCMD_EDITF_ARG1, CHATCMD_EDITF_ARG2,  \
	   CHATCMD_EDITF_ARG3, CHATCMD_EDITF_ARG4)

#define CHATCMD_INSF "filter.insert"
#define CHATCMD_INSF_ARG1 "<filter-id>"
#define CHATCMD_INSF_USAGE concat3 ("Usage:\n", CHATCMD_INSF, CHATCMD_INSF_ARG1)

#define CHATCMD_RMF "filter.remove"
#define CHATCMD_RMF_ARG1 "<filter-id>"
#define CHATCMD_RMF_USAGE concat3 ("Usage:\n", CHATCMD_RMF, CHATCMD_RMF_ARG1)

#define CHATCMD_RPLY_PREFIX "〘 TGFCMD 〙"
#define CHATCMD_RPLY_PREFIX_N_CP 10

typedef enum
{
  ChatCmdPause,
  ChatCmdResume,
  ChatCmdFilters,
  ChatCmdRawFilters,
  ChatCmdEditFilter,
  ChatCmdInsertFilter,
  ChatCmdRemoveFilter,
} ChatCmdType;

class ChatCmdHandler
{
  optional<string> __succ_data;
  optional<string> __did_what;
  string __aux_msg;

public:
  ChatCmdHandler () = delete;
  ChatCmdHandler (ChatCmdType typ, string ipt, UserData &p_userdata);
  inline bool done () const noexcept { return __succ_data.has_value (); }
  inline optional<string> succ_data () const noexcept { return __succ_data; }
  inline optional<string> did_what () const noexcept { return __did_what; }
  inline string aux_msg () const noexcept { return __aux_msg; }
};

} // namespace tgf

#endif
