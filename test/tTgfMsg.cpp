#include "TgfMsg.hpp"
#include "tgf_locale.hh"
#include <assert.h>
#include <iostream>

using namespace std;

void
test_not_decorate ()
{
  {
    TgfMsg msg (("michael2 | TG-Focusing"), ("michael2 | TG-Focusing"),
	       ("XXXXXXXXX"), (1705740724));
    string msg_lcstr = msg.to_locale_string ();
    string expected
      = "\xf0\x9f\x92\xa1 | michael2 | TG-Focusing\n\xe2\x9c\x89 | michael2 | "
	"TG-Focusing\n\xf0\x9f\x92\xac | XXXXXXXXX\n\xf0\x9f\x95\x94 | "
	"2024-01-20 08:52:04 +0000 UTC\n\xe2\x99\xbe | 0\n";
    cout << msg_lcstr << endl;
    cout << expected << endl;
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
    TgfMsg msg (("michael2 | TG-Focusing"), ("michael2 | TG-Focusing"),
	       ("好好好好好好好好好好好好好好好。"), (1705740724));
    string msg_lcstr = msg.to_locale_string ();
    string expected = "\xf0\x9f\x92\xa1 | michael2 | TG-Focusing\n\xe2\x9c\x89 "
		      "| michael2 | TG-Focusing\n\xf0\x9f\x92\xac | "
		      "好好好好好好好好好好好好好好好。\n\xf0\x9f\x95\x94 | "
		      "2024-01-20 08:52:04 +0000 UTC\n\xe2\x99\xbe | 0\n";
    cout << msg_lcstr << endl;
    assert (msg_lcstr == expected);

    vector<tuple<int, int>> pos_info = get_decor_pos (msg_lcstr);

    cout << pos_info.size () << endl;
    assert (pos_info.size () == 0);
  }

  // ascii + nonascii
  {
    TgfMsg msg (("michael2 | TG-Focusing"), ("michael2 | TG-Focusing"),
	       ("ののののののののAA..."), (1705740724));
    string msg_lcstr = msg.to_locale_string ();
    string expected = "\xf0\x9f\x92\xa1 | michael2 | TG-Focusing\n\xe2\x9c\x89 "
		      "| michael2 | TG-Focusing\n\xf0\x9f\x92\xac | "
		      "ののののののののAA...\n\xf0\x9f\x95\x94 | 2024-01-20 "
		      "08:52:04 +0000 UTC\n\xe2\x99\xbe | 0\n";
    cout << msg_lcstr << endl;
    assert (msg_lcstr == expected);

    vector<tuple<int, int>> pos_info = get_decor_pos (msg_lcstr);

    cout << pos_info.size () << endl;
    assert (pos_info.size () == 0);
  }

  // ascii + emoji
  {
    TgfMsg msg (("michael2 | TG-Focusing"), ("michael2 | TG-Focusing"),
	       ("🤣🤣🤣🤣🤣🤣🤣🤣AA..."), (1705740724));
    string msg_lcstr = msg.to_locale_string ();
    string expected = "\xf0\x9f\x92\xa1 | michael2 | TG-Focusing\n\xe2\x9c\x89 "
		      "| michael2 | TG-Focusing\n\xf0\x9f\x92\xac | "
		      "🤣🤣🤣🤣🤣🤣🤣🤣AA...\n\xf0\x9f\x95\x94 "
		      "| 2024-01-20 08:52:04 +0000 UTC\n\xe2\x99\xbe | 0\n";
    cout << msg_lcstr << endl;
    assert (msg_lcstr == expected);

    vector<tuple<int, int>> pos_info = get_decor_pos (msg_lcstr);

    cout << pos_info.size () << endl;
    assert (pos_info.size () == 0);
  }

  // ascii + nonascii + emoji
  {
    TgfMsg msg (("michael2🐰🐭🐹🐻🐶🐱🌼🏵️💮🌸🪷🌺 | "
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
      = "\xf0\x9f\x92\xa1 | "
	"michael2🐰🐭🐹🐻🐶🐱🌼🏵️💮🌸🪷🌺 "
	"| "
	"TG-Focusing\n\xe2\x9c\x89 | "
	"michael2🐰🐭🐹🐻🐶🐱🌼🏵️💮🌸🪷🌺 "
	"| "
	"TG-Focusing\n\xf0\x9f\x92\xac | "
	"🤣🤣🤣🤣🤣のののののAAAAA好好好好好\n\xf0\x9f\x95"
	"\x94"
	" "
	"| "
	"2024-01-20 08:52:04 +0000 UTC\n\xe2\x99\xbe | 0\n";
    cout << msg_lcstr << endl;
    assert (msg_lcstr == expected);

    vector<tuple<int, int>> pos_info = get_decor_pos (msg_lcstr);

    cout << pos_info.size () << endl;
    assert (pos_info.size () == 0);
  }

  // ascii + emoji(sometimes rendered as 1 emoji, sometimes 2)
  {
    TgfMsg msg (("michael2 | TG-Focusing"), ("michael2 | TG-Focusing"),
	       ("aaa😮‍💨😮‍💨😮‍💨aaa"), (1705740724));
    string msg_lcstr = msg.to_locale_string ();
    string expected
      = "\xf0\x9f\x92\xa1 | michael2 | TG-Focusing\n\xe2\x9c\x89 "
	"| michael2 | TG-Focusing\n\xf0\x9f\x92\xac | "
	"aaa😮‍💨😮‍💨😮‍💨aaa\n\xf0\x9f\x95\x94 | "
	"2024-01-20 "
	"08:52:04 +0000 UTC\n\xe2\x99\xbe | 0\n";
    cout << msg_lcstr << endl;
    assert (msg_lcstr == expected);

    vector<tuple<int, int>> pos_info = get_decor_pos (msg_lcstr);

    cout << pos_info.size () << endl;
    assert (pos_info.size () == 0);
  }
}

void
test_should_decorate_zh_cn ()
{
  assert (tgf::try_ensure_locale ());

  // ascii + nonascii
  {
    TgfMsg msg (("michael2"), ("michael2"), ("好好好。"), (1705740724));
    string msg_lcstr = msg.to_locale_string ();
    string expected = R"([ 群组 ] michael2
[ 用户 ] michael2
[ 信息 ] 好好好。
[ 时间 ] 2024-01-20 08:52:04 +0000 UTC
[ 标识 ] 0
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
    TgfMsg msg (("michael2"), ("michael2"), ("好好好。"), (1705740724));
    string msg_lcstr = msg.to_locale_string ();
    string expected = R"([ 群組 ] michael2
[ 用戶 ] michael2
[ 訊息 ] 好好好。
[ 時間 ] 2024-01-20 08:52:04 +0000 UTC
[ 標識 ] 0
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
  else
    assert (false);

  tgf::PREFER_LANG = tgf::Lang::zh_HK;
  if (tgf::try_ensure_locale () && tgf::HOST_LANG == tgf::PREFER_LANG)
    {
      test_should_decorate_zh_hk ();
    }
  else
    assert (false);

  tgf::PREFER_LANG = tgf::Lang::zh_CN;
  if (tgf::try_ensure_locale () && tgf::HOST_LANG == tgf::PREFER_LANG)
    {
      test_should_decorate_zh_cn ();
    }
  else
    assert (false);

  return 0;
}
