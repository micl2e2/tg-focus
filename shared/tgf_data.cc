#include <chrono>
#include <thread>
#include <limits.h>
#include <stdio.h>
#include <assert.h>
#include <cstdlib>
#include <optional>
#include <string>
#include <filesystem>
#include <iostream>
#include <sys/file.h>

#include "common.hh"

#include "tgf_data.hh"

namespace fs = std::filesystem;

bool
lock_stream (FILE *strm)
{
  if (strm == nullptr)
    return false;

  int lck_fd = fileno (strm);
  if (lck_fd == -1)
    return false;

  if (flock (lck_fd, LOCK_EX) == -1)
    return false;

  return true;
}

bool
unlock_stream (FILE *strm)
{
  if (strm == nullptr)
    return false;

  int lck_fd = fileno (strm);
  if (lck_fd == -1)
    return false;

  if (flock (lck_fd, LOCK_UN) == -1)
    return false;

  return true;
}

TgFocusData::~TgFocusData ()
{
  if (this->lck_droot != nullptr)
    {
      // FIXME: unlock again needed?
      // int fd = fileno (this->lck_droot);
      // if (fd != -1)
      //   flock (fd, LOCK_UN);
      fclose (this->lck_droot);
    }
  if (this->lck_filters != nullptr)
    {
      fclose (this->lck_filters);
    }
}

void
init_tgfid_type_fs (std::filesystem::path droot);

TgFocusData::TgFocusData (std::optional<std::filesystem::path> &&may_pred_home,
			  bool reset)
{
  std::filesystem::path home_dir;

  if (auto pred = may_pred_home)
    {
      home_dir = *pred;
      std::filesystem::create_directory (home_dir);
    }
  else
    home_dir = std::string{std::getenv ("HOME")};

  // required for the following operations
  assert (std::filesystem::exists (home_dir));

  // data root
  auto droot_dir = home_dir / DIR_DATAROOT;

  if (reset)
    std::filesystem::remove_all (droot_dir);

  std::filesystem::create_directory (droot_dir);

  this->data_root = droot_dir;

  // droot lock
  auto p = droot_dir / FILE_LOCK_DROOT;
  {
    FILE *fstrm = fopen (p.c_str (), "w");
    if (fstrm == nullptr)
      return;
    this->lck_droot = fstrm;

    // LOCK
    if (!lock_stream (this->lck_droot))
      return;
  }

  // td data
  auto tddata_dir = droot_dir / DIR_TDDATA;
  std::filesystem::create_directory (tddata_dir);

  // api id
  auto apiid_file = droot_dir / FILE_APIID;
  {
    FILE *fstrm = fopen (apiid_file.c_str (), "a");
    if (fstrm)
      fclose (fstrm);
  }

  // api hash
  auto apihash_file = droot_dir / FILE_APIHASH;
  {
    FILE *fstrm = fopen (apihash_file.c_str (), "a");
    if (fstrm)
      fclose (fstrm);
  }

  // auth_hint
  auto auth_hint_file = droot_dir / FILE_AUTH_HINT;
  {
    FILE *fstrm = fopen (auth_hint_file.c_str (), "a");
    if (fstrm)
      fclose (fstrm);
  }

  // filters
  auto filters_file = droot_dir / FILE_FILTERS;
  {
    if (!std::filesystem::exists (filters_file))
      {
	FILE *fstrm = fopen (filters_file.c_str (), "a");
	const std::string default_ctn = R"([[focus-filter]]
title = ".*"
)";
	std::fwrite (default_ctn.c_str (), 1, default_ctn.length (), fstrm);
	if (fstrm)
	  fclose (fstrm);
      }
  }

  // filters lock
  auto filters_lock_file = droot_dir / FILE_LOCK_FILTERS;
  {
    FILE *fstrm = fopen (filters_lock_file.c_str (), "w");
    if (fstrm)
      this->lck_filters = fstrm;
    // fclose (fstrm);
  }

  // filters tmp
  auto filters_tmp_file = droot_dir / FILE_FILTERS_TMP;
  {
    FILE *fstrm = fopen (filters_tmp_file.c_str (), "w");
    if (fstrm)
      fclose (fstrm);
  }

  // tgfid
  namespace fs = std::filesystem;
  fs::path p_tgfid = droot_dir / FILE_TGFID;
  if (!fs::exists (p_tgfid))
    {
      FILE *fstrm = fopen (p_tgfid.c_str (), "w");

      if (fstrm)
	{
	  char b = 0xff;
	  for (int i = 0; i < 8; i++)
	    fwrite (&b, 1, 1, fstrm);
	  fclose (fstrm);
	}
    }

  // tgfid_type
  init_tgfid_type_fs (droot_dir);

  // pref_lang
  namespace fs = std::filesystem;
  fs::path p_pref_lang = droot_dir / FILE_PREF_LANG;
  if (!fs::exists (p_pref_lang))
    {
      FILE *f = fopen (p_pref_lang.c_str (), "w");

      if (f)
	{
	  char b = 0x00;
	  fwrite (&b, 1, 1, f);
	  fclose (f);
	}
    }

  // UNLOCK
  if (!unlock_stream (this->lck_droot))
    return;
}

