#include "toml.hpp"
#include "posix_regex.hh"
#include "focus_filter.hh"
#include <cstdint>
#include <ctime>
#include <exception>

// FocusFilterList

FocusFilterList::FocusFilterList (const toml::value &v)
{
  std::vector<FocusFilter> filters;

  try
    {
      filters = toml::find<std::vector<FocusFilter>> (v, "focus-filter");
    }
  catch (std::exception &ex)
    {
      using namespace toml::literals::toml_literals;
      filters = toml::find<std::vector<FocusFilter>> ("[[focus-filter]]"_toml,
						      "focus-filter");
    }

  this->filters = std::move (filters);
}

FocusFilterList::FocusFilterList (const char *v)
{
  std::vector<FocusFilter> filters;

  try
    {
      auto iss = std::istringstream (v);
      auto is = std::istream (iss.rdbuf ());
      auto tomldata = toml::parse (is);

      filters = toml::find<std::vector<FocusFilter>> (tomldata, "focus-filter");
    }
  catch (std::exception &ex)
    {
      using namespace toml::literals::toml_literals;
      filters = toml::find<std::vector<FocusFilter>> ("[[focus-filter]]"_toml,
						      "focus-filter");
    }

  this->filters = std::move (filters);
}

FocusFilterList::FocusFilterList (const std::string &v)
{
  std::vector<FocusFilter> filters;

  try
    {
      auto iss = std::istringstream (v);
      auto is = std::istream (iss.rdbuf ());
      auto tomldata = toml::parse (is);

      filters = toml::find<std::vector<FocusFilter>> (tomldata, "focus-filter");
    }
  catch (std::exception &ex)
    {
      std::cerr << "focusfilter from std::string error: " << ex.what ()
		<< std::endl;
      using namespace toml::literals::toml_literals;
      filters = toml::find<std::vector<FocusFilter>> ("[[focus-filter]]"_toml,
						      "focus-filter");
    }

  this->filters = std::move (filters);
}

bool
FocusFilterList::is_valid (const std::string &v)
{
  std::vector<FocusFilter> filters;

  try
    {
      auto iss = std::istringstream (v);
      auto is = std::istream (iss.rdbuf ());
      auto tomldata = toml::parse (is);

      filters = toml::find<std::vector<FocusFilter>> (tomldata, "focus-filter");
    }
  catch (std::exception &ex)
    {
      return false;
    }

  return true;
}

bool
FocusFilterList::is_tgmsg_match (const TgMsg &in)
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

// FocusFilter

FocusFilter::FocusFilter (FocusFilter &&move_ctor_from)
{
  this->chat_title = std::move (move_ctor_from.chat_title);
  this->senders = std::move (move_ctor_from.senders);
  this->no_senders = std::move (move_ctor_from.no_senders);
  this->rej_senders = std::move (move_ctor_from.rej_senders);
  this->keywords = std::move (move_ctor_from.keywords);
  this->no_keywords = std::move (move_ctor_from.no_keywords);
}

FocusFilter &
FocusFilter::operator= (FocusFilter &&move_assign_from)
{
  this->chat_title = std::move (move_assign_from.chat_title);
  this->senders = std::move (move_assign_from.senders);
  this->no_senders = std::move (move_assign_from.no_senders);
  this->keywords = std::move (move_assign_from.keywords);
  this->no_keywords = std::move (move_assign_from.no_keywords);
  return *this;
}

FocusFilter::FocusFilter (const toml::value &v)
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

bool
FocusFilter::is_title_match (const std::string &input)
{
  if (auto flag = this->chat_title.is_match (input))
    {
      if (*flag)
	return true;
    }

  return false;
}

bool
FocusFilter::is_keyword_match (const std::string &input)
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

bool
FocusFilter::is_no_keyword_match (const std::string &input)
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

bool
FocusFilter::is_sender_match (const std::string &input)
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

bool
FocusFilter::is_no_sender_match (const std::string &input)
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

bool
FocusFilter::is_rej_sender_match (const std::string &input)
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

FocusDecision
FocusFilter::is_tgmsg_match (const TgMsg &in)
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
