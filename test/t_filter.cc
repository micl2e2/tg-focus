#include <assert.h>

#include "toml.hpp"
#include "std_comp.hh"
#include "filter.hh"
#include "toml/from.hpp"
#include "toml/parser.hpp"
#include "toml/string.hpp"

void
test_default_filter ()
{ // match everything
  tgf::FilterToml f;
  tgfass (f.mtch_titles (""s));
  tgfass (f.mtch_keywords (""s));
  tgfass (f.mtch_senders (""s));
  tgfass (!f.mtch_no_titles (""s));
  tgfass (!f.mtch_no_senders (""s));
  tgfass (!f.mtch_no_keywords (""s));
}

void
test_default_fg ()
{ // match everthing
  using namespace toml::literals::toml_literals;
  using namespace std;

  auto dataxxx = R"(
[[focus-filter]]
)"_toml;

  vector<tgf::FilterToml> filters
    = toml::find<vector<tgf::FilterToml>> (dataxxx, "focus-filter");

  tgfass (filters.size () == 1);

  auto fcf_list = tgf::FilterGroupToml (dataxxx);
  tgfass (fcf_list.n_filter () == 1);

  {
    tgf::TgMsg msg (""s, ""s, ""s);
    tgfass (fcf_list.mtch_tgmsg (msg));
  }

  {
    tgf::TgMsg msg ("title_aaa"s, "sd_bbb"s, "kw_bbb"s);
    tgfass (fcf_list.mtch_tgmsg (msg));
  }
}

void
test_sub_match1 ()
{
  using namespace toml::literals::toml_literals;
  using namespace std;

  auto dataxxx = R"(
# this line will be ignored by parser

[[focus-filter]]
titles = ["bcd"]

[[focus-filter]]
titles = ["xxx"]
senders = ["usr1","usr2"]

[[focus-filter]]
titles = ["xxx"]
no-senders = ["usr3","usr4",]

[[focus-filter]]
titles = ["A{2,}B{2,}"]
keywords = ["kw1", "kw2", ]

[[focus-filter]]
keywords = [ "kw3", "kw4", "(😺){3}" ]

[[focus-filter]]
titles = ["xxx"]
keywords = ["kw5"]
no-keywords = ["kw6","kw7"]

)"_toml;

  vector<tgf::FilterToml> filters
    = toml::find<vector<tgf::FilterToml>> (dataxxx, "focus-filter");

  tgfass (filters.size () == 6);

  // [0]
  tgfass (filters[0].mtch_titles ("abcde"));
  tgfass (!filters[0].mtch_titles ("cd"));
  // if absent, then match anything
  tgfass (filters[0].mtch_keywords (""));
  // if absent, then not match anything
  tgfass (!filters[0].mtch_no_keywords (""));
  tgfass (filters[0].mtch_senders (""));
  tgfass (!filters[0].mtch_no_senders (""));

  // [1]
  tgfass (filters[1].mtch_senders ("usr1"));
  tgfass (filters[1].mtch_senders ("usr2"));
  tgfass (!filters[1].mtch_senders ("usr"));

  // [2]
  tgfass (filters[2].mtch_no_senders ("usr3"));
  tgfass (filters[2].mtch_no_senders ("usr4"));
  tgfass (!filters[2].mtch_no_senders ("usr"));

  // [3]
  tgfass (filters[3].mtch_titles ("AABB"));
  tgfass (filters[3].mtch_titles ("AAABBB"));
  tgfass (!filters[3].mtch_titles ("AB"));
  tgfass (filters[3].mtch_keywords ("kw1"));
  tgfass (filters[3].mtch_keywords ("kw2"));
  tgfass (!filters[3].mtch_keywords ("kw3"));

  // [4]
  // if title absent, match anything
  tgfass (filters[4].mtch_titles (""));
  tgfass (filters[4].mtch_titles (" "));
  tgfass (filters[4].mtch_titles ("123abcABC😺"));
  tgfass (filters[4].mtch_keywords ("kw3"));
  tgfass (filters[4].mtch_keywords ("kw4"));
  tgfass (!filters[4].mtch_keywords ("kw1"));
  tgfass (!filters[4].mtch_keywords ("😺"));
  tgfass (!filters[4].mtch_keywords ("😺😺"));
  tgfass (filters[4].mtch_keywords ("😺😺😺"));
  tgfass (filters[4].mtch_keywords ("😺😺😺😺"));

  // [5]
  tgfass (filters[5].mtch_keywords ("kw5"));
  tgfass (filters[5].mtch_no_keywords ("kw6"));
  tgfass (filters[5].mtch_no_keywords ("kw7"));
  tgfass (!filters[5].mtch_no_keywords ("kw8"));
}

