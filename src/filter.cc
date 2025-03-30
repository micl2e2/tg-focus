#define __TU__ "filter"

#include "filter.hh"
#include "posix_regex.hh"

// ------------------------- FilterToml -------------------------

tgf::FilterToml::FilterToml (const toml::value &v)
{
  using std::string, std::vector;

  // title
  vector<string> title_list
    = toml::find_or<vector<string>> (v, "titles", vector<string> (0));
  for (string &strval : title_list)
    this->__titles.emplace_back (PosixExtRegex (strval));

  // no title
  vector<string> no_title_list
    = toml::find_or<vector<string>> (v, "no-titles", vector<string> (0));
  for (string &strval : no_title_list)
    this->__no_titles.emplace_back (PosixExtRegex (strval));

  // sender
  vector<string> sender_list
    = toml::find_or<vector<string>> (v, "senders", vector<string> (0));
  for (string &strval : sender_list)
    this->__senders.emplace_back (PosixExtRegex (strval));

  // no sender
  vector<string> no_sender_list
    = toml::find_or<vector<string>> (v, "no-senders", vector<string> (0));
  for (string &strval : no_sender_list)
    this->__no_senders.emplace_back (PosixExtRegex (strval));

  // keyword
  vector<string> keyword_list
    = toml::find_or<vector<string>> (v, "keywords", vector<string> (0));
  for (string &strval : keyword_list)
    this->__keywords.emplace_back (PosixExtRegex (strval));
  // no keyword
  vector<string> no_keyword_list
    = toml::find_or<vector<string>> (v, "no-keywords", vector<string> (0));
  for (string &strval : no_keyword_list)
    {
      this->__no_keywords.emplace_back (PosixExtRegex (strval));
    }
}

tgf::FilterGroupToml::FilterGroupToml (const string &v) noexcept
{
  std::vector<FilterToml> fs;
  try
    {
      auto iss = std::istringstream (v);
      auto is = std::istream (iss.rdbuf ());
      auto tomldata = toml::parse (is);
      fs = toml::find<std::vector<FilterToml>> (tomldata, "focus-filter");
    }
  catch (std::exception &ex)
    {
      using namespace toml::literals::toml_literals;
      fs = toml::find<std::vector<FilterToml>> ("[[focus-filter]]"_toml,
						"focus-filter");
    }
  this->__filters = std::move (fs);
}

string
escape (const string &self)
{
  ostringstream news;
  for (int i = 0; i < self.length (); i++)
    {
      if (self[i] == 0x5c)
	{
	  news << R"(\\)";
	}
      else
	news << self[i];
    }
  return news.str ();
}

// template <typename V, typename F>
//   requires CanFilterRecogValue<F, V>
// string
// FilterGroup<V, F>::as_fsdata () const noexcept
string
tgf::FilterGroupToml::as_fsdata () noexcept
{
  ostringstream oss;
  for (int i = 0; i < __filters.size (); i++)
    {
      const tgf::FilterToml &el = __filters[i];
      oss << endl << "[[focus-filter]]" << endl;

      oss << "titles = [";
      for (int ii = 0; ii < el.__titles.size (); ii++)
	{
	  const PosixExtRegex &elel = el.__titles[ii];
	  string s = escape (elel.ptn ());
	  oss << "\"" << s << "\"";
	  if (ii < el.__titles.size () - 1)
	    oss << ", ";
	}
      oss << "]" << endl;

      oss << "no-titles = [";
      for (int ii = 0; ii < el.__no_titles.size (); ii++)
	{
	  const PosixExtRegex &elel = el.__no_titles[ii];
	  string s = escape (elel.ptn ());
	  oss << "\"" << s << "\"";
	  if (ii < el.__no_titles.size () - 1)
	    oss << ", ";
	}
      oss << "]" << endl;

      oss << "senders = [";
      for (int ii = 0; ii < el.__senders.size (); ii++)
	{
	  const PosixExtRegex &elel = el.__senders[ii];
	  string s = escape (elel.ptn ());
	  oss << "\"" << s << "\"";
	  if (ii < el.__senders.size () - 1)
	    oss << ", ";
	}
      oss << "]" << endl;

      oss << "no-senders = [";
      for (int ii = 0; ii < el.__no_senders.size (); ii++)
	{
	  const PosixExtRegex &elel = el.__no_senders[ii];
	  string s = escape (elel.ptn ());
	  oss << "\"" << s << "\"";
	  if (ii < el.__no_senders.size () - 1)
	    oss << ", ";
	}
      oss << "]" << endl;

      oss << "keywords = [";
      for (int ii = 0; ii < el.__keywords.size (); ii++)
	{
	  const PosixExtRegex &elel = el.__keywords[ii];
	  string s = escape (elel.ptn ());
	  oss << "\"" << s << "\"";
	  if (ii < el.__keywords.size () - 1)
	    oss << ", ";
	}
      oss << "]" << endl;

      oss << "no-keywords = [";
      for (int ii = 0; ii < el.__no_keywords.size (); ii++)
	{
	  const PosixExtRegex &elel = el.__no_keywords[ii];
	  string s = escape (elel.ptn ());
	  oss << "\"" << s << "\"";
	  if (ii < el.__no_keywords.size () - 1)
	    oss << ", ";
	}
      oss << "]" << endl;
    }

  return oss.str ();
}
