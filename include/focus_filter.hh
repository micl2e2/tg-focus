#ifndef _FOCUS_FILTER_H
#define _FOCUS_FILTER_H 1

#include <string>
#include <vector>
#include <iostream>

#include "toml.hpp"
#include "posix_regex.hh"
#include "tf_msg.hh"

class FocusFilter
{
public:
  FocusFilter () = default;

  FocusFilter (FocusFilter &&other);

  FocusFilter &operator= (FocusFilter &&other);

  explicit FocusFilter (const toml::value &v);

  bool is_title_match (const std::string &input);

  bool is_sender_match (const std::string &input);

  bool is_no_sender_match (const std::string &input);

  // the input string can match any of keywords
  bool is_keyword_match (const std::string &input);

  // check whether the input string can match any of no-keyword list.
  bool is_no_keyword_match (const std::string &input);

  // If any xyz filter not matched, or any no-xyz filter matched, then is false,
  // otherwise true.
  bool is_tgmsg_match (const TgMsg &input);

  // private:
  PosixExtRegex chat_title;
  std::vector<PosixExtRegex> senders;
  std::vector<PosixExtRegex> no_senders;
  std::vector<PosixExtRegex> keywords;
  std::vector<PosixExtRegex> no_keywords;
};

class FocusFilterList
{
public:
  FocusFilterList () = delete;

  //
  // to be deprecated
  FocusFilterList (const toml::value &v);

  FocusFilterList (const char *v);

  FocusFilterList (const std::string &v);

  static bool is_valid (const std::string &v);

  inline size_t n_filter () { return this->filters.size (); }
  inline size_t i_prev_matched () { return this->i_prev_matched_; }

  // If any enclosing filter's "is_tgmsg_match" is true, then is true, otherwise
  // false. This differs largly from sub-field matching mechanism.
  bool is_tgmsg_match (const TgMsg &in);

private:
  size_t i_prev_matched_;
  std::vector<FocusFilter> filters;
};

#endif // _FOCUS_FILTER_H