//
// same as 1, except input is cstr
void
test_sub_match2 ()
{
  using namespace toml::literals::toml_literals;
  using namespace std;

  const char *tomlcstr = R"(
[[focus-filter]]
titles = ["bcd"]

[[focus-filter]]
titles = ["xxx"]
senders = ["usr1","usr2"]

[[focus-filter]]
titles = ["xxx"]
no-senders = ["usr3","usr4",]

[[focus-filter]]
titles = ["A{2,}B{2,}"]
keywords = ["kw1", "kw2", ]

[[focus-filter]]
keywords = [ "kw3", "kw4", "(😺){3}" ]

[[focus-filter]]
titles = ["xxx"]
keywords = ["kw5"]
no-keywords = ["kw6","kw7"]

)";

  auto iss = std::istringstream (
    tomlcstr); // must exist before 'is', since rdbuf() calling is non-sense if
	       // this is temporary variable
  auto is = std::istream (iss.rdbuf ());
  auto tomldata = toml::parse (is);

  vector<tgf::FilterToml> filters
    = toml::find<vector<tgf::FilterToml>> (tomldata, "focus-filter");

  tgfass (filters.size () == 6);

  // [0]
  tgfass (filters[0].mtch_titles ("abcde"));
  tgfass (!filters[0].mtch_titles ("cd"));
  // if absent, then match anything
  tgfass (filters[0].mtch_keywords (""));
  // if absent, then not match anything
  tgfass (!filters[0].mtch_no_keywords (""));
  tgfass (filters[0].mtch_senders (""));
  tgfass (!filters[0].mtch_no_senders (""));

  // [1]
  tgfass (filters[1].mtch_senders ("usr1"));
  tgfass (filters[1].mtch_senders ("usr2"));
  tgfass (!filters[1].mtch_senders ("usr"));

  // [2]
  tgfass (filters[2].mtch_no_senders ("usr3"));
  tgfass (filters[2].mtch_no_senders ("usr4"));
  tgfass (!filters[2].mtch_no_senders ("usr"));

  // [3]
  tgfass (filters[3].mtch_titles ("AABB"));
  tgfass (filters[3].mtch_titles ("AAABBB"));
  tgfass (!filters[3].mtch_titles ("AB"));
  tgfass (filters[3].mtch_keywords ("kw1"));
  tgfass (filters[3].mtch_keywords ("kw2"));
  tgfass (!filters[3].mtch_keywords ("kw3"));

  // [4]
  // if title absent, match anything
  tgfass (filters[4].mtch_titles (""));
  tgfass (filters[4].mtch_titles (" "));
  tgfass (filters[4].mtch_titles ("123abcABC😺"));
  tgfass (filters[4].mtch_keywords ("kw3"));
  tgfass (filters[4].mtch_keywords ("kw4"));
  tgfass (!filters[4].mtch_keywords ("kw1"));
  tgfass (!filters[4].mtch_keywords ("😺"));
  tgfass (!filters[4].mtch_keywords ("😺😺"));
  tgfass (filters[4].mtch_keywords ("😺😺😺"));
  tgfass (filters[4].mtch_keywords ("😺😺😺😺"));

  // [5]
  tgfass (filters[5].mtch_keywords ("kw5"));
  tgfass (filters[5].mtch_no_keywords ("kw6"));
  tgfass (filters[5].mtch_no_keywords ("kw7"));
  tgfass (!filters[5].mtch_no_keywords ("kw8"));
}

