#include <iostream>
#include <assert.h>
#include "tf_msg.hh"

int
main ()
{
  using namespace std;

  assert (false);

  std::setlocale (LC_ALL, "en_US.UTF-8");

  // ascii + nonascii
  {
    string s = R"([ CHAT ] AAAAA AAAAAAAA 好好好
[ SENDER ] 好好好好好好h
[ CONTENT ] 好好好好好好好好好好好好好好好。
[ DATE ] 2023-09-25 13:18:11 +0800 HKT
[ ID ] 121)";

    vector<tuple<int, int>> pos_info = get_decor_pos (s);

    cout << pos_info.size () << endl;
    assert (pos_info.size () == 5);

    assert ((pos_info[0] == make_tuple<int, int> (1, 8)));
    assert ((pos_info[1] == make_tuple<int, int> (28, 10)));
    assert ((pos_info[2] == make_tuple<int, int> (47, 11)));
    assert ((pos_info[3] == make_tuple<int, int> (76, 8)));
    assert ((pos_info[4] == make_tuple<int, int> (115, 6)));
  }

  // ascii + nonascii
  {
    string s = R"([ CHAT ] AAAAA AAAAAAAA ののの
[ SENDER ] のののののの?
[ CONTENT ] ののののののののAA...
[ DATE ] 2023-09-25 13:20:00 +0800 HKT
[ ID ] 139)";

    vector<tuple<int, int>> pos_info = get_decor_pos (s);

    cout << pos_info.size () << endl;
    assert (pos_info.size () == 5);

    assert ((pos_info[0] == make_tuple<int, int> (0, 8)));
    assert ((pos_info[1] == make_tuple<int, int> (28, 10)));
    assert ((pos_info[2] == make_tuple<int, int> (47, 11)));
    assert ((pos_info[3] == make_tuple<int, int> (73, 8)));
    assert ((pos_info[4] == make_tuple<int, int> (112, 6)));
  }

  // ascii + emoji
  {
    string s = R"([ CHAT ] AAAAA AAAAAAAA 🤣🤣🤣
[ SENDER ] 🤣🤣🤣🤣🤣🤣?
[ CONTENT ] 🤣🤣🤣🤣🤣🤣🤣🤣AA...
[ DATE ] 2023-09-25 13:20:00 +0800 HKT
[ ID ] 139)";

    vector<tuple<int, int>> pos_info = get_decor_pos (s);

    cout << pos_info.size () << endl;
    assert (pos_info.size () == 5);

    assert ((pos_info[0] == make_tuple<int, int> (0, 8)));
    assert ((pos_info[1] == make_tuple<int, int> (28, 10)));
    assert ((pos_info[2] == make_tuple<int, int> (47, 11)));
    assert ((pos_info[3] == make_tuple<int, int> (73, 8)));
    assert ((pos_info[4] == make_tuple<int, int> (112, 6)));
  }

  // ascii + nonascii + emoji
  {
    string s = R"([ CHAT ] ABCDE 你好
[ SENDER ] abcde @abcde
[ CONTENT ] 1234567.89🤣
[ DATE ] 2023-09-25 13:19:57 +0800 HKT
[ ID ] 141)";

    vector<tuple<int, int>> pos_info = get_decor_pos (s);

    cout << pos_info.size () << endl;
    assert (pos_info.size () == 5);

    assert ((pos_info[0] == make_tuple<int, int> (0, 8)));
    assert ((pos_info[1] == make_tuple<int, int> (18, 10)));
    assert ((pos_info[2] == make_tuple<int, int> (42, 11)));
    assert ((pos_info[3] == make_tuple<int, int> (66, 8)));
    assert ((pos_info[4] == make_tuple<int, int> (105, 6)));
  }

  // ascii + nonascii + emoji(sometimes rendered as 1 emoji, sometimes 2)
  {
    string s = R"([ CHAT ] #AAAAAAAAA-AA-AAAAAAAA
[ SENDER ] ありがとAAAAAAAAA
[ CONTENT ] <sticker>(😮‍💨)
[ DATE ] 2023-09-25 13:19:47 +0800 HKT
[ ID ] 135)";

    vector<tuple<int, int>> pos_info = get_decor_pos (s);

    cout << pos_info.size () << endl;
    assert (pos_info.size () == 5);

    assert ((pos_info[0] == make_tuple<int, int> (0, 8)));
    assert ((pos_info[1] == make_tuple<int, int> (32, 10)));
    assert ((pos_info[2] == make_tuple<int, int> (57, 11)));
    assert ((pos_info[3] == make_tuple<int, int> (84, 8)));
    assert ((pos_info[4] == make_tuple<int, int> (123, 6)));
  }

  return 0;
}
