#include <assert.h>

#include "toml.hpp"
#include "focus_filter.hh"
#include "toml/from.hpp"
#include "toml/parser.hpp"
#include "toml/string.hpp"

void
test_sub_match1 ()
{
  using namespace toml::literals::toml_literals;
  using namespace std;

  auto dataxxx = R"(
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

  vector<FocusFilter> filters
    = toml::find<vector<FocusFilter>> (dataxxx, "focus-filter");

  assert (filters.size () == 6);

  // [0]
  assert (filters[0].is_title_match ("abcde"));
  assert (!filters[0].is_title_match ("cd"));
  // if absent, then match anything
  assert (filters[0].is_keyword_match (""));
  // if absent, then not match anything
  assert (!filters[0].is_no_keyword_match (""));
  assert (filters[0].is_sender_match (""));
  assert (!filters[0].is_no_sender_match (""));

  // [1]
  assert (filters[1].is_sender_match ("usr1"));
  assert (filters[1].is_sender_match ("usr2"));
  assert (!filters[1].is_sender_match ("usr"));

  // [2]
  assert (filters[2].is_no_sender_match ("usr3"));
  assert (filters[2].is_no_sender_match ("usr4"));
  assert (!filters[2].is_no_sender_match ("usr"));

  // [3]
  assert (filters[3].is_title_match ("AABB"));
  assert (filters[3].is_title_match ("AAABBB"));
  assert (!filters[3].is_title_match ("AB"));
  assert (filters[3].is_keyword_match ("kw1"));
  assert (filters[3].is_keyword_match ("kw2"));
  assert (!filters[3].is_keyword_match ("kw3"));

  // [4]
  // if title absent, match anything
  assert (filters[4].is_title_match (""));
  assert (filters[4].is_title_match (" "));
  assert (filters[4].is_title_match ("123abcABC😺"));
  assert (filters[4].is_keyword_match ("kw3"));
  assert (filters[4].is_keyword_match ("kw4"));
  assert (!filters[4].is_keyword_match ("kw1"));
  assert (!filters[4].is_keyword_match ("😺"));
  assert (!filters[4].is_keyword_match ("😺😺"));
  assert (filters[4].is_keyword_match ("😺😺😺"));
  assert (filters[4].is_keyword_match ("😺😺😺😺"));

  // [5]
  assert (filters[5].is_keyword_match ("kw5"));
  assert (filters[5].is_no_keyword_match ("kw6"));
  assert (filters[5].is_no_keyword_match ("kw7"));
  assert (!filters[5].is_no_keyword_match ("kw8"));
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

  vector<FocusFilter> filters
    = toml::find<vector<FocusFilter>> (tomldata, "focus-filter");

  assert (filters.size () == 6);

  // [0]
  assert (filters[0].is_title_match ("abcde"));
  assert (!filters[0].is_title_match ("cd"));
  // if absent, then match anything
  assert (filters[0].is_keyword_match (""));
  // if absent, then not match anything
  assert (!filters[0].is_no_keyword_match (""));
  assert (filters[0].is_sender_match (""));
  assert (!filters[0].is_no_sender_match (""));

  // [1]
  assert (filters[1].is_sender_match ("usr1"));
  assert (filters[1].is_sender_match ("usr2"));
  assert (!filters[1].is_sender_match ("usr"));

  // [2]
  assert (filters[2].is_no_sender_match ("usr3"));
  assert (filters[2].is_no_sender_match ("usr4"));
  assert (!filters[2].is_no_sender_match ("usr"));

  // [3]
  assert (filters[3].is_title_match ("AABB"));
  assert (filters[3].is_title_match ("AAABBB"));
  assert (!filters[3].is_title_match ("AB"));
  assert (filters[3].is_keyword_match ("kw1"));
  assert (filters[3].is_keyword_match ("kw2"));
  assert (!filters[3].is_keyword_match ("kw3"));

  // [4]
  // if title absent, match anything
  assert (filters[4].is_title_match (""));
  assert (filters[4].is_title_match (" "));
  assert (filters[4].is_title_match ("123abcABC😺"));
  assert (filters[4].is_keyword_match ("kw3"));
  assert (filters[4].is_keyword_match ("kw4"));
  assert (!filters[4].is_keyword_match ("kw1"));
  assert (!filters[4].is_keyword_match ("😺"));
  assert (!filters[4].is_keyword_match ("😺😺"));
  assert (filters[4].is_keyword_match ("😺😺😺"));
  assert (filters[4].is_keyword_match ("😺😺😺😺"));

  // [5]
  assert (filters[5].is_keyword_match ("kw5"));
  assert (filters[5].is_no_keyword_match ("kw6"));
  assert (filters[5].is_no_keyword_match ("kw7"));
  assert (!filters[5].is_no_keyword_match ("kw8"));
}