void
test_focus_filter_list1 ()
{
  using namespace toml::literals::toml_literals;
  using namespace std;

  tgf::TgMsg msg ("chatxxx"s, "senderxxx"s, "txtxxx"s);

  { // title matched
    auto tomldata = R"(

)"_toml;
    auto fcf_list = tgf::FilterGroupToml (tomldata);
    tgfass (fcf_list.n_filter () == 1);
    tgfass (fcf_list.mtch_tgmsg (msg));

    // note: an implicit title=".*" inserted
  }

  { // title matched
    auto tomldata = R"(
[[focus-filter]]
)"_toml;
    auto fcf_list = tgf::FilterGroupToml (tomldata);
    tgfass (fcf_list.n_filter () == 1);
    tgfass (fcf_list.mtch_tgmsg (msg));
  }

  { // title matched
    auto tomldata = R"(
[[focus-filter]]
titles = ["xxx"]
)"_toml;
    auto fcf_list = tgf::FilterGroupToml (tomldata);
    tgfass (fcf_list.n_filter () == 1);
    tgfass (fcf_list.mtch_tgmsg (msg));
  }

  { // senders matched
    auto tomldata = R"(
[[focus-filter]]
senders = ["xxx"]
)"_toml;
    auto fcf_list = tgf::FilterGroupToml (tomldata);
    tgfass (fcf_list.n_filter () == 1);
    tgfass (fcf_list.mtch_tgmsg (msg));
  }

  { // keywords matched
    auto tomldata = R"(
[[focus-filter]]
keywords = ["xxx"]
)"_toml;
    auto fcf_list = tgf::FilterGroupToml (tomldata);
    tgfass (fcf_list.n_filter () == 1);
    tgfass (fcf_list.mtch_tgmsg (msg));
  }

  { // no-keywords matched
    auto tomldata = R"(
[[focus-filter]]
no-keywords = ["xxx"]
)"_toml;
    auto fcf_list = tgf::FilterGroupToml (tomldata);
    tgfass (fcf_list.n_filter () == 1);
    tgfass (!fcf_list.mtch_tgmsg (msg));
  }

  { // no-keywords not matched
    auto tomldata = R"(
[[focus-filter]]
no-keywords = ["yyy"]
)"_toml;
    auto fcf_list = tgf::FilterGroupToml (tomldata);
    tgfass (fcf_list.n_filter () == 1);
    tgfass (fcf_list.mtch_tgmsg (msg));
  }

  { // no-senders not matched
    auto tomldata = R"(
[[focus-filter]]
no-senders = ["yyy"]
)"_toml;
    auto fcf_list = tgf::FilterGroupToml (tomldata);
    tgfass (fcf_list.n_filter () == 1);
    tgfass (fcf_list.mtch_tgmsg (msg));
  }

  { // senders vs. no-senders
    auto tomldata = R"(
[[focus-filter]]
senders = ["x{2}"]
no-senders = ["xxx"]
)"_toml;
    auto fcf_list = tgf::FilterGroupToml (tomldata);
    tgfass (fcf_list.n_filter () == 1);
    tgfass (!fcf_list.mtch_tgmsg (msg));
  }

  { // senders and keywords matched
    auto tomldata = R"(
[[focus-filter]]
senders = ["xxx"]
keywords = ["xxx"]
)"_toml;
    auto fcf_list = tgf::FilterGroupToml (tomldata);
    tgfass (fcf_list.n_filter () == 1);
    tgfass (fcf_list.mtch_tgmsg (msg));
  }

  { // senders matched, keywords not
    auto tomldata = R"(
[[focus-filter]]
senders = ["xxx"]
keywords = ["yyy"]
)"_toml;
    auto fcf_list = tgf::FilterGroupToml (tomldata);
    tgfass (fcf_list.n_filter () == 1);
    tgfass (!fcf_list.mtch_tgmsg (msg));
  }

  { // senders not, keywords matched
    auto tomldata = R"(
[[focus-filter]]
senders = ["yyy"]
keywords = ["xxx"]
)"_toml;
    auto fcf_list = tgf::FilterGroupToml (tomldata);
    tgfass (fcf_list.n_filter () == 1);
    tgfass (!fcf_list.mtch_tgmsg (msg));
  }

  { // all last matched
    auto tomldata = R"(
[[focus-filter]]
senders = ["a","b","c","d","x+"]
keywords = ["a","b","c","d","x+"]
)"_toml;
    auto fcf_list = tgf::FilterGroupToml (tomldata);
    tgfass (fcf_list.n_filter () == 1);
    tgfass (fcf_list.mtch_tgmsg (msg));
  }

  { // 2nd filter matched
    auto tomldata = R"(
[[focus-filter]]
senders = ["yyy"]

[[focus-filter]]
keywords = ["x{3}"]
)"_toml;
    auto fcf_list = tgf::FilterGroupToml (tomldata);
    tgfass (fcf_list.n_filter () == 2);
    tgfass (fcf_list.mtch_tgmsg (msg));
    tgfass (fcf_list.i_prev_visited () == 1);
  }

  { // 1st filter matched
    auto tomldata = R"(
[[focus-filter]]
senders = ["x{2}"]

[[focus-filter]]
keywords = ["x{3}"]
)"_toml;
    auto fcf_list = tgf::FilterGroupToml (tomldata);
    tgfass (fcf_list.n_filter () == 2);
    tgfass (fcf_list.mtch_tgmsg (msg));
    tgfass (fcf_list.i_prev_visited () == 0);
  }

  { // more complicated
    auto tomldata = R"(
[[focus-filter]]
senders = ["x{2}"]
no-senders = ["xxx"]

[[focus-filter]]
)"_toml;
    auto fcf_list = tgf::FilterGroupToml (tomldata);
    tgfass (fcf_list.n_filter () == 2);
    tgfass (!fcf_list.mtch_tgmsg (msg));
    tgfass (fcf_list.i_prev_visited () == 0);
  }

  { // more complicated
    auto tomldata = R"(
[[focus-filter]]
senders = ["x{2}"]
no-senders = ["xxx"]

[[focus-filter]]
keywords = ["x*"]
no-keywords = ["xx"]

[[focus-filter]]
keywords = "[a-z]+"
)"_toml;
    auto fcf_list = tgf::FilterGroupToml (tomldata);
    tgfass (fcf_list.n_filter () == 3);
    tgfass (!fcf_list.mtch_tgmsg (msg));
    tgfass (fcf_list.i_prev_visited () == 0);
  }

  { // more complicated
    auto tomldata = R"(
[[focus-filter]]
senders = ["yyy"]

[[focus-filter]]
keywords = ["xxx"]

[[focus-filter]]
no-keywords = ["[a-z]+"]

[[focus-filter]]
keywords = "[:alpha:]{3}"
)"_toml;
    auto fcf_list = tgf::FilterGroupToml (tomldata);
    tgfass (fcf_list.n_filter () == 4);
    tgfass (fcf_list.mtch_tgmsg (msg));
    tgfass (fcf_list.i_prev_visited () == 1);
  }

  { // more complicated
    auto tomldata = R"(
[[focus-filter]]
senders = ["zzz"]

[[focus-filter]]
no-keywords = ["xxx"]

[[focus-filter]]
no-keywords = ["[a-z]+"] 

[[focus-filter]]
keywords = "[:alpha:]{3}"
)"_toml;
    auto fcf_list = tgf::FilterGroupToml (tomldata);
    tgfass (fcf_list.n_filter () == 4);
    tgfass (!fcf_list.mtch_tgmsg (msg));
    tgfass (fcf_list.i_prev_visited () == 1);
  }
}

