#include <assert.h>

#include "toml.hpp"
#include "focus_filter.hh"
#include "toml/from.hpp"
#include "toml/parser.hpp"
#include "toml/string.hpp"

//
// config is allowed to contains comments
void
test_focus_filter_config ()
{
  using namespace toml::literals::toml_literals;
  using namespace std;

  auto dataxxx = R"(
# this line will be ignored by parser

[[focus-filter]]
title = "xxx"
senders = ["usr1","usr2"])"_toml;

  vector<FocusFilter> filters
    = toml::find<vector<FocusFilter>> (dataxxx, "focus-filter");

  assert (filters.size () == 1);

  // [0]
  assert (filters[0].is_sender_match ("usr1"));
  assert (filters[0].is_sender_match ("usr2"));
  assert (!filters[0].is_sender_match ("usr"));
}

void
test_focus_filter_config2 ()
{
  using namespace toml::literals::toml_literals;
  using namespace std;

  auto dataxxx = R"(
# wrap around list is ok

[[focus-filter]]
title = "xxx"
senders = ["usr1",
"usr2",])"_toml;

  vector<FocusFilter> filters
    = toml::find<vector<FocusFilter>> (dataxxx, "focus-filter");

  assert (filters.size () == 1);

  // [0]
  assert (filters[0].is_sender_match ("usr1"));
  assert (filters[0].is_sender_match ("usr2"));
  assert (!filters[0].is_sender_match ("usr"));
}

void
test_focus_filter_config3 ()
{
  using namespace toml::literals::toml_literals;
  using namespace std;

  auto dataxxx = R"(
# wrap around list is allow; multiple-line value is allowed

[[focus-filter]]
title = """
xxx
"""
senders = [
"usr1",
"usr2",
])"_toml;

  vector<FocusFilter> filters
    = toml::find<vector<FocusFilter>> (dataxxx, "focus-filter");

  assert (filters.size () == 1);

  // [0]
  assert (filters[0].is_title_match ("\nxxx\n"));
  assert (filters[0].is_sender_match ("usr1"));
  assert (filters[0].is_sender_match ("usr2"));
  assert (!filters[0].is_sender_match ("usr"));
}

void
test_focus_filter_config4 ()
{
  using namespace toml::literals::toml_literals;
  using namespace std;

  auto dataxxx = R"(
# wrap around list is allow; multiple-line value is allowed;
# multiple-line value inside a list is allowed

[[focus-filter]]
title = """
xxx"""
senders = ["usr1",
"usr2",
"""
usrrr333
"""])"_toml;

  vector<FocusFilter> filters
    = toml::find<vector<FocusFilter>> (dataxxx, "focus-filter");

  assert (filters.size () == 1);

  // [0]
  assert (filters[0].is_title_match ("xxx"));
  // FIXME: comply with toml standard?

  assert (filters[0].is_sender_match ("usr1"));
  assert (filters[0].is_sender_match ("usr2"));
  assert (filters[0].is_sender_match ("\nusrrr333\n"));
  assert (!filters[0].is_sender_match ("usr"));
}

int
main ()
{
  test_focus_filter_config ();
  test_focus_filter_config2 ();
  test_focus_filter_config3 ();
  test_focus_filter_config4 ();
}
