#include <cassert>
#include <iostream>

#include "posix_regex.hh"
#include "std_comp.hh"

using namespace std;

void
ptn_should_match (string ptn, string str, long lineno)
{
  PosixExtRegex re (ptn);

  if (auto flag = re.is_match (str))
    {
      if (*flag)
	tgfass (true);
      else
	{
	  cerr << "L" << lineno << ", "
	       << "EXPECT match" << endl;
	  tgfass (false);
	}
    }
  else
    {
      cerr << "L" << lineno << ", "
	   << "EXPECT no errors" << endl;
      tgfass (false);
    }
}

void
ptn_should_not_match (string ptn, string str, long lineno)
{
  PosixExtRegex re (ptn);

  if (auto flag = re.is_match (str))
    {
      if (*flag)
	{
	  cerr << "L" << lineno << ", "
	       << "EXPECT not match" << endl;
	  tgfass (false);
	}
      else
	tgfass (true);
    }
  else
    {
      cerr << "L" << lineno << ", "
	   << "EXPECT no errors" << endl;
      tgfass (false);
    }
}

void
ptn_invalid (string &&ptn, long lineno)
{
  PosixExtRegex re (ptn);

  if (auto flag = re.is_match ("???"))
    {
      cerr << "L" << lineno << ", "
	   << "EXPECT invalid pattern" << endl;
      tgfass (false);
    }
  else
    tgfass (true);

  tgfass (!re.is_pattern_accept ());
  tgfass (re.get_err ().length () > 0);
  cerr << "L" << lineno << ", "
       << "posix re error msg: " << re.get_err ();
}

#define assert_ptn_invalid(arg1) ptn_invalid (arg1, __LINE__)

#define assert_match(arg1, arg2) ptn_should_match (arg1, arg2, __LINE__)

#define assert_not_match(arg1, arg2) ptn_should_not_match (arg1, arg2, __LINE__)

int
main ()
{
  tgfass (!std::is_copy_constructible_v<PosixExtRegex>);
  tgfass (!std::is_copy_assignable_v<PosixExtRegex>);
  tgfass (std::is_move_constructible_v<PosixExtRegex>);
  tgfass (std::is_move_assignable_v<PosixExtRegex>);

  assert_ptn_invalid (")(");

  assert_match ("C++", "C++");
  assert_match ("C++", "C+");
  assert_not_match ("C+{2}", "C+");

  assert_match ("x", "xxx");
  assert_match ("x{2}", "xxx");
  assert_match ("x{3}", "xxx");
  assert_match (".*x.*", "xxx");
  assert_not_match ("x{4}", "xxx");
  assert_match ("[a-z0-9]+", "a1b2");

  assert_match ("😺", "😺");
  assert_match ("😺😺", "😺😺");
  assert_match ("🐶", "😺🐶😺");
  assert_not_match ("😺{2}", "😺😺");
  assert_match ("(😺){2}", "😺😺"); // good if parenthesized
  assert_match ("(😺){3}", "😺😺😺");
  assert_match ("(😺){3}", "😺😺😺😺");
  assert_not_match ("😺{2}", "😺");
  assert_not_match ("好{2}", "好好");
  assert_match ("(好){2}", "好好");
  assert_match ("好好", "好好");
  assert_match ("好.*好", "好不好");
}