void
test_focus_filter_list2 ()
{
  using namespace toml::literals::toml_literals;
  using namespace std;

  tgf::TgMsg msg ("-"s, "😺🐶😺🐶"s, "😺😺😺🐶🐶🐶"s);

  { // more complicated
    auto tomldata = R"(
[[focus-filter]]
senders = ["😺🐶"]
)"_toml;
    auto fcf_list = tgf::FilterGroupToml (tomldata);
    tgfass (fcf_list.n_filter () == 1);
    tgfass (fcf_list.mtch_tgmsg (msg));
    tgfass (fcf_list.i_prev_visited () == 0);
  }

  { // more complicated
    auto tomldata = R"(
[[focus-filter]]
no-senders = ["🐶"]

[[focus-filter]]
no-keywords = ["🐶"]

[[focus-filter]]
no-keywords = ["(🐶){2}"] 

[[focus-filter]]
no-keywords = ["(🐶){3}"] 

[[focus-filter]]
no-keywords = ["(🐶){2}(🐶){2}"]   # <--
)"_toml;
    auto fcf_list = tgf::FilterGroupToml (tomldata);
    tgfass (fcf_list.n_filter () == 5);
    tgfass (!fcf_list.mtch_tgmsg (msg));
    tgfass (fcf_list.i_prev_visited () == 0);
  }

  { // more complicated
    auto tomldata = R"(
[[focus-filter]]
no-senders = ["(😺🐶){2}"]

[[focus-filter]]
senders = ["😺"]  # <--

[[focus-filter]]
senders = ["🐶"]
)"_toml;
    auto fcf_list = tgf::FilterGroupToml (tomldata);
    tgfass (fcf_list.n_filter () == 3);
    tgfass (!fcf_list.mtch_tgmsg (msg));
    tgfass (fcf_list.i_prev_visited () == 0);
  }

  { // more complicated
    auto tomldata = R"(
[[focus-filter]]
senders = ["(😺🐶){3}"]

[[focus-filter]]
senders = ["😺"]
no-keywords = ["🐶"]

[[focus-filter]]
senders = ["🐶"]
no-keywords = ["😺"]

[[focus-filter]]
keywords = ["😺😺😺"]  # <-- 
)"_toml;
    auto fcf_list = tgf::FilterGroupToml (tomldata);
    tgfass (fcf_list.n_filter () == 4);
    tgfass (!fcf_list.mtch_tgmsg (msg));
    tgfass (fcf_list.i_prev_visited () == 1);
  }

  { // more complicated
    auto tomldata = R"(
[[focus-filter]]
no-senders = ["(😺🐶){3}"]

[[focus-filter]]
senders = ["😺"]
keywords = ["🐶"]

[[focus-filter]]
senders = ["🐶"]
no-keywords = ["😺"]

[[focus-filter]]
keywords = ["😺😺😺"]  # <-- 
)"_toml;
    auto fcf_list = tgf::FilterGroupToml (tomldata);
    tgfass (fcf_list.n_filter () == 4);
    tgfass (fcf_list.mtch_tgmsg (msg));
    tgfass (fcf_list.i_prev_visited () == 0);
    // note: [1] and [2] is skipped bc both filters' <no-keywords> reject the
    //       input, even though both filters's <senders> accept it.
  }
}

