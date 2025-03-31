#include "std_comp.hh"
#include "utfutils.hh"
#include <locale.h>

int
main ()
{
  {
    string s = ""s;
    size_t len = get_c16_len (s);
    cerr << len << endl;
    tgfass (len == 0);
  }

  {
    string s = "x"s;
    size_t len = get_c16_len (s);
    cerr << len << endl;
    tgfass (len == 1);
  }

  {
    string s = "TGFCMD"s;
    size_t len = get_c16_len (s);
    cerr << len << endl;
    tgfass (len == 6);
  }

  {
    setlocale (LC_ALL, "en_US.utf8");
    string s = "zß水🍌"s;
    size_t len = get_c16_len (s);
    cerr << len << endl;
    tgfass (len == 5);
  }

  return 0;
}