void
test_focus_filter_list1 ()
{
  using namespace toml::literals::toml_literals;
  using namespace std;

  TgMsg msg ("chatxxx"s, "senderxxx"s, "txtxxx"s);

  { // title matched
    auto tomldata = R"(

)"_toml;
    auto fcf_list = FocusFilterList (tomldata);
    assert (fcf_list.n_filter () == 1);
    assert (fcf_list.is_tgmsg_match (msg));

    // note: an implicit title=".*" inserted
  }

  { // title matched
    auto tomldata = R"(
[[focus-filter]]
)"_toml;
    auto fcf_list = FocusFilterList (tomldata);
    assert (fcf_list.n_filter () == 1);
    assert (fcf_list.is_tgmsg_match (msg));
  }

  { // title matched
    auto tomldata = R"(
[[focus-filter]]
title = ["xxx"]
)"_toml;
    auto fcf_list = FocusFilterList (tomldata);
    assert (fcf_list.n_filter () == 1);
    assert (fcf_list.is_tgmsg_match (msg));
  }

  { // senders matched
    auto tomldata = R"(
[[focus-filter]]
senders = ["xxx"]
)"_toml;
    auto fcf_list = FocusFilterList (tomldata);
    assert (fcf_list.n_filter () == 1);
    assert (fcf_list.is_tgmsg_match (msg));
  }

  { // keywords matched
    auto tomldata = R"(
[[focus-filter]]
keywords = ["xxx"]
)"_toml;
    auto fcf_list = FocusFilterList (tomldata);
    assert (fcf_list.n_filter () == 1);
    assert (fcf_list.is_tgmsg_match (msg));
  }

  { // no-keywords matched
    auto tomldata = R"(
[[focus-filter]]
no-keywords = ["xxx"]
)"_toml;
    auto fcf_list = FocusFilterList (tomldata);
    assert (fcf_list.n_filter () == 1);
    assert (!fcf_list.is_tgmsg_match (msg));
  }

  { // no-keywords not matched
    auto tomldata = R"(
[[focus-filter]]
no-keywords = ["yyy"]
)"_toml;
    auto fcf_list = FocusFilterList (tomldata);
    assert (fcf_list.n_filter () == 1);
    assert (fcf_list.is_tgmsg_match (msg));
  }

  { // no-senders not matched
    auto tomldata = R"(
[[focus-filter]]
no-senders = ["yyy"]
)"_toml;
    auto fcf_list = FocusFilterList (tomldata);
    assert (fcf_list.n_filter () == 1);
    assert (fcf_list.is_tgmsg_match (msg));
  }

  { // senders vs. no-senders
    auto tomldata = R"(
[[focus-filter]]
senders = ["x{2}"]
no-senders = ["xxx"]
)"_toml;
    auto fcf_list = FocusFilterList (tomldata);
    assert (fcf_list.n_filter () == 1);
    assert (!fcf_list.is_tgmsg_match (msg));
  }

  { // senders and keywords matched
    auto tomldata = R"(
[[focus-filter]]
senders = ["xxx"]
keywords = ["xxx"]
)"_toml;
    auto fcf_list = FocusFilterList (tomldata);
    assert (fcf_list.n_filter () == 1);
    assert (fcf_list.is_tgmsg_match (msg));
  }

  { // senders matched, keywords not
    auto tomldata = R"(
[[focus-filter]]
senders = ["xxx"]
keywords = ["yyy"]
)"_toml;
    auto fcf_list = FocusFilterList (tomldata);
    assert (fcf_list.n_filter () == 1);
    assert (!fcf_list.is_tgmsg_match (msg));
  }

  { // senders not, keywords matched
    auto tomldata = R"(
[[focus-filter]]
senders = ["yyy"]
keywords = ["xxx"]
)"_toml;
    auto fcf_list = FocusFilterList (tomldata);
    assert (fcf_list.n_filter () == 1);
    assert (!fcf_list.is_tgmsg_match (msg));
  }

  { // all last matched
    auto tomldata = R"(
[[focus-filter]]
senders = ["a","b","c","d","x+"]
keywords = ["a","b","c","d","x+"]
)"_toml;
    auto fcf_list = FocusFilterList (tomldata);
    assert (fcf_list.n_filter () == 1);
    assert (fcf_list.is_tgmsg_match (msg));
  }

  { // 2nd filter matched
    auto tomldata = R"(
[[focus-filter]]
senders = ["yyy"]

[[focus-filter]]
keywords = ["x{3}"]
)"_toml;
    auto fcf_list = FocusFilterList (tomldata);
    assert (fcf_list.n_filter () == 2);
    assert (fcf_list.is_tgmsg_match (msg));
    assert (fcf_list.i_prev_matched () == 1);
  }

  { // 1st filter matched
    auto tomldata = R"(
[[focus-filter]]
senders = ["x{2}"]

[[focus-filter]]
keywords = ["x{3}"]
)"_toml;
    auto fcf_list = FocusFilterList (tomldata);
    assert (fcf_list.n_filter () == 2);
    assert (fcf_list.is_tgmsg_match (msg));
    assert (fcf_list.i_prev_matched () == 0);
  }

  { // more complicated
    auto tomldata = R"(
[[focus-filter]]
senders = ["x{2}"]
no-senders = ["xxx"]

[[focus-filter]]
)"_toml;
    auto fcf_list = FocusFilterList (tomldata);
    assert (fcf_list.n_filter () == 2);
    assert (fcf_list.is_tgmsg_match (msg));
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
    auto fcf_list = FocusFilterList (tomldata);
    assert (fcf_list.n_filter () == 3);
    assert (fcf_list.is_tgmsg_match (msg));
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
    auto fcf_list = FocusFilterList (tomldata);
    assert (fcf_list.n_filter () == 4);
    assert (fcf_list.is_tgmsg_match (msg));
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
    auto fcf_list = FocusFilterList (tomldata);
    assert (fcf_list.n_filter () == 4);
    assert (fcf_list.is_tgmsg_match (msg));
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

  TgMsg msg ("-"s, "😺🐶😺🐶"s, "😺😺😺🐶🐶🐶"s);

  { // more complicated
    auto tomldata = R"(
[[focus-filter]]
senders = ["😺🐶"]
)"_toml;
    auto fcf_list = FocusFilterList (tomldata);
    assert (fcf_list.n_filter () == 1);
    assert (fcf_list.is_tgmsg_match (msg));
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
    auto fcf_list = FocusFilterList (tomldata);
    assert (fcf_list.n_filter () == 5);
    assert (fcf_list.is_tgmsg_match (msg));
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
    auto fcf_list = FocusFilterList (tomldata);
    assert (fcf_list.n_filter () == 3);
    assert (fcf_list.is_tgmsg_match (msg));
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
    auto fcf_list = FocusFilterList (tomldata);
    assert (fcf_list.n_filter () == 4);
    assert (fcf_list.is_tgmsg_match (msg));
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
    auto fcf_list = FocusFilterList (tomldata);
    assert (fcf_list.n_filter () == 4);
    assert (fcf_list.is_tgmsg_match (msg));
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

  TgMsg msg ("-"s, "xxx"s, "linux is good"s);

  { // toml literal
    auto tomldata = R"(
[[focus-filter]]
keywords = ["linux", "gnu", "fsf"]
no-keywords = ["macos"]
)"_toml;
    auto fcf_list = FocusFilterList (tomldata);
    assert (fcf_list.n_filter () == 1);
    assert (fcf_list.is_tgmsg_match (msg));
    assert (fcf_list.i_prev_matched () == 0);
  }
}

void
test_focus_filter_list4 ()
{
  using namespace std;

  TgMsg msg ("-"s, "xxx"s, "linux is good"s);

  { // cstr
    auto tomldata = R"(
[[focus-filter]]
keywords = ["linux", "gnu", "fsf"]
no-keywords = ["macos"]
)";
    auto fcf_list = FocusFilterList (tomldata);
    assert (fcf_list.n_filter () == 1);
    assert (fcf_list.is_tgmsg_match (msg));
    assert (fcf_list.i_prev_matched () == 0);
  }
}

int
main ()
{
  test_sub_match1 ();
  test_sub_match2 ();

  test_focus_filter_list1 ();
  test_focus_filter_list2 ();
  test_focus_filter_list3 ();
  test_focus_filter_list4 ();
}
