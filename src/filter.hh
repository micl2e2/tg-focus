#ifndef hh_filter
#define hh_filter

#include <concepts>
#include <algorithm>
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

template <typename V> requires HasFilterFields<V> class Filter;

template <typename F, typename V>
concept CanFilterRecogValue = std::derived_from<F, Filter<V>>;

template <typename V, typename F>
requires CanFilterRecogValue<F, V> class FilterGroup;

// --------------------------- Filter ---------------------------

template <typename V> requires HasFilterFields<V> class Filter
{
public:
  Filter ();

  Filter (Filter &&other);

  Filter &operator= (Filter &&other);

  bool mtch_titles (const std::string &input);

  bool mtch_no_titles (const std::string &input);

  bool mtch_senders (const std::string &input);

  bool mtch_no_senders (const std::string &input);

  bool mtch_keywords (const std::string &input);

  bool mtch_no_keywords (const std::string &input);

  string as_readable () const;

  string getptn_title () const;

protected:
  std::vector<PosixExtRegex> __titles;
  std::vector<PosixExtRegex> __no_titles;
  std::vector<PosixExtRegex> senders;
  std::vector<PosixExtRegex> no_senders;
  std::vector<PosixExtRegex> keywords;
  std::vector<PosixExtRegex> no_keywords;

  FocusDecision mtch_tgmsg (const TgMsg &input);

  template <typename _V, typename _F>
  requires CanFilterRecogValue<_F, _V> friend class FilterGroup;
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
  FilterPropertyTitles,
  FilterPropertyNoTitles,
  FilterPropertyKeywords,
  FilterPropertyNoKeywords,
  FilterPropertySenders,
  FilterPropertyNoSenders,
} FilterProperty;

