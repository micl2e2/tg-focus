#ifndef _TGF_FILTER_H
#define _TGF_FILTER_H 1

#include <concepts>
#include <string>
#include <vector>
#include "toml.hpp"
#include "PosixRegex.hpp"
#include "TgfMsg.hpp"

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

  bool isMatchTitle (const std::string &input);

  bool isMatchSender (const std::string &input);

  bool isNoSendersMatch (const std::string &input);

  bool isRejSendersMatch (const std::string &input);

  bool isKeywordsMatch (const std::string &input);

  bool isNoKeywordsMatch (const std::string &input);

protected:
  PosixExtRegex chat_title;
  std::vector<PosixExtRegex> senders;
  std::vector<PosixExtRegex> no_senders;
  std::vector<PosixExtRegex> rej_senders;
  std::vector<PosixExtRegex> keywords;
  std::vector<PosixExtRegex> no_keywords;

  FocusDecision isMatchTgfMsg (const TgfMsg &input);

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
  inline size_t n_filter () { return this->filters.size (); }
  inline size_t i_prev_matched () { return this->i_prev_matched_; }

  bool isMatchTgfMsg (const TgfMsg &in);

protected:
  size_t i_prev_matched_;
  std::vector<F> filters;
};

class TgfFilterGroupToml : public TgfFilterGroup<toml::value, TgfFilterToml>
{
public:
  TgfFilterGroupToml () = delete;
  explicit TgfFilterGroupToml (const toml::value &v) noexcept;
  // no const bc both are basic_value
  explicit TgfFilterGroupToml (std::string &v) noexcept;
  // Has at least one filter
  bool isEffective () noexcept { return this->filters.size () > 0; }
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
TgfFilter<V>::isMatchTitle (const std::string &input)
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
TgfFilter<V>::isKeywordsMatch (const std::string &input)
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
TgfFilter<V>::isNoKeywordsMatch (const std::string &input)
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
TgfFilter<V>::isMatchSender (const std::string &input)
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
TgfFilter<V>::isNoSendersMatch (const std::string &input)
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
TgfFilter<V>::isRejSendersMatch (const std::string &input)
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
TgfFilter<V>::isMatchTgfMsg (const TgfMsg &in)
{
  if (!this->isMatchTitle (in.get_chat_title ()))
    return FocusDecision::Skip;

  if (!this->isMatchSender (in.get_sender ()))
    return FocusDecision::Skip;

  if (this->isNoSendersMatch (in.get_sender ()))
    return FocusDecision::Skip;

  if (this->isRejSendersMatch (in.get_sender ()))
    return FocusDecision::Reject;

  if (!this->isKeywordsMatch (in.get_text_content ()))
    return FocusDecision::Skip;

  if (this->isNoKeywordsMatch (in.get_text_content ()))
    return FocusDecision::Skip;

  return FocusDecision::Accept;
}

// --------------------- TgfFilterGroup<> Impl ---------------------

template <typename V, typename F>
  requires CanFilterRecogValue<F, V>
bool
TgfFilterGroup<V, F>::isMatchTgfMsg (const TgfMsg &in)
{
  this->i_prev_matched_ = 0;

  for (auto &f : this->filters)
    {
      if (f.isMatchTgfMsg (in) == FocusDecision::Accept)
	{
	  return true;
	}
      if (f.isMatchTgfMsg (in) == FocusDecision::Reject)
	{
	  return false;
	}

      this->i_prev_matched_++;
    }

  return false;
}

#endif // _H
