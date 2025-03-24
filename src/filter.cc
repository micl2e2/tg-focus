#define __TU__ "filter"

#include "filter.hh"
#include "posix_regex.hh"

// ------------------------- FilterToml -------------------------

tgf::FilterToml::FilterToml (const toml::value &v)
{
  using std::string, std::vector;

  // title
  std::string sval = toml::find_or<string> (v, "title", ".*");
  this->chat_title = PosixExtRegex (sval);

  // sender
  vector<string> sender_list
    = toml::find_or<vector<string>> (v, "senders", vector<string> (0));
  for (string &strval : sender_list)
    this->senders.emplace_back (PosixExtRegex (strval));

  // no sender
  vector<string> no_sender_list
    = toml::find_or<vector<string>> (v, "no-senders", vector<string> (0));
  for (string &strval : no_sender_list)
    this->no_senders.emplace_back (PosixExtRegex (strval));

  // rej sender
  vector<string> rej_sender_list
    = toml::find_or<vector<string>> (v, "rej-senders", vector<string> (0));
  for (string &strval : rej_sender_list)
    this->rej_senders.emplace_back (PosixExtRegex (strval));

  // keyword
  vector<string> keyword_list
    = toml::find_or<vector<string>> (v, "keywords", vector<string> (0));
  for (string &strval : keyword_list)
    this->keywords.emplace_back (PosixExtRegex (strval));
  // no keyword
  vector<string> no_keyword_list
    = toml::find_or<vector<string>> (v, "no-keywords", vector<string> (0));
  for (string &strval : no_keyword_list)
    {
      this->no_keywords.emplace_back (PosixExtRegex (strval));
    }
}

tgf::FilterGroupToml::FilterGroupToml (const toml::value &v) noexcept
{
  std::vector<FilterToml> fs;
  try
    {
      fs = toml::find<std::vector<FilterToml>> (v, "focus-filter");
    }
  catch (std::exception &ex)
    {
      using namespace toml::literals::toml_literals;
      fs = toml::find<std::vector<FilterToml>> ("[[focus-filter]]"_toml,
						"focus-filter");
    }
  this->filters = std::move (fs);
}

tgf::FilterGroupToml::FilterGroupToml (std::string &v) noexcept
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
  this->filters = std::move (fs);
}

// template <typename V, typename F>
//   requires CanFilterRecogValue<F, V>
// string
// FilterGroup<V, F>::as_fsdata () const noexcept
string
tgf::FilterGroupToml::as_fsdata () noexcept
{
  ostringstream oss;
  for (int i = 0; i < filters.size (); i++)
    {
      const tgf::FilterToml &el = filters[i];
      oss << endl << "[[focus-filter]]" << endl;

      oss << "title = " << "\"" << el.chat_title.ptn () << "\"" << endl;

      oss << "keywords = [";
      for (int ii = 0; ii < el.keywords.size (); ii++)
	{
	  const PosixExtRegex &elel = el.keywords[ii];
	  oss << "\"" << elel.ptn () << "\"";
	  if (ii < el.keywords.size () - 1)
	    oss << ", ";
	}
      oss << "]" << endl;

      oss << "no-keywords = [";
      for (int ii = 0; ii < el.no_keywords.size (); ii++)
	{
	  const PosixExtRegex &elel = el.no_keywords[ii];
	  oss << "\"" << elel.ptn () << "\"";
	  if (ii < el.no_keywords.size () - 1)
	    oss << ", ";
	}
      oss << "]" << endl;

      oss << "senders = [";
      for (int ii = 0; ii < el.senders.size (); ii++)
	{
	  const PosixExtRegex &elel = el.senders[ii];
	  oss << "\"" << elel.ptn () << "\"";
	  if (ii < el.senders.size () - 1)
	    oss << ", ";
	}
      oss << "]" << endl;

      oss << "no-senders = [";
      for (int ii = 0; ii < el.no_senders.size (); ii++)
	{
	  const PosixExtRegex &elel = el.no_senders[ii];
	  oss << "\"" << elel.ptn () << "\"";
	  if (ii < el.no_senders.size () - 1)
	    oss << ", ";
	}
      oss << "]" << endl;

      oss << "rej-senders = [";
      for (int ii = 0; ii < el.rej_senders.size (); ii++)
	{
	  const PosixExtRegex &elel = el.rej_senders[ii];
	  oss << "\"" << elel.ptn () << "\"";
	  if (ii < el.rej_senders.size () - 1)
	    oss << ", ";
	}
      oss << "]" << endl;
    }

  return oss.str ();
}