void
test_focus_filter_list3 ()
{
  using namespace toml::literals::toml_literals;
  using namespace std;

  tgf::TgMsg msg ("-"s, "xxx"s, "linux is good"s);

  { // toml literal
    auto tomldata = R"(
[[focus-filter]]
keywords = ["linux", "gnu", "fsf"]
no-keywords = ["macos"]
)"_toml;
    auto fcf_list = tgf::FilterGroupToml (tomldata);
    tgfass (fcf_list.n_filter () == 1);
    tgfass (fcf_list.mtch_tgmsg (msg));
    tgfass (fcf_list.i_prev_visited () == 0);
  }
}

void
test_focus_filter_list4 ()
{
  using namespace std;

  tgf::TgMsg msg ("-"s, "xxx"s, "linux is good"s);

  { // cstr
    auto tomldata = R"(
[[focus-filter]]
keywords = ["linux", "gnu", "fsf"]
no-keywords = ["macos"]
)";
    auto fcf_list = tgf::FilterGroupToml (tomldata);
    tgfass (fcf_list.n_filter () == 1);
    tgfass (fcf_list.mtch_tgmsg (msg));
    tgfass (fcf_list.i_prev_visited () == 0);
  }
}

//
// a filter list containing one filter, reject a message because of a
// FocusDecision::Skip
void
test_match_and_submatch1 ()
{
  using namespace toml::literals::toml_literals;
  using namespace std;

  auto dataxxx = R"(

[[focus-filter]]
no-senders = ["sd1","sd2"]
keywords = ["kw1","kw2"]

)"_toml;

  vector<tgf::FilterToml> filters
    = toml::find<vector<tgf::FilterToml>> (dataxxx, "focus-filter");

  tgfass (filters.size () == 1);

  // [0]
  tgfass (filters[0].mtch_no_senders ("sd1"));
  tgfass (filters[0].mtch_no_senders ("sd2"));
  tgfass (filters[0].mtch_keywords ("kw1"));
  tgfass (filters[0].mtch_keywords ("kw2"));

  auto fcf_list = tgf::FilterGroupToml (dataxxx);
  tgfass (fcf_list.n_filter () == 1);

  {
    tgf::TgMsg msg ("chatxxx"s, "sd1"s, "kw2"s);
    tgfass (!fcf_list.mtch_tgmsg (msg));
  }
}

//
// a filter list containing two filter2, accept a message because of a
// FocusDecision::Skip and the following FocusDecision::Focus
void
test_match_and_submatch2 ()
{
  using namespace toml::literals::toml_literals;
  using namespace std;

  auto dataxxx = R"(

[[focus-filter]]
no-senders = ["sd1","sd2"]
keywords = ["kw1","kw2"]

[[focus-filter]]
keywords = ["kw1","kw2"]

)"_toml;

  vector<tgf::FilterToml> filters
    = toml::find<vector<tgf::FilterToml>> (dataxxx, "focus-filter");

  tgfass (filters.size () == 2);

  // [0]
  tgfass (filters[0].mtch_no_senders ("sd1"));
  tgfass (filters[0].mtch_no_senders ("sd2"));
  tgfass (filters[0].mtch_keywords ("kw1"));
  tgfass (filters[0].mtch_keywords ("kw2"));

  auto fcf_list = tgf::FilterGroupToml (dataxxx);
  tgfass (fcf_list.n_filter () == 2);

  {
    tgf::TgMsg msg ("chatxxx"s, "sd1"s, "kw2"s);
    tgfass (!fcf_list.mtch_tgmsg (msg));
  }
}

