#ifndef _TGF_FILTER_H
#define _TGF_FILTER_H 1

#include <concepts>
#include <string>
#include <vector>
#include "toml.hpp"
#include "posix_regex.hh"
#include "tgf_msg.hh"

enum FocusDecision
{
  Accept,
  Skip,
  Reject
};

template <typename T>
concept HasFilterFields = std::same_as<T, toml::value> || false;

template <typename V>
  requires HasFilterFields<V>
class TgfFilter
{
public:
  TgfFilter () = default;

  TgfFilter (TgfFilter &&other);

  TgfFilter &operator= (TgfFilter &&other);

  // FIXME: is_xyz_match family should be private

  bool is_title_match (const std::string &input);

  bool is_sender_match (const std::string &input);

  bool is_no_sender_match (const std::string &input);

  bool is_rej_sender_match (const std::string &input);

  bool is_keyword_match (const std::string &input);

  bool is_no_keyword_match (const std::string &input);

protected:
  PosixExtRegex chat_title;
  std::vector<PosixExtRegex> senders;
  std::vector<PosixExtRegex> no_senders;
  std::vector<PosixExtRegex> rej_senders;
  std::vector<PosixExtRegex> keywords;
  std::vector<PosixExtRegex> no_keywords;

  // Check whether this "Focus Filter" can match the input. If any xyz filter
  // not matched, or any no-xyz filter matched, then is false, otherwise
  // true(i.e. all xyz filters match and all no-xyz filters cannot match the
  // input).
  FocusDecision is_tgmsg_match (const TgMsg &input);

  friend class FocusFilterList;
};

class TgfFilterToml : TgfFilter<toml::value>
{
public:
  explicit TgfFilterToml (const toml::value &v);
};

template <typename F> class TgfFilterGroup
{
public:
  // TgfFilterGroup () = delete;

  // TgfFilterGroup (const toml::value &v);

  // TgfFilterGroup (const char *v);

  // TgfFilterGroup (const std::string &v);

  // static bool is_valid (const std::string &v);

  inline size_t n_filter () { return this->filters.size (); }
  inline size_t i_prev_matched () { return this->i_prev_matched_; }

  // Check whether this "Focus Filter List" can match the input. If any
  // enclosing filter matches input(i.e. "is_tgmsg_match" returns true), the
  // result is true, otherwise false. Note that this differs largly from
  // sub-field matching mechanism.
  bool is_tgmsg_match (const TgMsg &in);

protected:
  size_t i_prev_matched_;
  std::vector<F> filters;
};

class TgfFilterGroupToml : TgfFilterGroup<TgfFilterToml>
{
public:
  TgfFilterGroupToml () = delete;
  explicit TgfFilterGroupToml (const toml::value &v);
};

#endif // _H
