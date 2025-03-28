#ifndef hh_user_data
#define hh_user_data

#include <optional>
#include <string>
#include <filesystem>
#include <stdio.h>
#include "std_comp.hh"
#include "locale_conf.hh"

namespace tgf {

class UserData
{
  FILE *lck_droot{nullptr};
  FILE *lck_filters{nullptr};
  path data_root_majv;
  static constexpr auto DIR_DATAROOT = ".tgfocus";
  static constexpr auto DIR_TDDATA = "tdlib";
  static constexpr auto FILE_APIID = "api_id";
  static constexpr auto FILE_APIHASH = "api_hash";
  static constexpr auto FILE_AUTH_HINT = "auth_hint";
  static constexpr auto FILE_FILTERS = "filters.toml";
  static constexpr auto FILE_FILTERS_TMP = "filters.toml.tmp";
  static constexpr auto FILE_TGFID = "tgfid";
  static constexpr auto FILE_PREF_LANG = "pref_lang";
  static constexpr auto FILE_LOCK_DROOT = "droot.lock";
  static constexpr auto FILE_LOCK_FILTERS = "filters.lock";
  static constexpr const char *FILE_LOG_PARENT = "parent.log";
  static constexpr const char *FILE_LOG_CHILD = "child.log";
  static constexpr const char *FILE_LOG_CHILD_COLL = "child_coll.log";

public:
  UserData () = delete;
  ~UserData ();
  UserData (std::optional<path> &&may_pred_home, bool reset = false);

  std::string name_droot ();

  path abspath_of (const char *subname) const;
  path path_droot () const;
  path path_tddata () const;
  path path_api_id () const;
  path path_api_hash () const;
  path path_auth_hint () const;
  path path_filters () const;
  path path_filters_tmp () const;
  path path_tgfid () const;
  path path_pref_lang () const;
  path path_log_parent () const;
  path path_log_child () const;
  path path_log_child_coll () const;

  bool prepare_filters_tmp () const;
  std::int32_t get_api_id () const;
  std::string get_api_hash () const;
  bool get_auth_hint () const;
  std::string get_filters () const;
  std::string get_filters_tmp () const;
  int64_t get_tgfid () const;
  bool is_tgfid_valid () const;
  tgf::Lang get_pref_lang () const;

  // setter

  void set_api_id (int32_t ipt) const;
  void set_api_hash (std::string &&in) const;
  // this flag will critically affect authorization process
  void set_auth_hint (bool flag);
  void set_filters (std::string &&in) const;
  void set_tgfid (int64_t in) const;
  void set_pref_lang (tgf::Lang) const;
  bool is_basic_tgfid () const;
  bool is_super_tgfid () const;
  void set_basic_tgfid () const;
  void set_super_tgfid () const;
};

// FIXME: placed in some other tu?
constexpr uintmax_t MAX_SZ_LARGE_FILE =
#ifdef NDEBUG
  (1024 * 500) // lets be tolerant at early v3
#else
  (1024 * 1024)
#endif
  ;

// FIXME: placed in some other tu?
inline bool
is_too_large (const path &p)
{
  if (exists (p) && is_regular_file (p))
    {
      uintmax_t fsz = file_size (p);
      return fsz > MAX_SZ_LARGE_FILE;
    }
  return false;
}

} // namespace tgf

#endif
