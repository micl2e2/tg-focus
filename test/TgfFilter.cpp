#include <assert.h>

#include "toml.hpp"
#include "TgfFilter.hpp"
#include "toml/from.hpp"
#include "toml/parser.hpp"
#include "toml/string.hpp"

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

  vector<TgfFilter<toml::value>> filters
    = toml::find<vector<TgfFilter<toml::value>>> (dataxxx, "focus-filter");

  assert (filters.size () == 6);

  // [0]
  assert (filters[0].is_title_match ("abcde"));
}

int
main ()
{
  test_sub_match1 ();
}