std::string
TgFocusData::name_droot ()
{
  return this->DIR_DATAROOT;
}

std::filesystem::path
TgFocusData::abspath_of (const char *subname) const
{
  auto tmp = this->data_root;
  return tmp.append (subname);
}

std::filesystem::path
TgFocusData::path_droot () const
{
  return this->data_root;
}

std::filesystem::path
TgFocusData::path_tddata () const
{
  auto tmp = this->data_root;
  return tmp.append (DIR_TDDATA);
}

std::filesystem::path
TgFocusData::path_api_id () const
{
  auto tmp = this->data_root;
  return tmp.append (FILE_APIID);
}

std::filesystem::path
TgFocusData::path_api_hash () const
{
  auto tmp = this->data_root;
  return tmp.append (FILE_APIHASH);
}

std::filesystem::path
TgFocusData::path_auth_hint () const
{
  auto tmp = this->data_root;
  return tmp.append (FILE_AUTH_HINT);
}

std::filesystem::path
TgFocusData::path_filters () const
{
  auto tmp = this->data_root;
  return tmp.append (FILE_FILTERS);
}

std::filesystem::path
TgFocusData::path_filters_tmp () const
{
  auto tmp = this->data_root;
  return tmp.append (FILE_FILTERS_TMP);
}

std::filesystem::path
TgFocusData::path_tgfid () const
{
  auto tmp = this->data_root;
  return tmp.append (FILE_TGFID);
}

std::filesystem::path
TgFocusData::path_pref_lang () const
{
  fs::path tmp = this->data_root;
  return tmp.append (FILE_PREF_LANG);
}

bool
TgFocusData::prepare_filters_tmp () const
{
  std::filesystem::copy (this->path_filters (), this->path_filters_tmp (),
			 std::filesystem::copy_options::overwrite_existing);
  return true;
}

std::string
TgFocusData::get_api_id () const
{
  FileReader freader{this->path_api_id ().c_str ()};
  auto strdata = freader.read_to_string ().value_or ("-");
  trim (strdata);
  return strdata;
}

std::int32_t
TgFocusData::get_api_id_as_int32 () const
{
  auto strdata = this->get_api_id ();

  return std::atoi (strdata.c_str ());
}

std::string
TgFocusData::get_api_hash () const
{
  FileReader freader{this->path_api_hash ().c_str ()};
  auto strdata = freader.read_to_string ().value_or ("-");
  trim (strdata);
  return strdata;
}

bool
TgFocusData::get_auth_hint () const
{
  FileReader freader{this->path_auth_hint ().c_str ()};
  auto strdata = freader.read_to_string ().value_or ("-");
  if (strdata == "a")
    return true;
  else if (strdata == "u")
    return false;
  else
    return false;
}

std::string
TgFocusData::get_filters () const
{
  FileReader freader{this->path_filters ().c_str ()};
  auto strdata = freader.read_to_string ().value_or ("-");
  // trim (strdata);
  return strdata;
}

std::string
TgFocusData::get_filters_tmp () const
{
  FileReader freader{this->path_filters_tmp ().c_str ()};
  auto strdata = freader.read_to_string ().value_or ("-");
  // trim (strdata);
  return strdata;
}

// setter

void
TgFocusData::set_api_id (std::string &&in) const
{
  if (in.length () == 0)
    return;
  // check convert to int
  long as_num = std::strtol (in.c_str (), nullptr, 10);
  if (as_num < INT_MIN || as_num > INT_MAX)
    return;

  if (std::to_string (as_num) != in)
    return;

  auto path = this->path_api_id ();
  auto filename = path.c_str ();
  FILE *f = std::fopen (filename, "w+");
  if (f == nullptr)
    return;
  fwrite (in.c_str (), 1, in.length (), f);
  fclose (f);
}

void
TgFocusData::set_api_hash (std::string &&in) const
{
  if (in.length () == 0)
    return;

  auto path = this->path_api_hash ();
  auto filename = path.c_str ();
  FILE *f = std::fopen (filename, "w+");
  if (f == nullptr)
    return;
  std::fwrite (in.c_str (), 1, in.length (), f);
  fclose (f);
}

