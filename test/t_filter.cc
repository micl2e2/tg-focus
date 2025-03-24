#include <assert.h>

#include "toml.hpp"
#include "filter.hh"
#include "toml/from.hpp"
#include "toml/parser.hpp"
#include "toml/string.hpp"

void
test_default_filter ()
{
  tgf::FilterToml f;
  string ptn = f.getptn_title ();
  assert (ptn == ".*");
}

void
test_sub_match1 ()
{
  using namespace toml::literals::toml_literals;
  using namespace std;

  auto dataxxx = R"(
# this line will be ignored by parser

[[focus-filter]]
title = "bcd"

[[focus-filter]]
title = "xxx"
senders = ["usr1","usr2"]

[[focus-filter]]
title = "xxx"
no-senders = ["usr3","usr4",]

[[focus-filter]]
title = "A{2,}B{2,}"
keywords = ["kw1", "kw2", ]

[[focus-filter]]
keywords = [ "kw3", "kw4", "(😺){3}" ]

[[focus-filter]]
title = "xxx"
keywords = ["kw5"]
no-keywords = ["kw6","kw7"]

)"_toml;

  vector<tgf::FilterToml> filters
    = toml::find<vector<tgf::FilterToml>> (dataxxx, "focus-filter");

  assert (filters.size () == 6);

  // [0]
  assert (filters[0].isMatchTitle ("abcde"));
  assert (!filters[0].isMatchTitle ("cd"));
  // if absent, then match anything
  assert (filters[0].isKeywordsMatch (""));
  // if absent, then not match anything
  assert (!filters[0].isNoKeywordsMatch (""));
  assert (filters[0].isMatchSender (""));
  assert (!filters[0].isNoSendersMatch (""));

  // [1]
  assert (filters[1].isMatchSender ("usr1"));
  assert (filters[1].isMatchSender ("usr2"));
  assert (!filters[1].isMatchSender ("usr"));

  // [2]
  assert (filters[2].isNoSendersMatch ("usr3"));
  assert (filters[2].isNoSendersMatch ("usr4"));
  assert (!filters[2].isNoSendersMatch ("usr"));

  // [3]
  assert (filters[3].isMatchTitle ("AABB"));
  assert (filters[3].isMatchTitle ("AAABBB"));
  assert (!filters[3].isMatchTitle ("AB"));
  assert (filters[3].isKeywordsMatch ("kw1"));
  assert (filters[3].isKeywordsMatch ("kw2"));
  assert (!filters[3].isKeywordsMatch ("kw3"));

  // [4]
  // if title absent, match anything
  assert (filters[4].isMatchTitle (""));
  assert (filters[4].isMatchTitle (" "));
  assert (filters[4].isMatchTitle ("123abcABC😺"));
  assert (filters[4].isKeywordsMatch ("kw3"));
  assert (filters[4].isKeywordsMatch ("kw4"));
  assert (!filters[4].isKeywordsMatch ("kw1"));
  assert (!filters[4].isKeywordsMatch ("😺"));
  assert (!filters[4].isKeywordsMatch ("😺😺"));
  assert (filters[4].isKeywordsMatch ("😺😺😺"));
  assert (filters[4].isKeywordsMatch ("😺😺😺😺"));

  // [5]
  assert (filters[5].isKeywordsMatch ("kw5"));
  assert (filters[5].isNoKeywordsMatch ("kw6"));
  assert (filters[5].isNoKeywordsMatch ("kw7"));
  assert (!filters[5].isNoKeywordsMatch ("kw8"));
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
title = "bcd"

[[focus-filter]]
title = "xxx"
senders = ["usr1","usr2"]

[[focus-filter]]
title = "xxx"
no-senders = ["usr3","usr4",]

[[focus-filter]]
title = "A{2,}B{2,}"
keywords = ["kw1", "kw2", ]

[[focus-filter]]
keywords = [ "kw3", "kw4", "(😺){3}" ]