template <typename V, typename F>
requires CanFilterRecogValue<F, V> class FilterGroup
{
public:
  inline size_t n_filter () { return this->filters.size (); }
  inline size_t i_prev_visited () { return this->i_prev_visited_; }

  bool mtch_tgmsg (const TgMsg &in);
  string as_readable () const;
  string as_fsdata () const noexcept;
  bool rm_filter (const u32 &which_filter) noexcept;
  bool add_one (u32 &which_filter, const FilterProperty p, const string &value);
  bool del_one (u32 &which_filter, const FilterProperty p, const string &value);
  bool add_titles (u32 &which_filter, const string &value);
  bool del_titles (u32 &which_filter, const string &value);
  bool add_no_titles (u32 &which_filter, const string &value);
  bool del_no_titles (u32 &which_filter, const string &value);
  bool add_senders (u32 &which_filter, const string &value);
  bool del_senders (u32 &which_filter, const string &value);
  bool add_no_senders (u32 &which_filter, const string &value);
  bool del_no_senders (u32 &which_filter, const string &value);
  bool add_keywords (u32 &which_filter, const string &value);
  bool del_keywords (u32 &which_filter, const string &value);
  bool add_no_keywords (u32 &which_filter, const string &value);
  bool del_no_keywords (u32 &which_filter, const string &value);

protected:
  size_t i_prev_visited_;
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
{}

template <typename V>
requires HasFilterFields<V>
Filter<V>::Filter (Filter<V> &&move_ctor_from)
{
  this->__titles = std::move (move_ctor_from.__titles);
  this->__no_titles = std::move (move_ctor_from.__no_titles);
  this->senders = std::move (move_ctor_from.senders);
  this->no_senders = std::move (move_ctor_from.no_senders);
  this->keywords = std::move (move_ctor_from.keywords);
  this->no_keywords = std::move (move_ctor_from.no_keywords);
}

template <typename V>
requires HasFilterFields<V> Filter<V> &
Filter<V>::operator= (Filter<V> &&move_assign_from)
{
  this->__titles = std::move (move_assign_from.__titles);
  this->__no_titles = std::move (move_assign_from.__no_titles);
  this->senders = std::move (move_assign_from.senders);
  this->no_senders = std::move (move_assign_from.no_senders);
  this->keywords = std::move (move_assign_from.keywords);
  this->no_keywords = std::move (move_assign_from.no_keywords);
  return *this;
}

template <typename V>
requires HasFilterFields<V> bool
Filter<V>::mtch_titles (const std::string &input)
{
  // if no candidates, match anything
  if (this->__titles.size () == 0)
    return true;

  for (PosixExtRegex &re : this->__titles)
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
requires HasFilterFields<V> bool
Filter<V>::mtch_no_titles (const std::string &input)
{
  // if no candidates, match anything
  // if (this->__no_titles.size () == 0)
  // return true;

  for (PosixExtRegex &re : this->__no_titles)
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
requires HasFilterFields<V> bool
Filter<V>::mtch_keywords (const std::string &input)
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
requires HasFilterFields<V> bool
Filter<V>::mtch_no_keywords (const std::string &input)
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
  oss << "⤷ ";
  for (int i = 0; i < list.size (); i++)
    {
      const PosixExtRegex &el = list[i];
      oss << "(" << el.ptn () << ")";
      if (i < list.size () - 1)
	oss << "  "; // double spc
    }
  oss << endl;
  return oss.str ();
}

template <typename V>
requires HasFilterFields<V> string
Filter<V>::as_readable () const
{
  ostringstream oss;
  oss << endl
      << "🞋 Titles <titles>" //
      << endl
      << readable_eles (__titles);
  oss << "🞋 Senders <senders>" //
      << endl
      << readable_eles (senders);
  oss << "🞋 Keywords <keywords>" //
      << endl
      << readable_eles (keywords);
  oss << "🞋 NO Titles <no-titles>" //
      << endl
      << readable_eles (__no_titles);
  oss << "🞋 NO Senders <no-senders>" //
      << endl
      << readable_eles (no_senders);
  oss << "🞋 NO Keywords <no-keywords>" //
      << endl
      << readable_eles (no_keywords);
  // if no candidates, not match anything

  return oss.str ();
}

template <typename V>
requires HasFilterFields<V> bool
Filter<V>::mtch_senders (const std::string &input)
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
requires HasFilterFields<V> bool
Filter<V>::mtch_no_senders (const std::string &input)
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
requires HasFilterFields<V> FocusDecision
Filter<V>::mtch_tgmsg (const TgMsg &in)
{
  // ACCEPT

  if (!this->mtch_titles (in.get_chat_title ()))
    return FocusDecision::Skip;

  if (!this->mtch_senders (in.get_sender ()))
    return FocusDecision::Skip;

  if (!this->mtch_keywords (in.get_text_content ()))
    return FocusDecision::Skip;

  // REJECT

  if (this->mtch_no_titles (in.get_chat_title ()))
    return FocusDecision::Reject;

  if (this->mtch_no_senders (in.get_sender ()))
    return FocusDecision::Reject;

  if (this->mtch_no_keywords (in.get_text_content ()))
    return FocusDecision::Reject;

  return FocusDecision::Accept;
}

// --------------------- FilterGroup<> Impl ---------------------

template <typename V, typename F>
requires CanFilterRecogValue<F, V> bool
FilterGroup<V, F>::mtch_tgmsg (const TgMsg &in)
{
  this->i_prev_visited_ = 0;

  for (auto &f : this->filters)
    {
      if (f.mtch_tgmsg (in) == FocusDecision::Accept)
	{
	  return true;
	}
      if (f.mtch_tgmsg (in) == FocusDecision::Reject)
	{
	  return false;
	}

      this->i_prev_visited_++; // prev skipped?
    }

  return false;
}

template <typename V, typename F>
requires CanFilterRecogValue<F, V> string
FilterGroup<V, F>::as_readable () const
{
  ostringstream oss;
  for (int i = 0; i < filters.size (); i++)
    {
      const F &el = filters[i];
      oss << endl
	  << "🞋 🞋 🞋 🞋 🞋 "
	  << "FILTER " << (i + 1) << " 🞋 🞋 🞋 🞋 🞋" << endl;
      oss << el.as_readable ();
    }

  return oss.str ();
}

template <typename V, typename F>
requires CanFilterRecogValue<F, V> bool
FilterGroup<V, F>::rm_filter (const u32 &which_filter) noexcept
{
  // std::reverse (filters.begin (), filters.end ());

  {
    u32 idx = which_filter - 1;
    if (idx < 0 || filters.size () == 1 || idx > filters.size () - 1)
      {
	goto bad_ret;
      }
    size_t oldsz = filters.size ();
    filters.erase (filters.begin () + idx);
    size_t newsz = filters.size ();
    if (newsz == oldsz - 1)
      goto good_ret;
  }

good_ret:
  // std::reverse (filters.begin (), filters.end ());
  return true;

bad_ret:
  // std::reverse (filters.begin (), filters.end ());
  return false;
}

template <typename V, typename F>
requires CanFilterRecogValue<F, V> bool
FilterGroup<V, F>::add_one (u32 &which_filter, const FilterProperty p,
			    const string &value)
{
  // std::reverse (filters.begin (), filters.end ());

  if (which_filter < 1)
    goto bad_rtn;

  {
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
      goto bad_rtn;
    switch (p)
      {
      case FilterPropertyTitles:
	filters[idx].__titles.emplace_back (move (may_re));
	break;
      case FilterPropertyNoTitles:
	filters[idx].__no_titles.emplace_back (move (may_re));
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
      }
  }

  // std::reverse (filters.begin (), filters.end ());
  return true;

bad_rtn:
  // std::reverse (filters.begin (), filters.end ());
  return true;
}

template <typename V, typename F>
requires CanFilterRecogValue<F, V> bool
FilterGroup<V, F>::del_one (u32 &which_filter, const FilterProperty p,
			    const string &value)
{
  // std::reverse (filters.begin (), filters.end ());

  if (which_filter < 1)
    goto bad_rtn;

  {
    u32 idx = which_filter - 1;
    if (idx > filters.size () - 1)
      {
	idx = filters.size () - 1;
	which_filter = filters.size ();
      }
    PosixExtRegex may_re (value);
    if (!may_re.is_pattern_accept ())
      goto bad_rtn;

    vector<PosixExtRegex> &list_titles = filters[idx].__titles;
    vector<PosixExtRegex> &list_no_titles = filters[idx].__no_titles;
    vector<PosixExtRegex> &list_keywords = filters[idx].keywords;
    vector<PosixExtRegex> &list_no_keywords = filters[idx].no_keywords;
    vector<PosixExtRegex> &list_senders = filters[idx].senders;
    vector<PosixExtRegex> &list_no_senders = filters[idx].no_senders;
    using it_type = vector<PosixExtRegex>::iterator;
    it_type begit = list_keywords.begin ();
    it_type endit = list_keywords.end ();
    switch (p)
      {
      case FilterPropertyTitles:
	begit = list_titles.begin ();
	endit = list_titles.end ();
	for (it_type it = begit; it != endit; it++)
	  if (it->ptn () == value)
	    {
	      it = list_titles.erase (it);
	      break;
	    }
	break;
      case FilterPropertyNoTitles:
	begit = list_no_titles.begin ();
	endit = list_no_titles.end ();
	for (it_type it = begit; it != endit; it++)
	  if (it->ptn () == value)
	    {
	      it = list_no_titles.erase (it);
	      break;
	    }
	break;
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
      }
  }

  // std::reverse (filters.begin (), filters.end ());
  return true;

bad_rtn:
  // std::reverse (filters.begin (), filters.end ());
  return false;
}

template <typename V, typename F>
requires CanFilterRecogValue<F, V> bool
FilterGroup<V, F>::add_titles (u32 &which_filter, const string &value)
{
  return add_one (which_filter, FilterPropertyTitles, value);
}

template <typename V, typename F>
requires CanFilterRecogValue<F, V> bool
FilterGroup<V, F>::del_titles (u32 &which_filter, const string &value)
{
  return del_one (which_filter, FilterPropertyTitles, value);
}

template <typename V, typename F>
requires CanFilterRecogValue<F, V> bool
FilterGroup<V, F>::add_no_titles (u32 &which_filter, const string &value)
{
  return add_one (which_filter, FilterPropertyNoTitles, value);
}

template <typename V, typename F>
requires CanFilterRecogValue<F, V> bool
FilterGroup<V, F>::del_no_titles (u32 &which_filter, const string &value)
{
  return del_one (which_filter, FilterPropertyNoTitles, value);
}

template <typename V, typename F>
requires CanFilterRecogValue<F, V> bool
FilterGroup<V, F>::add_senders (u32 &which_filter, const string &value)
{
  return add_one (which_filter, FilterPropertySenders, value);
}

template <typename V, typename F>
requires CanFilterRecogValue<F, V> bool
FilterGroup<V, F>::del_senders (u32 &which_filter, const string &value)
{
  return del_one (which_filter, FilterPropertySenders, value);
}

template <typename V, typename F>
requires CanFilterRecogValue<F, V> bool
FilterGroup<V, F>::add_no_senders (u32 &which_filter, const string &value)
{
  return add_one (which_filter, FilterPropertyNoSenders, value);
}

template <typename V, typename F>
requires CanFilterRecogValue<F, V> bool
FilterGroup<V, F>::del_no_senders (u32 &which_filter, const string &value)
{
  return del_one (which_filter, FilterPropertyNoSenders, value);
}

template <typename V, typename F>
requires CanFilterRecogValue<F, V> bool
FilterGroup<V, F>::add_keywords (u32 &which_filter, const string &value)
{
  return add_one (which_filter, FilterPropertyKeywords, value);
}

template <typename V, typename F>
requires CanFilterRecogValue<F, V> bool
FilterGroup<V, F>::del_keywords (u32 &which_filter, const string &value)
{
  return del_one (which_filter, FilterPropertyKeywords, value);
}

template <typename V, typename F>
requires CanFilterRecogValue<F, V> bool
FilterGroup<V, F>::add_no_keywords (u32 &which_filter, const string &value)
{
  return add_one (which_filter, FilterPropertyNoKeywords, value);
}

template <typename V, typename F>
requires CanFilterRecogValue<F, V> bool
FilterGroup<V, F>::del_no_keywords (u32 &which_filter, const string &value)
{
  return del_one (which_filter, FilterPropertyNoKeywords, value);
}

} // namespace tgf

#endif
