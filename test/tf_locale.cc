#include <assert.h>
#include <iostream>
#include <locale.h>

#include "tf_locale.hh"

using namespace std;

void
locales_must_have ()
{
  if (setlocale (LC_ALL, "en_HK.UTF-8") == nullptr
      || setlocale (LC_ALL, "en_US.UTF-8") == nullptr
      || setlocale (LC_ALL, "en_ZW.UTF-8") == nullptr)
    assert (false);

  if (setlocale (LC_ALL, "C") == nullptr)
    assert (false);
}

void
init_and_prefer (const char *init_lc_all, tgf::Lang prefer1, tgf::Lang prefer2)
{
  setlocale (LC_ALL, init_lc_all);
  tgf::PREFER_LANG = prefer1;
  tgf::try_ensure_locale ();
  cout << "prefer:" << tgf::PREFER_LANG << " actual:" << tgf::HOST_LANG << endl;
  assert (tgf::HOST_LANG == prefer1);
  tgf::PREFER_LANG = prefer2;
  tgf::try_ensure_locale ();
  cout << "prefer:" << tgf::PREFER_LANG << " actual:" << tgf::HOST_LANG << endl;
  assert (tgf::HOST_LANG == prefer2);
}

int
main ()
{
  assert (tgf::HOST_LANG == tgf::Lang::unknown);
  assert (tgf::PREFER_LANG == tgf::Lang::unknown);

  locales_must_have ();

  // init: en_HK
  init_and_prefer ("en_HK.UTF-8", tgf::Lang::en_US, tgf::Lang::en_ZW);
  init_and_prefer ("en_HK.UTF-8", tgf::Lang::en_ZW, tgf::Lang::en_US);

  // init: en_US
  init_and_prefer ("en_US.UTF-8", tgf::Lang::en_ZW, tgf::Lang::en_HK);
  init_and_prefer ("en_US.UTF-8", tgf::Lang::en_HK, tgf::Lang::en_ZW);

  // init: en_ZW
  init_and_prefer ("en_ZW.UTF-8", tgf::Lang::en_US, tgf::Lang::en_HK);
  init_and_prefer ("en_ZW.UTF-8", tgf::Lang::en_HK, tgf::Lang::en_US);

  return 0;
}
