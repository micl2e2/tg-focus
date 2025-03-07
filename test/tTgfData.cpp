#include <assert.h>
#include <optional>
#include <filesystem>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

#include "TgfData.hpp"

using namespace std;

// disable this when testing droot locking
static constexpr bool RESET_ALL_DUMMPY_HOME = true;

void
tst_files_existence ()
{
  const string PRED_HOME = "/tmp/dummy_home1";

  TgfData tfdata{make_optional (PRED_HOME), RESET_ALL_DUMMPY_HOME};

  assert (fs::exists (PRED_HOME));

  assert (fs::exists (tfdata.path_tddata ()));
  assert (fs::is_directory (tfdata.path_tddata ()));

  assert (fs::exists (tfdata.path_api_id ()));
  assert (fs::is_regular_file (tfdata.path_api_id ()));

  assert (fs::exists (tfdata.path_api_hash ()));
  assert (fs::is_regular_file (tfdata.path_api_hash ()));

  assert (fs::exists (tfdata.path_auth_hint ()));
  assert (fs::is_regular_file (tfdata.path_auth_hint ()));

  assert (fs::exists (tfdata.path_filters ()));
  assert (fs::is_regular_file (tfdata.path_filters ()));

  assert (fs::exists (tfdata.path_filters_tmp ()));
  assert (fs::is_regular_file (tfdata.path_filters_tmp ()));

  // "{}/{}/abc"
  string expected = "";
  expected += PRED_HOME;
  expected += "/";
  expected += tfdata.name_droot ();
  expected += "/";
  expected += TF_VER_MAJOR;
  expected += "/abc";

  cerr << "expected: " << expected << endl;
  assert (tfdata.abspath_of ("abc") == expected);
}

void
tst_files_content ()
{
  const string PRED_HOME = "/tmp/dummy_home2";

  TgfData tfdata{make_optional (PRED_HOME), RESET_ALL_DUMMPY_HOME};

  // return;

  // api_id ------------------------

  {
    int32_t strdata = tfdata.get_api_id ();
    assert (strdata == 0);
  }

  // set api_id
  {
    tfdata.set_api_id (1122334455);
    int32_t strdata = tfdata.get_api_id ();
    assert (strdata == 1122334455);
  }

  // change api_id
  {
    tfdata.set_api_id (2147483647);
    int32_t strdata = tfdata.get_api_id ();
    assert (strdata == 2147483647);
  }

  // change api_id
  {
    tfdata.set_api_id (2147483648);
    int32_t strdata = tfdata.get_api_id ();
    assert (strdata == -2147483648);
  }

  // api_hash ------------------------

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

  // auth_hint ------------------------

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

  // filters ------------------------

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

  // filters_tmp ------------------------

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

  // tgfid ------------------------

  {
    int64_t strdata = tfdata.get_tgfid ();
    assert (strdata == -1);
    assert (tfdata.is_tgfid_valid () == false);
  }

  {
    tfdata.set_tgfid (-4115780813);
    int64_t strdata = tfdata.get_tgfid ();
    assert (strdata == -4115780813);
    assert (tfdata.is_tgfid_valid () == true);
  }

  {
    tfdata.set_tgfid (-1);
    int64_t strdata = tfdata.get_tgfid ();
    assert (strdata == -1);
    assert (tfdata.is_tgfid_valid () == false);
  }

  {
    tfdata.set_tgfid (-2);
    int64_t strdata = tfdata.get_tgfid ();
    assert (strdata == -2);
    assert (tfdata.is_tgfid_valid () == true);
  }

  {
    tfdata.set_tgfid (-4162446887);
    int64_t strdata = tfdata.get_tgfid ();
    assert (strdata == -4162446887);
    assert (tfdata.is_tgfid_valid () == true);
  }

  {
    tfdata.set_tgfid (-4101385855);
    int64_t strdata = tfdata.get_tgfid ();
    assert (strdata == -4101385855);
    assert (tfdata.is_tgfid_valid () == true);
  }

  // pref_lang ------------------------

  {
    tgf::Lang data = tfdata.get_pref_lang ();
    cout << data << endl;
    assert (data == tgf::Lang::unknown);
  }

  {
    tfdata.set_pref_lang (tgf::Lang::aa_DJ);
    tgf::Lang data = tfdata.get_pref_lang ();
    cout << data << endl;
    assert (data == tgf::Lang::aa_DJ);
  }

  {
    tfdata.set_pref_lang (tgf::Lang::en_HK);
    tgf::Lang data = tfdata.get_pref_lang ();
    cout << data << endl;
    assert (data == tgf::Lang::en_HK);
  }

  {
    tfdata.set_pref_lang (tgf::Lang::en_ZW);
    tgf::Lang data = tfdata.get_pref_lang ();
    cout << data << endl;
    assert (data == tgf::Lang::en_ZW);
  }
}

int
main ()
{
  tst_files_existence ();
  tst_files_content ();
}