//
// a filter list containing one filter, reject a message because of a
// FocusDecision::Reject
void
test_match_and_submatch1_ ()
{
  using namespace toml::literals::toml_literals;
  using namespace std;

  auto dataxxx = R"(

[[focus-filter]]
no-senders = ["sd1","sd2"]
keywords = ["kw1","kw2"]

)"_toml;

  vector<tgf::FilterToml> filters
    = toml::find<vector<tgf::FilterToml>> (dataxxx, "focus-filter");

  tgfass (filters.size () == 1);

  // [0]
  tgfass (filters[0].mtch_no_senders ("sd1"));
  tgfass (filters[0].mtch_no_senders ("sd2"));
  tgfass (filters[0].mtch_keywords ("kw1"));
  tgfass (filters[0].mtch_keywords ("kw2"));

  auto fcf_list = tgf::FilterGroupToml (dataxxx);
  tgfass (fcf_list.n_filter () == 1);

  {
    tgf::TgMsg msg ("chatxxx"s, "sd1"s, "kw2"s);
    tgfass (!fcf_list.mtch_tgmsg (msg));
  }
}

//
// a filter list containing two filter2, reject a message because of a
// FocusDecision::Reject
void
test_match_and_submatch2_ ()
{
  using namespace toml::literals::toml_literals;
  using namespace std;

  auto dataxxx = R"(

[[focus-filter]]
no-senders = ["sd1","sd2"]
keywords = ["kw1","kw2"]

[[focus-filter]]
keywords = ["kw1","kw2"]

)"_toml;

  vector<tgf::FilterToml> filters
    = toml::find<vector<tgf::FilterToml>> (dataxxx, "focus-filter");

  tgfass (filters.size () == 2);

  // [0]
  tgfass (filters[0].mtch_no_senders ("sd1"));
  tgfass (filters[0].mtch_no_senders ("sd2"));
  tgfass (filters[0].mtch_keywords ("kw1"));
  tgfass (filters[0].mtch_keywords ("kw2"));

  auto fcf_list = tgf::FilterGroupToml (dataxxx);
  tgfass (fcf_list.n_filter () == 2);

  {
    tgf::TgMsg msg ("chatxxx"s, "sd1"s, "kw2"s);
    tgfass (!fcf_list.mtch_tgmsg (msg));
  }
}

void
test_nokw_not_much_inituitive ()
{
  using namespace toml::literals::toml_literals;
  using namespace std;

  auto dataxxx = R"(

[[focus-filter]]
titles = ["aaa"]
no-keywords = [".*"]

[[focus-filter]]
titles = [".*"]

)"_toml;

  vector<tgf::FilterToml> filters
    = toml::find<vector<tgf::FilterToml>> (dataxxx, "focus-filter");

  tgfass (filters.size () == 2);

  auto fcf_list = tgf::FilterGroupToml (dataxxx);
  tgfass (fcf_list.n_filter () == 2);

  {
    tgf::TgMsg msg ("title_bbb"s, "sd_bbb"s, "kw_bbb"s);
    tgfass (fcf_list.mtch_tgmsg (msg));
    tgfass (fcf_list.i_prev_visited () == 1);
  }

  {
    tgf::TgMsg msg ("title_aaa"s, "sd_bbb"s, "kw_bbb"s);
    tgfass (!fcf_list.mtch_tgmsg (msg));
    tgfass (fcf_list.i_prev_visited () == 0);
  }
}

void
test_nokw_not_much_inituitive2 ()
{
  using namespace toml::literals::toml_literals;
  using namespace std;

  auto dataxxx = R"(

[[focus-filter]]
titles = [".*"]

[[focus-filter]]
titles = ["aaa"]
no-keywords = [".*"]

)"_toml;

  vector<tgf::FilterToml> filters
    = toml::find<vector<tgf::FilterToml>> (dataxxx, "focus-filter");

  tgfass (filters.size () == 2);

  auto fcf_list = tgf::FilterGroupToml (dataxxx);
  tgfass (fcf_list.n_filter () == 2);

  {
    tgf::TgMsg msg ("title_bbb"s, "sd_bbb"s, "kw_bbb"s);
    tgfass (fcf_list.mtch_tgmsg (msg));
    tgfass (fcf_list.i_prev_visited () == 0);
  }

  {
    tgf::TgMsg msg ("title_aaa"s, "sd_bbb"s, "kw_bbb"s);
    tgfass (fcf_list.mtch_tgmsg (msg));
    tgfass (fcf_list.i_prev_visited () == 0);
  }
}

