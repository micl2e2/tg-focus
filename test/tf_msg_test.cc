#include "tf_msg.hh"
#include "tgf_locale.hh"
#include <assert.h>
#include <iostream>

using namespace std;

void
test_not_decorate ()
{
  {
    TgMsg msg (("michael2 | TG-Focusing"), ("michael2 | TG-Focusing"),
	       ("XXXXXXXXX"), (1705740724));
    string msg_lcstr = msg.to_locale_string ();
    string expected = R"([ CHAT ] michael2 | TG-Focusing
[ SENDER ] michael2 | TG-Focusing
[ CONTENT ] XXXXXXXXX
[ DATE ] 2024-01-20 08:52:04 +0000 UTC
[ ID ] -1
)";
    cout << msg_lcstr << endl;
    assert (msg_lcstr == expected);

    vector<tuple<int, int>> pos_info = get_decor_pos (expected);

    cout << pos_info.size () << endl;
    assert (pos_info.size () == 0);
  }
}

void
test_should_decorate_en_us ()
{
  assert (tgf::try_ensure_locale ());

  // ascii + nonascii
  {
    TgMsg msg (("michael2 | TG-Focusing"), ("michael2 | TG-Focusing"),
	       ("好好好好好好好好好好好好好好好。"), (1705740724));
    string msg_lcstr = msg.to_locale_string ();
    string expected = R"([ CHAT ] michael2 | TG-Focusing
[ SENDER ] michael2 | TG-Focusing
[ CONTENT ] 好好好好好好好好好好好好好好好。
[ DATE ] 2024-01-20 08:52:04 +0000 UTC
[ ID ] -1
)";
    cout << msg_lcstr << endl;
    assert (msg_lcstr == expected);

    vector<tuple<int, int>> pos_info = get_decor_pos (msg_lcstr);

    cout << pos_info.size () << endl;
    assert (pos_info.size () == 5);

    assert ((pos_info[0] == make_tuple<int, int> (0, 8)));
    assert ((pos_info[1] == make_tuple<int, int> (32, 10)));
    assert ((pos_info[2] == make_tuple<int, int> (66, 11)));
    assert ((pos_info[3] == make_tuple<int, int> (95, 8)));
    assert ((pos_info[4] == make_tuple<int, int> (134, 6)));
  }

  // ascii + nonascii
  {
    TgMsg msg (("michael2 | TG-Focusing"), ("michael2 | TG-Focusing"),
	       ("ののののののののAA..."), (1705740724));
    string msg_lcstr = msg.to_locale_string ();
    string expected = R"([ CHAT ] michael2 | TG-Focusing
[ SENDER ] michael2 | TG-Focusing
[ CONTENT ] ののののののののAA...
[ DATE ] 2024-01-20 08:52:04 +0000 UTC
[ ID ] -1
)";
    cout << msg_lcstr << endl;
    assert (msg_lcstr == expected);

    vector<tuple<int, int>> pos_info = get_decor_pos (msg_lcstr);

    cout << pos_info.size () << endl;
    assert (pos_info.size () == 5);

    assert ((pos_info[0] == make_tuple<int, int> (0, 8)));
    assert ((pos_info[1] == make_tuple<int, int> (32, 10)));
    assert ((pos_info[2] == make_tuple<int, int> (66, 11)));
    assert ((pos_info[3] == make_tuple<int, int> (92, 8)));
    assert ((pos_info[4] == make_tuple<int, int> (131, 6)));
  }

  // ascii + emoji
  {
    TgMsg msg (("michael2 | TG-Focusing"), ("michael2 | TG-Focusing"),
	       ("🤣🤣🤣🤣🤣🤣🤣🤣AA..."), (1705740724));
    string msg_lcstr = msg.to_locale_string ();
    string expected = R"([ CHAT ] michael2 | TG-Focusing
[ SENDER ] michael2 | TG-Focusing
[ CONTENT ] 🤣🤣🤣🤣🤣🤣🤣🤣AA...
[ DATE ] 2024-01-20 08:52:04 +0000 UTC
[ ID ] -1
)";
    cout << msg_lcstr << endl;
    assert (msg_lcstr == expected);

    vector<tuple<int, int>> pos_info = get_decor_pos (msg_lcstr);

    cout << pos_info.size () << endl;
    assert (pos_info.size () == 5);

    // <0,8>,<32,10>,<66,11>,<100,8>,<139,6>
    assert ((pos_info[0] == make_tuple<int, int> (0, 8)));
    assert ((pos_info[1] == make_tuple<int, int> (32, 10)));
    assert ((pos_info[2] == make_tuple<int, int> (66, 11)));
    assert ((pos_info[3] == make_tuple<int, int> (100, 8)));
    assert ((pos_info[4] == make_tuple<int, int> (139, 6)));
  }

  // ascii + nonascii + emoji
  {
    TgMsg msg (("michael2🐰🐭🐹🐻🐶🐱🌼🏵️💮🌸🪷🌺 | "
		"TG-Focusing"),
	       //
	       ("michael2🐰🐭🐹🐻🐶🐱🌼🏵️💮🌸🪷🌺 | "
		"TG-Focusing"),
	       //
	       ("🤣🤣🤣🤣🤣のののののAAAAA好好好好好"),
	       //
	       (1705740724));
    string msg_lcstr = msg.to_locale_string ();
    string expected
      = R"([ CHAT ] michael2🐰🐭🐹🐻🐶🐱🌼🏵️💮🌸🪷🌺 | TG-Focusing
[ SENDER ] michael2🐰🐭🐹🐻🐶🐱🌼🏵️💮🌸🪷🌺 | TG-Focusing
[ CONTENT ] 🤣🤣🤣🤣🤣のののののAAAAA好好好好好
[ DATE ] 2024-01-20 08:52:04 +0000 UTC
[ ID ] -1
)";
    cout << msg_lcstr << endl;
    assert (msg_lcstr == expected);

    vector<tuple<int, int>> pos_info = get_decor_pos (msg_lcstr);

    cout << pos_info.size () << endl;
    assert (pos_info.size () == 5);

    // <0,8>,<57,10>,<116,11>,<154,8>,<193,6>
    assert ((pos_info[0] == make_tuple<int, int> (0, 8)));
    assert ((pos_info[1] == make_tuple<int, int> (57, 10)));
    assert ((pos_info[2] == make_tuple<int, int> (116, 11)));
    assert ((pos_info[3] == make_tuple<int, int> (154, 8)));
    assert ((pos_info[4] == make_tuple<int, int> (193, 6)));
  }

  // ascii + emoji(sometimes rendered as 1 emoji, sometimes 2)
  {
    TgMsg msg (("michael2 | TG-Focusing"), ("michael2 | TG-Focusing"),
	       ("aaa😮‍💨😮‍💨😮‍💨aaa"), (1705740724));
    string msg_lcstr = msg.to_locale_string ();
    string expected = R"([ CHAT ] michael2 | TG-Focusing
[ SENDER ] michael2 | TG-Focusing
[ CONTENT ] aaa😮‍💨😮‍💨😮‍💨aaa
[ DATE ] 2024-01-20 08:52:04 +0000 UTC
[ ID ] -1
)";
    cout << msg_lcstr << endl;
    assert (msg_lcstr == expected);

    vector<tuple<int, int>> pos_info = get_decor_pos (msg_lcstr);

    cout << pos_info.size () << endl;
    assert (pos_info.size () == 5);

    // :<0,8>,<32,10>,<66,11>,<100,8>,<139,6>,
    assert ((pos_info[0] == make_tuple<int, int> (0, 8)));
    assert ((pos_info[1] == make_tuple<int, int> (32, 10)));
    assert ((pos_info[2] == make_tuple<int, int> (66, 11)));
    assert ((pos_info[3] == make_tuple<int, int> (100, 8)));
    assert ((pos_info[4] == make_tuple<int, int> (139, 6)));
  }
}

