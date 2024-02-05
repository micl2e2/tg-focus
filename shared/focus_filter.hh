#ifndef _FOCUS_FILTER_H
#define _FOCUS_FILTER_H 1

#include <string>
#include <vector>
#include <iostream>

#include "toml.hpp"
#include "posix_regex.hh"
#include "tgf_msg.hh"

enum FocusDecision
{
  Accept,
  Skip,
  Reject
};

class FocusFilter
{
public:
  FocusFilter () = default;

  FocusFilter (FocusFilter &&other);

  FocusFilter &operator= (FocusFilter &&other);

  explicit FocusFilter (const toml::value &v);

  // FIXME: is_xyz_match family should be private

  //
  // Check whether the input string can match "title" filter.
  bool is_title_match (const std::string &input);

  //
  // Check whether the input string can match any of "senders" filter list.
  bool is_sender_match (const std::string &input);

  //
  // Check whether the input string can match any of "no-senders" filter list.
  bool is_no_sender_match (const std::string &input);

  //
  // Check whether the input string can match any of "rej-senders" filter list.
  bool is_rej_sender_match (const std::string &input);

  //
  // Check whether the input string can match any of "keywords" filter list.
  bool is_keyword_match (const std::string &input);

  //
  // Check whether the input string can match any of "no-keywords" filter list.
  bool is_no_keyword_match (const std::string &input);

private:
  PosixExtRegex chat_title;
  std::vector<PosixExtRegex> senders;
  std::vector<PosixExtRegex> no_senders;
  std::vector<PosixExtRegex> rej_senders;
  std::vector<PosixExtRegex> keywords;
  std::vector<PosixExtRegex> no_keywords;

  //
  // Check whether this "Focus Filter" can match the input. If any xyz filter
  // not matched, or any no-xyz filter matched, then is false, otherwise
  // true(i.e. all xyz filters match and all no-xyz filters cannot match the
  // input).
  FocusDecision is_tgmsg_match (const TgMsg &input);

  friend class FocusFilterList;
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

  //
  // Check whether this "Focus Filter List" can match the input. If any
  // enclosing filter matches input(i.e. "is_tgmsg_match" returns true), the
  // result is true, otherwise false. Note that this differs largly from
  // sub-field matching mechanism.
  bool is_tgmsg_match (const TgMsg &in);

private:
  size_t i_prev_matched_;
  std::vector<FocusFilter> filters;
};

#endif // _FOCUS_FILTER_H