void
test_how_to_no_keywords ()
{
  using namespace toml::literals::toml_literals;
  using namespace std;

  auto dataxxx = R"(

[[focus-filter]]
senders = ["micl2e2"]
keywords = ["cheese"]

[[focus-filter]]
no-senders = ["micl2e2"]

)"_toml;

  vector<tgf::FilterToml> filters
    = toml::find<vector<tgf::FilterToml>> (dataxxx, "focus-filter");

  tgfass (filters.size () == 2);

  auto fcf_list = tgf::FilterGroupToml (dataxxx);
  tgfass (fcf_list.n_filter () == 2);

  {
    tgf::TgMsg msg ("anytitle"s, "sd_micl2e2"s, ""s);
    tgfass (!fcf_list.mtch_tgmsg (msg));
  }

  {
    tgf::TgMsg msg ("anytitle"s, "sd_micl2e2"s, "a"s);
    tgfass (!fcf_list.mtch_tgmsg (msg));
  }

  {
    tgf::TgMsg msg ("anytitle"s, "sd_micl2e2"s, "kw_cheese"s);
    tgfass (fcf_list.mtch_tgmsg (msg));
  }
}

void
test_how_to_no_keywords2 ()
{
  using namespace toml::literals::toml_literals;
  using namespace std;

  auto dataxxx = R"(

[[focus-filter]]
no-senders = ["micl2e2"]

[[focus-filter]]
senders = ["micl2e2"]
keywords = ["cheese"]

)"_toml;

  vector<tgf::FilterToml> filters
    = toml::find<vector<tgf::FilterToml>> (dataxxx, "focus-filter");

  tgfass (filters.size () == 2);

  auto fcf_list = tgf::FilterGroupToml (dataxxx);
  tgfass (fcf_list.n_filter () == 2);

  {
    tgf::TgMsg msg ("anytitle"s, "sd_micl2e2"s, ""s);
    tgfass (!fcf_list.mtch_tgmsg (msg));
  }

  {
    tgf::TgMsg msg ("anytitle"s, "sd_micl2e2"s, "a"s);
    tgfass (!fcf_list.mtch_tgmsg (msg));
  }

  {
    tgf::TgMsg msg ("anytitle"s, "sd_micl2e2"s, "kw_cheese"s);
    tgfass (!fcf_list.mtch_tgmsg (msg));
  }
}

void
test_how_to_no_titles ()
{
  using namespace toml::literals::toml_literals;
  using namespace std;

  auto dataxxx = R"(

[[focus-filter]]
no-titles= ["aaa"]

[[focus-filter]]
keywords = [".*"]

)"_toml;

  vector<tgf::FilterToml> filters
    = toml::find<vector<tgf::FilterToml>> (dataxxx, "focus-filter");

  tgfass (filters.size () == 2);

  auto fcf_list = tgf::FilterGroupToml (dataxxx);
  tgfass (fcf_list.n_filter () == 2);

  {
    tgf::TgMsg msg ("title_any"s, ""s, ""s);
    tgfass (fcf_list.mtch_tgmsg (msg));
  }

  {
    tgf::TgMsg msg ("title_any"s, ""s, ""s);
    tgfass (fcf_list.mtch_tgmsg (msg));
  }

  {
    tgf::TgMsg msg ("title_aaa"s, ""s, ""s);
    tgfass (!fcf_list.mtch_tgmsg (msg));
  }
}

void
test_how_to_no_titles_order_is_signif ()
{
  using namespace toml::literals::toml_literals;
  using namespace std;

  auto dataxxx = R"(

[[focus-filter]]
keywords = [".*"]

[[focus-filter]]
no-titles= ["aaa"]

)"_toml;

  vector<tgf::FilterToml> filters
    = toml::find<vector<tgf::FilterToml>> (dataxxx, "focus-filter");

  tgfass (filters.size () == 2);

  auto fcf_list = tgf::FilterGroupToml (dataxxx);
  tgfass (fcf_list.n_filter () == 2);

  {
    tgf::TgMsg msg ("title_any"s, ""s, ""s);
    tgfass (fcf_list.mtch_tgmsg (msg));
  }

  {
    tgf::TgMsg msg ("title_any"s, ""s, ""s);
    tgfass (fcf_list.mtch_tgmsg (msg));
  }

  {
    tgf::TgMsg msg ("title_aaa"s, ""s, ""s);
    tgfass (fcf_list.mtch_tgmsg (msg)); // now match anyway
  }
}