// this flag will critically affect authorization process
void
TgFocusData::set_auth_hint (bool flag)
{
  auto path = this->path_auth_hint ();
  auto filename = path.c_str ();
  FILE *f = std::fopen (filename, "w");
  if (f == nullptr)
    return;
  if (flag)
    std::fwrite ("a", 1, 1, f);
  else
    std::fwrite ("u", 1, 1, f);

  fclose (f);
}

void
TgFocusData::set_filters (std::string &&in) const
{
  if (in.length () == 0)
    return;

  // LOCK
  if (!lock_stream (this->lck_filters))
    return;

  auto path = this->path_filters ();
  auto filename = path.c_str ();
  FILE *f = std::fopen (filename, "w");
  if (f == nullptr)
    return;
  std::fwrite (in.c_str (), 1, in.length (), f);
  fclose (f);

  // UNLOCK
  if (!unlock_stream (this->lck_filters))
    return;
}

void
TgFocusData::set_tgfid (int64_t in) const
{
  auto path = this->path_tgfid ();
  auto filename = path.c_str ();
  FILE *f = std::fopen (filename, "wb");

  int64_t tmp = in;
  for (int i = 0; i < 8; i++)
    {
      if (i > 0)
	tmp = tmp >> 8;
      char curr = static_cast<char> (tmp & 0xff);
      fwrite (&curr, 1, 1, f);
    }

  fclose (f);
}

int64_t
TgFocusData::get_tgfid () const
{
  auto path = this->path_tgfid ();
  auto filename = path.c_str ();
  FILE *f = fopen (filename, "r");

  int64_t res = 0;
  for (int i = 0; i < 8; i++)
    {
      uint8_t b = 0;
      fread (&b, 1, 1, f);
      res = ((static_cast<int64_t> (b) << (8 * i)) | res);
    }

  fclose (f);

  return res;
}

bool
TgFocusData::is_tgfid_valid () const
{
  auto path = this->path_tgfid ();
  auto filename = path.c_str ();
  FILE *f = fopen (filename, "r");

  int64_t res = 0;
  for (int i = 0; i < 8; i++)
    {
      uint8_t b = 0;
      fread (&b, 1, 1, f);
      res = ((static_cast<int64_t> (b) << (8 * i)) | res);
    }

  fclose (f);

  return res != -1;
}

tgf::Lang
TgFocusData::get_pref_lang () const
{
  auto path = this->path_pref_lang ();
  auto filename = path.c_str ();
  FILE *f = fopen (filename, "r");
  uint8_t b = 0;
  fread (&b, 1, 1, f);
  tgf::Lang ret = static_cast<tgf::Lang> (b);
  fclose (f);

  return ret;
}

void
TgFocusData::set_pref_lang (tgf::Lang l) const
{
  auto path = this->path_pref_lang ();
  auto filename = path.c_str ();
  FILE *f = fopen (filename, "w");
  uint8_t b = static_cast<uint8_t> (l);
  fwrite (&b, 1, 1, f);

  fclose (f);
}

//  -  tgfid type  -  //

static constexpr auto FILENAME_TGFID_TYPE_PREF = "tgfid_t";

std::filesystem::path
path_tgfid_type_pref (const std::filesystem::path &droot)
{
  return droot / FILENAME_TGFID_TYPE_PREF;
}

void
set_tgfid_type_fs (const std::filesystem::path &droot, uint8_t t)
{
  auto path = path_tgfid_type_pref (droot);
  auto filename = path.c_str ();
  FILE *f = fopen (filename, "w");
  uint8_t b = t;
  fwrite (&b, 1, 1, f);
  fclose (f);
}

void
init_tgfid_type_fs (const std::filesystem::path &droot)
{
  if (!std::filesystem::exists (droot / FILENAME_TGFID_TYPE_PREF))
    set_tgfid_type_fs (droot, 2);
}

uint8_t
get_tgfid_type_fs (const std::filesystem::path &droot)
{
  auto path = path_tgfid_type_pref (droot);
  auto filename = path.c_str ();
  FILE *f = fopen (filename, "r");
  uint8_t ret = 0;
  fread (&ret, 1, 1, f);
  fclose (f);
  return ret;
}

void
TgFocusData::set_basic_tgfid () const
{
  set_tgfid_type_fs (this->data_root, 2);
}

void
TgFocusData::set_super_tgfid () const
{
  set_tgfid_type_fs (this->data_root, 3);
}

bool
TgFocusData::is_basic_tgfid () const
{
  return get_tgfid_type_fs (this->data_root) == 2;
}

bool
TgFocusData::is_super_tgfid () const
{
  return get_tgfid_type_fs (this->data_root) == 2;
}
