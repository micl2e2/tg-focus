#ifndef _TG_FOCUS_DATA_H
#define _TG_FOCUS_DATA_H 1

#include <optional>
#include <string>
#include <filesystem>
#include <stdio.h>
#include "tgf_locale.hh"

class TgFocusData
{
public:
  TgFocusData () = delete;

  ~TgFocusData ();

  TgFocusData (std::optional<std::filesystem::path> &&may_pred_home,
	       bool reset = false);

  std::string name_droot ();

  std::filesystem::path abspath_of (const char *subname) const;

  std::filesystem::path path_droot () const;

  std::filesystem::path path_tddata () const;

  std::filesystem::path path_api_id () const;

  std::filesystem::path path_api_hash () const;

  std::filesystem::path path_auth_hint () const;

  std::filesystem::path path_filters () const;

  std::filesystem::path path_filters_tmp () const;

  std::filesystem::path path_tgfid () const;

  std::filesystem::path path_pref_lang () const;

  bool prepare_filters_tmp () const;

  std::string get_api_id () const;

  std::int32_t get_api_id_as_int32 () const;

  std::string get_api_hash () const;

  bool get_auth_hint () const;

  std::string get_filters () const;

  std::string get_filters_tmp () const;

  int64_t get_tgfid () const;

  bool is_tgfid_valid () const;

  tgf::Lang get_pref_lang () const;

  // setter

  void set_api_id (std::string &&in) const;

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

private:
  FILE *lck_droot{nullptr};
  FILE *lck_filters{nullptr};
  std::filesystem::path data_root;
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
};

#endif