[[focus-filter]]
title = "xxx"
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

  assert (filters.size () == 6);

  // [0]
  assert (filters[0].isMatchTitle ("abcde"));
  assert (!filters[0].isMatchTitle ("cd"));
  // if absent, then match anything
  assert (filters[0].isKeywordsMatch (""));
  // if absent, then not match anything
  assert (!filters[0].isNoKeywordsMatch (""));
  assert (filters[0].isMatchSender (""));
  assert (!filters[0].isNoSendersMatch (""));

  // [1]
  assert (filters[1].isMatchSender ("usr1"));
  assert (filters[1].isMatchSender ("usr2"));
  assert (!filters[1].isMatchSender ("usr"));

  // [2]
  assert (filters[2].isNoSendersMatch ("usr3"));
  assert (filters[2].isNoSendersMatch ("usr4"));
  assert (!filters[2].isNoSendersMatch ("usr"));

  // [3]
  assert (filters[3].isMatchTitle ("AABB"));
  assert (filters[3].isMatchTitle ("AAABBB"));
  assert (!filters[3].isMatchTitle ("AB"));
  assert (filters[3].isKeywordsMatch ("kw1"));
  assert (filters[3].isKeywordsMatch ("kw2"));
  assert (!filters[3].isKeywordsMatch ("kw3"));

  // [4]
  // if title absent, match anything
  assert (filters[4].isMatchTitle (""));
  assert (filters[4].isMatchTitle (" "));
  assert (filters[4].isMatchTitle ("123abcABC😺"));
  assert (filters[4].isKeywordsMatch ("kw3"));
  assert (filters[4].isKeywordsMatch ("kw4"));
  assert (!filters[4].isKeywordsMatch ("kw1"));
  assert (!filters[4].isKeywordsMatch ("😺"));
  assert (!filters[4].isKeywordsMatch ("😺😺"));
  assert (filters[4].isKeywordsMatch ("😺😺😺"));
  assert (filters[4].isKeywordsMatch ("😺😺😺😺"));

  // [5]
  assert (filters[5].isKeywordsMatch ("kw5"));
  assert (filters[5].isNoKeywordsMatch ("kw6"));
  assert (filters[5].isNoKeywordsMatch ("kw7"));
  assert (!filters[5].isNoKeywordsMatch ("kw8"));
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
    assert (fcf_list.n_filter () == 1);
    assert (fcf_list.isMatchTgMsg (msg));

    // note: an implicit title=".*" inserted
  }

  { // title matched
    auto tomldata = R"(
[[focus-filter]]
)"_toml;
    auto fcf_list = tgf::FilterGroupToml (tomldata);
    assert (fcf_list.n_filter () == 1);
    assert (fcf_list.isMatchTgMsg (msg));
  }

  { // title matched
    auto tomldata = R"(
[[focus-filter]]
title = ["xxx"]
)"_toml;
    auto fcf_list = tgf::FilterGroupToml (tomldata);
    assert (fcf_list.n_filter () == 1);
    assert (fcf_list.isMatchTgMsg (msg));
  }

  { // senders matched
    auto tomldata = R"(
[[focus-filter]]
senders = ["xxx"]
)"_toml;
    auto fcf_list = tgf::FilterGroupToml (tomldata);
    assert (fcf_list.n_filter () == 1);
    assert (fcf_list.isMatchTgMsg (msg));
  }

  { // keywords matched
    auto tomldata = R"(
[[focus-filter]]
keywords = ["xxx"]
)"_toml;
    auto fcf_list = tgf::FilterGroupToml (tomldata);
    assert (fcf_list.n_filter () == 1);
    assert (fcf_list.isMatchTgMsg (msg));
  }

  { // no-keywords matched
    auto tomldata = R"(
[[focus-filter]]
no-keywords = ["xxx"]
)"_toml;
    auto fcf_list = tgf::FilterGroupToml (tomldata);
    assert (fcf_list.n_filter () == 1);
    assert (!fcf_list.isMatchTgMsg (msg));
  }

  { // no-keywords not matched
    auto tomldata = R"(
[[focus-filter]]
no-keywords = ["yyy"]
)"_toml;
    auto fcf_list = tgf::FilterGroupToml (tomldata);
    assert (fcf_list.n_filter () == 1);
    assert (fcf_list.isMatchTgMsg (msg));
  }

  { // no-senders not matched
    auto tomldata = R"(
[[focus-filter]]
no-senders = ["yyy"]
)"_toml;
    auto fcf_list = tgf::FilterGroupToml (tomldata);
    assert (fcf_list.n_filter () == 1);
    assert (fcf_list.isMatchTgMsg (msg));
  }

  { // senders vs. no-senders
    auto tomldata = R"(
[[focus-filter]]
senders = ["x{2}"]
no-senders = ["xxx"]
)"_toml;
    auto fcf_list = tgf::FilterGroupToml (tomldata);
    assert (fcf_list.n_filter () == 1);
    assert (!fcf_list.isMatchTgMsg (msg));
  }

  { // senders and keywords matched
    auto tomldata = R"(
[[focus-filter]]
senders = ["xxx"]
keywords = ["xxx"]
)"_toml;
    auto fcf_list = tgf::FilterGroupToml (tomldata);
    assert (fcf_list.n_filter () == 1);
    assert (fcf_list.isMatchTgMsg (msg));
  }

  { // senders matched, keywords not
    auto tomldata = R"(
[[focus-filter]]
senders = ["xxx"]
keywords = ["yyy"]
)"_toml;
    auto fcf_list = tgf::FilterGroupToml (tomldata);
    assert (fcf_list.n_filter () == 1);
    assert (!fcf_list.isMatchTgMsg (msg));
  }

  { // senders not, keywords matched
    auto tomldata = R"(
[[focus-filter]]
senders = ["yyy"]
keywords = ["xxx"]
)"_toml;
    auto fcf_list = tgf::FilterGroupToml (tomldata);
    assert (fcf_list.n_filter () == 1);
    assert (!fcf_list.isMatchTgMsg (msg));
  }

  { // all last matched
    auto tomldata = R"(
[[focus-filter]]
senders = ["a","b","c","d","x+"]
keywords = ["a","b","c","d","x+"]
)"_toml;
    auto fcf_list = tgf::FilterGroupToml (tomldata);
    assert (fcf_list.n_filter () == 1);
    assert (fcf_list.isMatchTgMsg (msg));
  }

  { // 2nd filter matched
    auto tomldata = R"(
[[focus-filter]]
senders = ["yyy"]

[[focus-filter]]
keywords = ["x{3}"]
)"_toml;
    auto fcf_list = tgf::FilterGroupToml (tomldata);
    assert (fcf_list.n_filter () == 2);
    assert (fcf_list.isMatchTgMsg (msg));
    assert (fcf_list.i_prev_matched () == 1);
  }

  { // 1st filter matched
    auto tomldata = R"(
[[focus-filter]]
senders = ["x{2}"]

[[focus-filter]]
keywords = ["x{3}"]
)"_toml;
    auto fcf_list = tgf::FilterGroupToml (tomldata);
    assert (fcf_list.n_filter () == 2);
    assert (fcf_list.isMatchTgMsg (msg));
    assert (fcf_list.i_prev_matched () == 0);
  }

  { // more complicated
    auto tomldata = R"(
[[focus-filter]]
senders = ["x{2}"]
no-senders = ["xxx"]

[[focus-filter]]
)"_toml;
    auto fcf_list = tgf::FilterGroupToml (tomldata);
    assert (fcf_list.n_filter () == 2);
    assert (fcf_list.isMatchTgMsg (msg));
    assert (fcf_list.i_prev_matched () == 1);
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
    assert (fcf_list.n_filter () == 3);
    assert (fcf_list.isMatchTgMsg (msg));
    assert (fcf_list.i_prev_matched () == 2);
  }

  { // more complicated
    auto tomldata = R"(
[[focus-filter]]
no-senders = ["xxx"]

[[focus-filter]]
no-keywords = ["xxx"]

[[focus-filter]]
no-keywords = ["[a-z]+"]

[[focus-filter]]
keywords = "[:alpha:]{3}"
)"_toml;
    auto fcf_list = tgf::FilterGroupToml (tomldata);
    assert (fcf_list.n_filter () == 4);
    assert (fcf_list.isMatchTgMsg (msg));
    assert (fcf_list.i_prev_matched () == 3);
  }

  { // more complicated
    auto tomldata = R"(
[[focus-filter]]
no-senders = ["xxx"]

[[focus-filter]]
no-keywords = "xxx" # remember this should be a list

[[focus-filter]]
no-keywords = ["[a-z]+"] 

[[focus-filter]]
keywords = "[:alpha:]{3}"
)"_toml;
    auto fcf_list = tgf::FilterGroupToml (tomldata);
    assert (fcf_list.n_filter () == 4);
    assert (fcf_list.isMatchTgMsg (msg));
    assert (fcf_list.i_prev_matched () == 1);
    // note: here the default title=".*" matched, since the 2nd no-keywords is
    //       parsed as 0-length vector
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
    assert (fcf_list.n_filter () == 1);
    assert (fcf_list.isMatchTgMsg (msg));
    assert (fcf_list.i_prev_matched () == 0);
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
    assert (fcf_list.n_filter () == 5);
    assert (fcf_list.isMatchTgMsg (msg));
    assert (fcf_list.i_prev_matched () == 4);
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
    assert (fcf_list.n_filter () == 3);
    assert (fcf_list.isMatchTgMsg (msg));
    assert (fcf_list.i_prev_matched () == 1);
  }

  { // more complicated
    auto tomldata = R"(
[[focus-filter]]
no-senders = ["(😺🐶){2}"]

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
    assert (fcf_list.n_filter () == 4);
    assert (fcf_list.isMatchTgMsg (msg));
    assert (fcf_list.i_prev_matched () == 3);
    // note: [1] and [2] is skipped bc both filters' <no-keywords> reject the
    //       input, even though both filters's <senders> accept it.
  }

  { // more complicated
    auto tomldata = R"(
[[focus-filter]]
no-senders = ["(😺🐶){2}"]

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
    assert (fcf_list.n_filter () == 4);
    assert (fcf_list.isMatchTgMsg (msg));
    assert (fcf_list.i_prev_matched () == 1);
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
    assert (fcf_list.n_filter () == 1);
    assert (fcf_list.isMatchTgMsg (msg));
    assert (fcf_list.i_prev_matched () == 0);
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
    assert (fcf_list.n_filter () == 1);
    assert (fcf_list.isMatchTgMsg (msg));
    assert (fcf_list.i_prev_matched () == 0);
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

  assert (filters.size () == 1);

  // [0]
  assert (filters[0].isNoSendersMatch ("sd1"));
  assert (filters[0].isNoSendersMatch ("sd2"));
  assert (filters[0].isKeywordsMatch ("kw1"));
  assert (filters[0].isKeywordsMatch ("kw2"));

  auto fcf_list = tgf::FilterGroupToml (dataxxx);
  assert (fcf_list.n_filter () == 1);

  {
    tgf::TgMsg msg ("chatxxx"s, "sd1"s, "kw2"s);
    assert (!fcf_list.isMatchTgMsg (msg));
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

  assert (filters.size () == 2);

  // [0]
  assert (filters[0].isNoSendersMatch ("sd1"));
  assert (filters[0].isNoSendersMatch ("sd2"));
  assert (filters[0].isKeywordsMatch ("kw1"));
  assert (filters[0].isKeywordsMatch ("kw2"));

  auto fcf_list = tgf::FilterGroupToml (dataxxx);
  assert (fcf_list.n_filter () == 2);

  {
    tgf::TgMsg msg ("chatxxx"s, "sd1"s, "kw2"s);
    assert (fcf_list.isMatchTgMsg (msg));
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
rej-senders = ["sd1","sd2"]
keywords = ["kw1","kw2"]

)"_toml;

  vector<tgf::FilterToml> filters
    = toml::find<vector<tgf::FilterToml>> (dataxxx, "focus-filter");

  assert (filters.size () == 1);

  // [0]
  assert (filters[0].isRejSendersMatch ("sd1"));
  assert (filters[0].isRejSendersMatch ("sd2"));
  assert (filters[0].isKeywordsMatch ("kw1"));
  assert (filters[0].isKeywordsMatch ("kw2"));

  auto fcf_list = tgf::FilterGroupToml (dataxxx);
  assert (fcf_list.n_filter () == 1);

  {
    tgf::TgMsg msg ("chatxxx"s, "sd1"s, "kw2"s);
    assert (!fcf_list.isMatchTgMsg (msg));
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
rej-senders = ["sd1","sd2"]
keywords = ["kw1","kw2"]

[[focus-filter]]
keywords = ["kw1","kw2"]

)"_toml;

  vector<tgf::FilterToml> filters
    = toml::find<vector<tgf::FilterToml>> (dataxxx, "focus-filter");

  assert (filters.size () == 2);

  // [0]
  assert (filters[0].isRejSendersMatch ("sd1"));
  assert (filters[0].isRejSendersMatch ("sd2"));
  assert (filters[0].isKeywordsMatch ("kw1"));
  assert (filters[0].isKeywordsMatch ("kw2"));

  auto fcf_list = tgf::FilterGroupToml (dataxxx);
  assert (fcf_list.n_filter () == 2);

  {
    tgf::TgMsg msg ("chatxxx"s, "sd1"s, "kw2"s);
    assert (!fcf_list.isMatchTgMsg (msg));
  }
}

int
main ()
{
  test_default_filter ();

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
}