void
test_should_decorate_zh_cn ()
{
  assert (tgf::try_ensure_locale ());

  // ascii + nonascii
  {
    TgMsg msg (("michael2"), ("michael2"), ("好好好。"), (1705740724));
    string msg_lcstr = msg.to_locale_string ();
    string expected = R"([ 群组 ] michael2
[ 用户 ] michael2
[ 信息 ] 好好好。
[ 时间 ] 2024-01-20 08:52:04 +0000 UTC
[ 标识 ] -1
)";
    cout << msg_lcstr << endl;
    assert (msg_lcstr == expected);

    vector<tuple<int, int>> pos_info = get_decor_pos (msg_lcstr);
    cout << pos_info.size () << endl;
    assert (pos_info.size () == 5);

    assert ((pos_info[0] == make_tuple<int, int> (0, 6)));
    assert ((pos_info[1] == make_tuple<int, int> (16, 6)));
    assert ((pos_info[2] == make_tuple<int, int> (32, 6)));
    assert ((pos_info[3] == make_tuple<int, int> (44, 6)));
    assert ((pos_info[4] == make_tuple<int, int> (81, 6)));
  }
}

void
test_should_decorate_zh_hk ()
{
  assert (tgf::try_ensure_locale ());

  // ascii + nonascii
  {
    TgMsg msg (("michael2"), ("michael2"), ("好好好。"), (1705740724));
    string msg_lcstr = msg.to_locale_string ();
    string expected = R"([ 群組 ] michael2
[ 用戶 ] michael2
[ 訊息 ] 好好好。
[ 時間 ] 2024-01-20 08:52:04 +0000 UTC
[ 標識 ] -1
)";
    cout << msg_lcstr << endl;
    assert (msg_lcstr == expected);

    vector<tuple<int, int>> pos_info = get_decor_pos (msg_lcstr);
    cout << pos_info.size () << endl;
    assert (pos_info.size () == 5);

    assert ((pos_info[0] == make_tuple<int, int> (0, 6)));
    assert ((pos_info[1] == make_tuple<int, int> (16, 6)));
    assert ((pos_info[2] == make_tuple<int, int> (32, 6)));
    assert ((pos_info[3] == make_tuple<int, int> (44, 6)));
    assert ((pos_info[4] == make_tuple<int, int> (81, 6)));
  }
}

//
// ATTENTION: make sure TZ is UTC when testing
//

int
main ()
{
  // C or POSIX
  test_not_decorate ();

  tgf::PREFER_LANG = tgf::Lang::en_US;
  if (tgf::try_ensure_locale () && tgf::HOST_LANG == tgf::PREFER_LANG)
    {
      test_should_decorate_en_us ();
    }

  tgf::PREFER_LANG = tgf::Lang::zh_HK;
  if (tgf::try_ensure_locale () && tgf::HOST_LANG == tgf::PREFER_LANG)
    {
      test_should_decorate_zh_hk ();
    }

  tgf::PREFER_LANG = tgf::Lang::zh_CN;
  if (tgf::try_ensure_locale () && tgf::HOST_LANG == tgf::PREFER_LANG)
    {
      test_should_decorate_zh_cn ();
    }

  return 0;
}