void
test_how_to_no_titles_using_nokeywords ()
{
  using namespace toml::literals::toml_literals;
  using namespace std;

  auto dataxxx = R"(

[[focus-filter]]
titles= ["aaa"]
no-keywords = [".*"]

[[focus-filter]]
titles = []

)"_toml;

  vector<tgf::FilterToml> filters
    = toml::find<vector<tgf::FilterToml>> (dataxxx, "focus-filter");

  tgfass (filters.size () == 2);

  auto fcf_list = tgf::FilterGroupToml (dataxxx);
  tgfass (fcf_list.n_filter () == 2);

  {
    tgf::TgMsg msg (""s, ""s, ""s);
    tgfass (fcf_list.mtch_tgmsg (msg));
  }

  {
    tgf::TgMsg msg (""s, ""s, ""s);
    tgfass (fcf_list.mtch_tgmsg (msg));
  }

  {
    tgf::TgMsg msg ("title_aaa"s, ""s, ""s);
    tgfass (!fcf_list.mtch_tgmsg (msg)); // now match anyway
  }
}

void
test_how_to_no_titles_using_nokeywords_emptyfilter ()
{
  using namespace toml::literals::toml_literals;
  using namespace std;

  auto dataxxx = R"(

[[focus-filter]]
titles= ["aaa"]
no-keywords = [".*"]

[[focus-filter]]

)"_toml;

  vector<tgf::FilterToml> filters
    = toml::find<vector<tgf::FilterToml>> (dataxxx, "focus-filter");

  tgfass (filters.size () == 2);

  auto fcf_list = tgf::FilterGroupToml (dataxxx);
  tgfass (fcf_list.n_filter () == 2);

  {
    tgf::TgMsg msg (""s, ""s, ""s);
    tgfass (fcf_list.mtch_tgmsg (msg));
  }

  {
    tgf::TgMsg msg (""s, ""s, ""s);
    tgfass (fcf_list.mtch_tgmsg (msg));
  }

  {
    tgf::TgMsg msg ("title_aaa"s, ""s, ""s);
    tgfass (!fcf_list.mtch_tgmsg (msg)); // now match anyway
  }
}

void
test_how_to_no_titles_using_nokeywords_empty_nokw_wont_work ()
{
  using namespace toml::literals::toml_literals;
  using namespace std;

  auto dataxxx = R"(

[[focus-filter]]
titles= ["aaa"]
no-keywords = []

[[focus-filter]]

)"_toml;

  vector<tgf::FilterToml> filters
    = toml::find<vector<tgf::FilterToml>> (dataxxx, "focus-filter");

  tgfass (filters.size () == 2);

  auto fcf_list = tgf::FilterGroupToml (dataxxx);
  tgfass (fcf_list.n_filter () == 2);

  {
    tgf::TgMsg msg (""s, ""s, ""s);
    tgfass (fcf_list.mtch_tgmsg (msg));
  }

  {
    tgf::TgMsg msg (""s, ""s, ""s);
    tgfass (fcf_list.mtch_tgmsg (msg));
  }

  {
    tgf::TgMsg msg ("title_aaa"s, ""s, ""s);
    tgfass (fcf_list.mtch_tgmsg (msg)); // now match anyway
  }
}

int
main ()
{
  test_default_filter ();
  test_default_fg ();

  test_sub_match1 ();
  test_sub_match2 ();

  test_focus_filter_list1 ();
  test_focus_filter_list2 ();
  test_focus_filter_list3 ();
  test_focus_filter_list4 ();

  test_match_and_submatch1 ();
  test_match_and_submatch2 ();
  test_match_and_submatch1_ ();
  test_match_and_submatch2_ ();

  test_nokw_not_much_inituitive ();
  test_nokw_not_much_inituitive2 ();
  test_how_to_no_keywords ();
  test_how_to_no_keywords2 ();

  test_how_to_no_titles ();
  test_how_to_no_titles_order_is_signif ();
  test_how_to_no_titles_using_nokeywords ();
  test_how_to_no_titles_using_nokeywords_emptyfilter ();
  test_how_to_no_titles_using_nokeywords_empty_nokw_wont_work ();
}
