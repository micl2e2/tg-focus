#include "TgfFilter.hpp"

// ------------------------- TgfFilterToml -------------------------

TgfFilterToml::TgfFilterToml (const toml::value &v)
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

TgfFilterGroupToml::TgfFilterGroupToml (const toml::value &v) noexcept
{
  std::vector<TgfFilterToml> fs;
  try
    {
      fs = toml::find<std::vector<TgfFilterToml>> (v, "focus-filter");
    }
  catch (std::exception &ex)
    {
      using namespace toml::literals::toml_literals;
      fs = toml::find<std::vector<TgfFilterToml>> ("[[focus-filter]]"_toml,
						   "focus-filter");
    }
  this->filters = std::move (fs);
}

TgfFilterGroupToml::TgfFilterGroupToml (std::string &v) noexcept
{
  std::vector<TgfFilterToml> fs;
  try
    {
      auto iss = std::istringstream (v);
      auto is = std::istream (iss.rdbuf ());
      auto tomldata = toml::parse (is);
      fs = toml::find<std::vector<TgfFilterToml>> (tomldata, "focus-filter");
    }
  catch (std::exception &ex)
    {
      using namespace toml::literals::toml_literals;
      fs = toml::find<std::vector<TgfFilterToml>> ("[[focus-filter]]"_toml,
						   "focus-filter");
    }
  this->filters = std::move (fs);
}
