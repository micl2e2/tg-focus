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

  {
    string s = "TGFCMD"s;
    vector seq = get_c16_seq (s);
    cerr << seq.size () << endl;
    tgfass (seq.size () == 6);
    cerr << scast<int> (seq[0]) << endl;
    tgfass (seq[0] == 0x0054);
    tgfass (seq[1] == 0x0047);
    tgfass (seq[2] == 0x0046);
    tgfass (seq[3] == 0x0043);
    tgfass (seq[4] == 0x004d);
    tgfass (seq[5] == 0x0044);
  }

  {
    string s = "zß水🍌"s;
    vector seq = get_c16_seq (s);
    cerr << seq.size () << endl;
    tgfass (seq.size () == 5);
    cerr << scast<int> (seq[0]) << endl;
    tgfass (seq[0] == 0x007a);
    tgfass (seq[1] == 0x00df);
    tgfass (seq[2] == 0x6c34);
    tgfass (seq[3] == 0xd83c);
    tgfass (seq[4] == 0xdf4c);
  }

  return 0;
}
