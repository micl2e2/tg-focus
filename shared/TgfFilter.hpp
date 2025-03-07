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

// -------------------------- forward decl --------------------------

template <typename T>
concept HasFilterFields = std::same_as<T, toml::value> || false;

template <typename V>
  requires HasFilterFields<V>
class TgfFilter;

template <typename F, typename V>
concept CanFilterRecogValue = std::derived_from<F, TgfFilter<V>>;

template <typename V, typename F>
  requires CanFilterRecogValue<F, V>
class TgfFilterGroup;

// --------------------------- TgfFilter ---------------------------

template <typename V>
  requires HasFilterFields<V>
class TgfFilter
{
public:
  TgfFilter () = default;

  TgfFilter (TgfFilter &&other);

  TgfFilter &operator= (TgfFilter &&other);

  // explicit TgfFilter (const V &v);
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

  template <typename _V, typename _F>
    requires CanFilterRecogValue<_F, _V>
  friend class TgfFilterGroup;
};

class TgfFilterToml : public TgfFilter<toml::value>
{
public:
  explicit TgfFilterToml (const toml::value &v);
};

// ------------------------- TgfFilterGroup -------------------------

template <typename V, typename F>
  requires CanFilterRecogValue<F, V>
class TgfFilterGroup
{
public:
  // TgfFilterGroup () = delete;

  // TgfFilterGroup (const toml::value &v);
  // explicit TgfFilterGroup (const V &v);

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

class TgfFilterGroupToml : public TgfFilterGroup<toml::value, TgfFilterToml>
{
public:
  TgfFilterGroupToml () = delete;
  explicit TgfFilterGroupToml (const toml::value &v);
};

// ---------------------- TgfFilter<> Impl ----------------------

template <typename V>
  requires HasFilterFields<V>
TgfFilter<V>::TgfFilter (TgfFilter<V> &&move_ctor_from)
{
  this->chat_title = std::move (move_ctor_from.chat_title);
  this->senders = std::move (move_ctor_from.senders);
  this->no_senders = std::move (move_ctor_from.no_senders);
  this->rej_senders = std::move (move_ctor_from.rej_senders);
  this->keywords = std::move (move_ctor_from.keywords);
  this->no_keywords = std::move (move_ctor_from.no_keywords);
}

template <typename V>
  requires HasFilterFields<V>
TgfFilter<V> &
TgfFilter<V>::operator= (TgfFilter<V> &&move_assign_from)
{
  this->chat_title = std::move (move_assign_from.chat_title);
  this->senders = std::move (move_assign_from.senders);
  this->no_senders = std::move (move_assign_from.no_senders);
  this->keywords = std::move (move_assign_from.keywords);
  this->no_keywords = std::move (move_assign_from.no_keywords);
  return *this;
}

template <typename V>
  requires HasFilterFields<V>
bool
TgfFilter<V>::is_title_match (const std::string &input)
{
  if (auto flag = this->chat_title.is_match (input))
    {
      if (*flag)
	return true;
    }

  return false;
}

template <typename V>
  requires HasFilterFields<V>
bool
TgfFilter<V>::is_keyword_match (const std::string &input)
{
  // if no candidates, match anything
  if (this->keywords.size () == 0)
    return true;

  for (PosixExtRegex &re : this->keywords)
    {
      if (auto flag = re.is_match (input))
	{
	  if (*flag)
	    return true;
	}
    }

  return false;
}

template <typename V>
  requires HasFilterFields<V>
bool
TgfFilter<V>::is_no_keyword_match (const std::string &input)
{
  // if no candidates, not match anything
  if (this->no_keywords.size () == 0)
    return false;

  for (PosixExtRegex &re : this->no_keywords)
    {
      if (auto flag = re.is_match (input))
	{
	  if (*flag)
	    return true;
	}
    }

  return false;
}

template <typename V>
  requires HasFilterFields<V>
bool
TgfFilter<V>::is_sender_match (const std::string &input)
{
  if (this->senders.size () == 0)
    return true;

  for (PosixExtRegex &re : this->senders)
    {
      if (auto flag = re.is_match (input))
	{
	  if (*flag)
	    return true;
	}
    }

  return false;
}

template <typename V>
  requires HasFilterFields<V>
bool
TgfFilter<V>::is_no_sender_match (const std::string &input)
{
  // if (this->no_senders.size () == 0)
  //   return true;

  for (PosixExtRegex &re : this->no_senders)
    {
      if (auto flag = re.is_match (input))
	{
	  if (*flag)
	    return true;
	}
    }

  return false;
}

template <typename V>
  requires HasFilterFields<V>
bool
TgfFilter<V>::is_rej_sender_match (const std::string &input)
{
  // if (this->no_senders.size () == 0)
  //   return true;

  for (PosixExtRegex &re : this->rej_senders)
    {
      if (auto flag = re.is_match (input))
	{
	  if (*flag)
	    return true;
	}
    }

  return false;
}

template <typename V>
  requires HasFilterFields<V>
FocusDecision
TgfFilter<V>::is_tgmsg_match (const TgMsg &in)
{
  if (!this->is_title_match (in.get_chat_title ()))
    return FocusDecision::Skip;

  if (!this->is_sender_match (in.get_sender ()))
    return FocusDecision::Skip;

  if (this->is_no_sender_match (in.get_sender ()))
    return FocusDecision::Skip;

  if (this->is_rej_sender_match (in.get_sender ()))
    return FocusDecision::Reject;

  if (!this->is_keyword_match (in.get_text_content ()))
    return FocusDecision::Skip;

  if (this->is_no_keyword_match (in.get_text_content ()))
    return FocusDecision::Skip;

  return FocusDecision::Accept;
}

// --------------------- TgfFilterGroup<> Impl ---------------------

template <typename V, typename F>
  requires CanFilterRecogValue<F, V>
bool
TgfFilterGroup<V, F>::is_tgmsg_match (const TgMsg &in)
{
  this->i_prev_matched_ = 0;

  for (auto &f : this->filters)
    {
      if (f.is_tgmsg_match (in) == FocusDecision::Accept)
	{
	  return true;
	}
      if (f.is_tgmsg_match (in) == FocusDecision::Reject)
	{
	  return false;
	}

      this->i_prev_matched_++;
    }

  return false;
}

#endif // _H
