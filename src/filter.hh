#ifndef hh_filter
#define hh_filter

#include <concepts>
#include <string>
#include <utility>
#include <vector>
#include "std_comp.hh"
#include "toml.hpp"
#include "posix_regex.hh"
#include "tg_msg.hh"

namespace tgf {

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
class Filter;

template <typename F, typename V>
concept CanFilterRecogValue = std::derived_from<F, Filter<V>>;

template <typename V, typename F>
  requires CanFilterRecogValue<F, V>
class FilterGroup;

// --------------------------- Filter ---------------------------

template <typename V>
  requires HasFilterFields<V>
class Filter
{
public:
  Filter ();

  Filter (Filter &&other);

  Filter &operator= (Filter &&other);

  bool isMatchTitle (const std::string &input);

  bool isMatchSender (const std::string &input);

  bool isNoSendersMatch (const std::string &input);

  bool isRejSendersMatch (const std::string &input);

  bool isKeywordsMatch (const std::string &input);

  bool isNoKeywordsMatch (const std::string &input);

  string as_readable () const;

  string getptn_title () const;

protected:
  PosixExtRegex chat_title;
  std::vector<PosixExtRegex> senders;
  std::vector<PosixExtRegex> no_senders;
  std::vector<PosixExtRegex> rej_senders;
  std::vector<PosixExtRegex> keywords;
  std::vector<PosixExtRegex> no_keywords;

  FocusDecision isMatchTgMsg (const TgMsg &input);

  template <typename _V, typename _F>
    requires CanFilterRecogValue<_F, _V>
  friend class FilterGroup;
  friend class FilterGroupToml;
};

class FilterToml : public Filter<toml::value>
{
public:
  FilterToml () {}
  explicit FilterToml (const toml::value &v);
};

// ------------------------- FilterGroup -------------------------

typedef enum
{
  FilterPropertyTitle,
  FilterPropertyKeywords,
  FilterPropertyNoKeywords,
  FilterPropertySenders,
  FilterPropertyNoSenders,
  FilterPropertyRejSenders,
} FilterProperty;

template <typename V, typename F>
  requires CanFilterRecogValue<F, V>
class FilterGroup
{
public:
  inline size_t n_filter () { return this->filters.size (); }
  inline size_t i_prev_matched () { return this->i_prev_matched_; }

