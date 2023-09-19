#include <assert.h>
#include <optional>
#include <filesystem>
#include <string>

#include <fmt/core.h>

namespace fs = std::filesystem;

#include "tg_focus_data.hh"

using namespace std;

// disable this when testing droot locking
static constexpr bool RESET_ALL_DUMMPY_HOME = true;

void
tst_files_existence ()
{
  const string PRED_HOME = "/tmp/dummy_home1";

  TgFocusData tfdata{make_optional (PRED_HOME), RESET_ALL_DUMMPY_HOME};

  assert (fs::exists (PRED_HOME));

  assert (fs::exists (tfdata.path_tddata ()));
  assert (fs::is_directory (tfdata.path_tddata ()));

  assert (fs::exists (tfdata.path_api_id ()));
  assert (fs::is_regular_file (tfdata.path_api_id ()));

  assert (fs::exists (tfdata.path_api_hash ()));
  assert (fs::is_regular_file (tfdata.path_api_hash ()));

  assert (fs::exists (tfdata.path_phone ()));
  assert (fs::is_regular_file (tfdata.path_phone ()));

  assert (fs::exists (tfdata.path_vcode ()));
  assert (fs::is_regular_file (tfdata.path_vcode ()));

  assert (fs::exists (tfdata.path_auth_hint ()));
  assert (fs::is_regular_file (tfdata.path_auth_hint ()));

  assert (fs::exists (tfdata.path_filters ()));
  assert (fs::is_regular_file (tfdata.path_filters ()));

  assert (fs::exists (tfdata.path_filters_tmp ()));
  assert (fs::is_regular_file (tfdata.path_filters_tmp ()));

  auto expected = fmt::format ("{}/{}/abc", PRED_HOME, tfdata.name_droot ());
  cerr << "expected: " << expected << endl;
  assert (tfdata.abspath_of ("abc") == expected);
}

void
tst_files_content ()
{
  const string PRED_HOME = "/tmp/dummy_home2";

  TgFocusData tfdata{make_optional (PRED_HOME), RESET_ALL_DUMMPY_HOME};

  {
    auto strdata = tfdata.get_api_id ();
    assert (strdata == "");
  }

  // set api_id
  {
    tfdata.set_api_id (std::string ("1122334455"));
    auto strdata = tfdata.get_api_id ();
    assert (strdata == "1122334455");
  }

  // change api_id
  {
    tfdata.set_api_id ("2147483647");
    auto strdata = tfdata.get_api_id ();
    assert (strdata == "2147483647");
  }

  // change api_id
  {
    tfdata.set_api_id ("2147483648");
    auto strdata = tfdata.get_api_id ();
    assert (strdata == "2147483647");
  }

  // change api_id (invalid num1)
  {
    tfdata.set_api_id ("5+5");
    auto strdata = tfdata.get_api_id ();
    cerr << strdata << endl;
    assert (strdata == "2147483647");
  }

  // change api_id (invalid num2)
  {
    tfdata.set_api_id ("a");
    auto strdata = tfdata.get_api_id ();
    assert (strdata == "2147483647");
  }

  // change api_id (invalid num3)
  {
    tfdata.set_api_id ("1111111111111111111111111111111111");
    auto strdata = tfdata.get_api_id ();
    cerr << strdata << endl;
    assert (strdata == "2147483647");
  }

  // set api_hash
  {
    tfdata.set_api_hash (std::string ("aabbccddee"));
    auto strdata = tfdata.get_api_hash ();
    assert (strdata == "aabbccddee");
  }

  // change api_hash
  {
    tfdata.set_api_hash ("qqwwee");
    auto strdata = tfdata.get_api_hash ();
    assert (strdata == "qqwwee");
  }

  // set phone
  {
    tfdata.set_phone (std::string ("+1 12345"));
    auto strdata = tfdata.get_phone ();
    assert (strdata == "+1 12345");
  }

  // change phone
  {
    tfdata.set_phone ("+1 222333");
    auto strdata = tfdata.get_phone ();
    assert (strdata == "+1 222333");
  }

  // set vcode
  {
    tfdata.set_vcode (std::string ("91601"));
    auto strdata = tfdata.get_vcode ();
    assert (strdata == "91601");
  }

  // change vcode
  {
    tfdata.set_vcode ("11601");
    auto strdata = tfdata.get_vcode ();
    assert (strdata == "11601");
  }

  // set auth hint
  {
    tfdata.set_auth_hint (true);
    auto may_authorized = tfdata.get_auth_hint ();
    assert (may_authorized == true);
  }

  // change auth hint
  {
    tfdata.set_auth_hint (false);
    auto may_authorized = tfdata.get_auth_hint ();
    assert (may_authorized == false);
  }

  // change auth hint
  {
    tfdata.set_auth_hint (true);
    auto may_authorized = tfdata.get_auth_hint ();
    assert (may_authorized == true);
  }

  // default filters
  {
    auto strdata = tfdata.get_filters ();
    assert (strdata == R"([[focus-filter]]
title = ".*"
)");
  }

  // change filters
  {
    tfdata.set_filters (R"([[focus-filter]]
title = ".*"

[[focus-filter]]
keywords = ["abc"]
)");
    auto strdata = tfdata.get_filters ();
    assert (strdata == R"([[focus-filter]]
title = ".*"

[[focus-filter]]
keywords = ["abc"]
)");
  }

  // filters tmp
  {
    auto strdata = tfdata.get_filters_tmp ();
    assert (strdata == "");

    if (tfdata.prepare_filters_tmp ())
      {
	// just after prepare
	auto strdata = tfdata.get_filters_tmp ();
	assert (strdata == R"([[focus-filter]]
title = ".*"

[[focus-filter]]
keywords = ["abc"]
)");

	// dont touch original filters
	auto strdata2 = tfdata.get_filters ();
	assert (strdata2 == R"([[focus-filter]]
title = ".*"

[[focus-filter]]
keywords = ["abc"]
)");

	// change filters
	tfdata.set_filters (R"([[focus-filter]]
title = "ttt"

[[focus-filter]]
keywords = ["xyz"]
)");

	// if not prepare, stay still
	auto strdata3 = tfdata.get_filters_tmp ();
	assert (strdata3 == R"([[focus-filter]]
title = ".*"

[[focus-filter]]
keywords = ["abc"]
)");
	// after prepare
	if (tfdata.prepare_filters_tmp ())
	  {
	    auto strdata3 = tfdata.get_filters_tmp ();
	    assert (strdata3 == R"([[focus-filter]]
title = "ttt"

[[focus-filter]]
keywords = ["xyz"]
)");
	  }
      }
  }
}

int
main ()
{
  tst_files_existence ();
  tst_files_content ();
}