  bool isMatchTgMsg (const TgMsg &in);
  string as_readable () const;
  string as_fsdata () const noexcept;
  bool rm_filter (const u32 &which_filter) noexcept;
  bool add_one (u32 &which_filter, const FilterProperty p, const string &value);
  bool del_one (u32 &which_filter, const FilterProperty p, const string &value);
  bool add_keywords (u32 &which_filter, const string &value);
  bool del_keywords (u32 &which_filter, const string &value);
  bool add_no_keywords (u32 &which_filter, const string &value);
  bool del_no_keywords (u32 &which_filter, const string &value);
  // bool add_rej_keywords (u32 &which_filter, const string &value);
  bool add_senders (u32 &which_filter, const string &value);
  bool del_senders (u32 &which_filter, const string &value);
  bool add_no_senders (u32 &which_filter, const string &value);
  bool del_no_senders (u32 &which_filter, const string &value);
  bool add_rej_senders (u32 &which_filter, const string &value);
  bool del_rej_senders (u32 &which_filter, const string &value);

protected:
  size_t i_prev_matched_;
  std::vector<F> filters;
};

class FilterGroupToml : public FilterGroup<toml::value, FilterToml>
{
public:
  FilterGroupToml () = delete;
  explicit FilterGroupToml (const toml::value &v) noexcept;
  // no const bc both are basic_value
  explicit FilterGroupToml (std::string &v) noexcept;
  // Has at least one filter
  bool isEffective () noexcept { return this->filters.size () > 0; }
  string as_fsdata () noexcept;
};

// ---------------------- Filter<> Impl ----------------------

template <typename V>
  requires HasFilterFields<V>
Filter<V>::Filter ()
{
  PosixExtRegex re (".*");
  chat_title = move (re);
}

template <typename V>
  requires HasFilterFields<V>
Filter<V>::Filter (Filter<V> &&move_ctor_from)
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
Filter<V> &
Filter<V>::operator= (Filter<V> &&move_assign_from)
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
Filter<V>::isMatchTitle (const std::string &input)
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
Filter<V>::isKeywordsMatch (const std::string &input)
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
Filter<V>::isNoKeywordsMatch (const std::string &input)
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

inline string
readable_eles (const vector<PosixExtRegex> &list)
{
  ostringstream oss;
  for (int i = 0; i < list.size (); i++)
    {
      const PosixExtRegex &el = list[i];
      oss << "(" << el.ptn () << ")";
      if (i < list.size () - 1)
	oss << "  "; // double spc
      else
	oss << endl;
    }
  return oss.str ();
}

template <typename V>
  requires HasFilterFields<V>
string
Filter<V>::getptn_title () const
{
  return chat_title.ptn ();
}

template <typename V>
  requires HasFilterFields<V>
string
Filter<V>::as_readable () const
{
  ostringstream oss;
  oss << "🞄 Accept Title 🞄" << endl
      << "(" << chat_title.ptn () << ")" << endl
      << endl;
  oss << "🞄 Accept Words 🞄" << endl << readable_eles (keywords) << endl;
  oss << "🞄 No Words 🞄" << endl << readable_eles (no_keywords) << endl;
  oss << "🞄 Accept Senders 🞄" << endl << readable_eles (senders) << endl;
  oss << "🞄 Reject Senders 🞄" << endl << readable_eles (rej_senders) << endl;
  oss << "🞄 No Senders 🞄" << endl << readable_eles (no_senders);
  // if no candidates, not match anything

  return oss.str ();
}

template <typename V>
  requires HasFilterFields<V>
bool
Filter<V>::isMatchSender (const std::string &input)
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
Filter<V>::isNoSendersMatch (const std::string &input)
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
Filter<V>::isRejSendersMatch (const std::string &input)
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
Filter<V>::isMatchTgMsg (const TgMsg &in)
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

// --------------------- FilterGroup<> Impl ---------------------

template <typename V, typename F>
  requires CanFilterRecogValue<F, V>
bool
FilterGroup<V, F>::isMatchTgMsg (const TgMsg &in)
{
  this->i_prev_matched_ = 0;

  for (auto &f : this->filters)
    {
      if (f.isMatchTgMsg (in) == FocusDecision::Accept)
	{
	  return true;
	}
      if (f.isMatchTgMsg (in) == FocusDecision::Reject)
	{
	  return false;
	}

      this->i_prev_matched_++;
    }

  return false;
}

template <typename V, typename F>
  requires CanFilterRecogValue<F, V>
string
FilterGroup<V, F>::as_readable () const
{
  ostringstream oss;
  for (int i = 0; i < filters.size (); i++)
    {
      const F &el = filters[i];
      oss << "🞋 🞋 🞋 " << "FILTER " << (i + 1) << " 🞋 🞋 🞋" << endl << endl;
      oss << el.as_readable () << endl;
    }

  return oss.str ();
}

template <typename V, typename F>
  requires CanFilterRecogValue<F, V>
bool
FilterGroup<V, F>::rm_filter (const u32 &which_filter) noexcept
{
  u32 idx = which_filter - 1;
  if (idx < 0 || filters.size () == 1 || idx > filters.size () - 1)
    {
      return false;
    }
  size_t oldsz = filters.size ();
  filters.erase (filters.begin () + idx);
  size_t newsz = filters.size ();
  if (newsz == oldsz - 1)
    return true;
  return false;
}

template <typename V, typename F>
  requires CanFilterRecogValue<F, V>
bool
FilterGroup<V, F>::add_one (u32 &which_filter, const FilterProperty p,
			    const string &value)
{
  if (which_filter < 1)
    return false;
  u32 idx = which_filter - 1;
  if (idx > filters.size () - 1)
    {
      F f;
      filters.emplace_back (move (f)); // exclusively for add_one
      idx = filters.size () - 1;
      which_filter = filters.size ();
    }
  PosixExtRegex may_re (value);
  if (!may_re.is_pattern_accept ())
    return false;
  switch (p)
    {
    case FilterPropertyTitle:
      filters[idx].chat_title = move (may_re);
      break;
    case FilterPropertyKeywords:
      filters[idx].keywords.emplace_back (move (may_re));
      break;
    case FilterPropertyNoKeywords:
      filters[idx].no_keywords.emplace_back (move (may_re));
      break;
    case FilterPropertySenders:
      filters[idx].senders.emplace_back (move (may_re));
      break;
    case FilterPropertyNoSenders:
      filters[idx].no_senders.emplace_back (move (may_re));
      break;
    case FilterPropertyRejSenders:
      filters[idx].rej_senders.emplace_back (move (may_re));
      break;
    }
  return true;
}

template <typename V, typename F>
  requires CanFilterRecogValue<F, V>
bool
FilterGroup<V, F>::del_one (u32 &which_filter, const FilterProperty p,
			    const string &value)
{
  if (which_filter < 1)
    return false;
  u32 idx = which_filter - 1;
  if (idx > filters.size () - 1)
    {
      idx = filters.size () - 1;
      which_filter = filters.size ();
    }
  PosixExtRegex may_re (value);
  if (!may_re.is_pattern_accept ())
    return false;

  if (p == FilterPropertyTitle)
    {
      if (filters[idx].chat_title.ptn () == value)
	filters[idx].chat_title = PosixExtRegex (".*");
    }
  else
    {
      vector<PosixExtRegex> &list_keywords = filters[idx].keywords;
      vector<PosixExtRegex> &list_no_keywords = filters[idx].no_keywords;
      vector<PosixExtRegex> &list_senders = filters[idx].senders;
      vector<PosixExtRegex> &list_no_senders = filters[idx].no_senders;
      vector<PosixExtRegex> &list_rej_senders = filters[idx].rej_senders;
      using it_type = vector<PosixExtRegex>::iterator;
      it_type begit = list_keywords.begin ();
      it_type endit = list_keywords.end ();
      switch (p)
	{
	case FilterPropertyKeywords:
	  begit = list_keywords.begin ();
	  endit = list_keywords.end ();
	  for (it_type it = begit; it != endit; it++)
	    if (it->ptn () == value)
	      {
		it = list_keywords.erase (it);
		break;
	      }
	  break;
	case FilterPropertyNoKeywords:
	  begit = list_no_keywords.begin ();
	  endit = list_no_keywords.end ();
	  for (it_type it = begit; it != endit; it++)
	    if (it->ptn () == value)
	      {
		it = list_no_keywords.erase (it);
		break;
	      }
	  break;
	case FilterPropertySenders:
	  begit = list_senders.begin ();
	  endit = list_senders.end ();
	  for (it_type it = begit; it != endit; it++)
	    if (it->ptn () == value)
	      {
		it = list_senders.erase (it);
		break;
	      }
	  break;
	case FilterPropertyNoSenders:
	  begit = list_no_senders.begin ();
	  endit = list_no_senders.end ();
	  for (it_type it = begit; it != endit; it++)
	    if (it->ptn () == value)
	      {
		it = list_no_senders.erase (it);
		break;
	      }
	  break;
	case FilterPropertyRejSenders:
	  begit = list_rej_senders.begin ();
	  endit = list_rej_senders.end ();
	  for (it_type it = begit; it != endit; it++)
	    if (it->ptn () == value)
	      {
		it = list_rej_senders.erase (it);
		break;
	      }
	  break;
	}
    }

  return true;
}

template <typename V, typename F>
  requires CanFilterRecogValue<F, V>
bool
FilterGroup<V, F>::add_keywords (u32 &which_filter, const string &value)
{
  return add_one (which_filter, FilterPropertyKeywords, value);
}

template <typename V, typename F>
  requires CanFilterRecogValue<F, V>
bool
FilterGroup<V, F>::del_keywords (u32 &which_filter, const string &value)
{
  return del_one (which_filter, FilterPropertyKeywords, value);
}

template <typename V, typename F>
  requires CanFilterRecogValue<F, V>
bool
FilterGroup<V, F>::add_no_keywords (u32 &which_filter, const string &value)
{
  return add_one (which_filter, FilterPropertyNoKeywords, value);
}

template <typename V, typename F>
  requires CanFilterRecogValue<F, V>
bool
FilterGroup<V, F>::del_no_keywords (u32 &which_filter, const string &value)
{
  return del_one (which_filter, FilterPropertyNoKeywords, value);
}

template <typename V, typename F>
  requires CanFilterRecogValue<F, V>
bool
FilterGroup<V, F>::add_senders (u32 &which_filter, const string &value)
{
  return add_one (which_filter, FilterPropertySenders, value);
}

template <typename V, typename F>
  requires CanFilterRecogValue<F, V>
bool
FilterGroup<V, F>::del_senders (u32 &which_filter, const string &value)
{
  return del_one (which_filter, FilterPropertySenders, value);
}

template <typename V, typename F>
  requires CanFilterRecogValue<F, V>
bool
FilterGroup<V, F>::add_no_senders (u32 &which_filter, const string &value)
{
  return add_one (which_filter, FilterPropertyNoSenders, value);
}

template <typename V, typename F>
  requires CanFilterRecogValue<F, V>
bool
FilterGroup<V, F>::del_no_senders (u32 &which_filter, const string &value)
{
  return del_one (which_filter, FilterPropertyNoSenders, value);
}

template <typename V, typename F>
  requires CanFilterRecogValue<F, V>
bool
FilterGroup<V, F>::add_rej_senders (u32 &which_filter, const string &value)
{
  return add_one (which_filter, FilterPropertyRejSenders, value);
}

template <typename V, typename F>
  requires CanFilterRecogValue<F, V>
bool
FilterGroup<V, F>::del_rej_senders (u32 &which_filter, const string &value)
{
  return del_one (which_filter, FilterPropertyRejSenders, value);
}

} // namespace tgf

#endif
