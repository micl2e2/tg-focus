#include <assert.h>
#include "std_comp.hh"
#include "chatcmd.hh"
#include "user_data.hh"
#include "stat.hh"

const string PRED_HOME = "/tmp/dummy_home_chatcmd";

tgf::UserData tfdata{make_optional (PRED_HOME), true};

void
tst_editf_failmsg_missingarg1 ()
{
  string usript = "editf";
  string curr_filters = R"(
[[focus-filter]]
title = ".*"
			)";
  tfdata.set_filters (move (curr_filters));
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdEditFilter, usript, tfdata);
  assert (!res.done ());
  assert (res.succ_data () == nullopt);
  assert (res.aux_msg ()
	  == ("〘 TGFCMD 〙need"
	      " " CHATCMD_EDITF_ARG1 "\n\n" CHATCMD_EDITF_USAGE));
}

void
tst_editf_failmsg_missingarg2 ()
{
  string usript = "editf 1";
  string curr_filters = R"(
[[focus-filter]]
title = ".*"
			)";
  tfdata.set_filters (move (curr_filters));
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdEditFilter, usript, tfdata);
  assert (!res.done ());
  assert (res.succ_data () == nullopt);
  // cerr << res.aux_msg () << endl;
  assert (res.aux_msg ()
	  == ("〘 TGFCMD 〙need"
	      " " CHATCMD_EDITF_ARG2 "\n\n" CHATCMD_EDITF_USAGE));
}

void
tst_editf_failmsg_missingarg3 ()
{
  string usript = "editf 1 keywords";
  string curr_filters = R"(
[[focus-filter]]
title = ".*"
			)";
  tfdata.set_filters (move (curr_filters));
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdEditFilter, usript, tfdata);
  assert (!res.done ());
  assert (res.succ_data () == nullopt);
  assert (res.aux_msg ()
	  == ("〘 TGFCMD 〙need"
	      " " CHATCMD_EDITF_ARG3 "\n\n" CHATCMD_EDITF_USAGE));
}

void
tst_editf_failmsg_missingarg4 ()
{
  string usript = "editf 1 keywords add";
  string curr_filters = R"(
[[focus-filter]]
title = ".*"
			)";
  tfdata.set_filters (move (curr_filters));
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdEditFilter, usript, tfdata);
  assert (!res.done ());
  assert (res.succ_data () == nullopt);
  assert (res.aux_msg ()
	  == ("〘 TGFCMD 〙need"
	      " " CHATCMD_EDITF_ARG4 "\n\n" CHATCMD_EDITF_USAGE));
}

void
tst_editf_failmsg_wrongspell ()
{ // actually bc of wrong spell but wont report it, instead, missing arg error
  string usript = "editf 1 keywordx";
  string curr_filters = R"(
[[focus-filter]]
title = ".*"
			)";
  tfdata.set_filters (move (curr_filters));
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdEditFilter, usript, tfdata);
  assert (!res.done ());
  assert (res.succ_data () == nullopt);
  assert (res.aux_msg ()
	  == ("〘 TGFCMD 〙need"
	      " " CHATCMD_EDITF_ARG3 "\n\n" CHATCMD_EDITF_USAGE));
}

void
tst_editf_failmsg_wrongspell2 ()
{
  string usript = "editf 1 keywordx add xxx";
  string curr_filters = R"(
[[focus-filter]]
title = ".*"
			)";
  tfdata.set_filters (move (curr_filters));
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdEditFilter, usript, tfdata);
  assert (!res.done ());
  assert (res.succ_data () == nullopt);
  assert (
    res.aux_msg ()
    == ("〘 TGFCMD 〙invalid " CHATCMD_EDITF_ARG2 "\n\n" CHATCMD_EDITF_USAGE));
}

void
tst_editf_failmsg_wrongspell3 ()
{
  string usript = "editf 1 keywords addx xxx";
  string curr_filters = R"(
[[focus-filter]]
title = ".*"
			)";
  tfdata.set_filters (move (curr_filters));
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdEditFilter, usript, tfdata);
  assert (!res.done ());
  assert (res.succ_data () == nullopt);
  assert (
    res.aux_msg ()
    == ("〘 TGFCMD 〙invalid " CHATCMD_EDITF_ARG3 "\n\n" CHATCMD_EDITF_USAGE));
}

void
tst_editf_failmsg_wrongspell4 ()
{
  string usript = "editf 1 keywords add xxx";
  string curr_filters = R"(
[[focus-filter]]
title = ".*"
			)";
  tfdata.set_filters (move (curr_filters));
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdEditFilter, usript, tfdata);
  assert (!res.done ());
  assert (res.succ_data () == nullopt);
  assert (
    res.aux_msg ()
    == ("〘 TGFCMD 〙invalid " CHATCMD_EDITF_ARG4 "\n\n" CHATCMD_EDITF_USAGE));
}

void
tst_editf_failmsg_wrongspell5 ()
{
  string usript = "editf 1 keywords add \"xxx";
  string curr_filters = R"(
[[focus-filter]]
title = ".*"
			)";
  tfdata.set_filters (move (curr_filters));
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdEditFilter, usript, tfdata);
  assert (!res.done ());
  assert (res.succ_data () == nullopt);
  assert (
    res.aux_msg ()
    == ("〘 TGFCMD 〙invalid " CHATCMD_EDITF_ARG4 "\n\n" CHATCMD_EDITF_USAGE));
}

void
tst_editf_1filter_succmsg ()
{
  string usript = "editf 1 keywords add \"xxx\"";
  string curr_filters = R"(
[[focus-filter]]
title = ".*"
)";
  tfdata.set_filters (move (curr_filters));
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdEditFilter, usript, tfdata);
  assert (res.done ());
  assert (res.aux_msg () == "〘 TGFCMD 〙success");
  assert (res.did_what ().has_value ());
  assert (res.did_what ().value () == "editf 1 keywords add \"xxx\" ");
  assert (res.succ_data ().has_value ());
  // cerr << tfdata.get_filters () << endl;
  assert (tfdata.get_filters () == R"(
[[focus-filter]]
title = ".*"
keywords = ["xxx"]
no-keywords = []
senders = []
no-senders = []
rej-senders = []
)");
}

void
tst_editf_1filter_succmsg2 ()
{
  string usript = "editf 1 rej-senders del \"yyy\"";
  string curr_filters = R"(
[[focus-filter]]
title = ".*"
rej-senders = ["xxx","yyy","zzz"]
)";
  tfdata.set_filters (move (curr_filters));
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdEditFilter, usript, tfdata);
  // cerr << res.aux_msg ();
  // cerr << res.succ_data ();
  assert (res.done ());
  assert (res.aux_msg () == "〘 TGFCMD 〙success");
  assert (res.did_what ().has_value ());
  assert (res.did_what ().value () == "editf 1 rej-senders del \"yyy\" ");
  assert (res.succ_data ().has_value ());
  assert (tfdata.get_filters () == R"(
[[focus-filter]]
title = ".*"
keywords = []
no-keywords = []
senders = []
no-senders = []
rej-senders = ["xxx", "zzz"]
)");
}

void
tst_editf_1filter_succmsg3 ()
// filters is stateless for ChatCmd
{
  string curr_filters = R"(
[[focus-filter]]
title = ".*"
keywords = []
no-keywords = []
senders = []
no-senders = []
rej-senders = ["xxx","yyy","zzz"]
)";
  string curr_filters_cp = curr_filters;
  {
    tfdata.set_filters (move (curr_filters));
    string usript = "editf 1 rej-senders del \"yyy\"";
    tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdEditFilter, usript,
			     tfdata);
    // cerr << res.aux_msg ();
    // cerr << res.succ_data ();
    assert (res.done ());
    assert (res.aux_msg () == "〘 TGFCMD 〙success");
    assert (res.did_what ().has_value ());
    assert (res.did_what ().value () == "editf 1 rej-senders del \"yyy\" ");
    assert (res.succ_data ().has_value ());
    assert (tfdata.get_filters () == R"(
[[focus-filter]]
title = ".*"
keywords = []
no-keywords = []
senders = []
no-senders = []
rej-senders = ["xxx", "zzz"]
)");
  }
  {
    tfdata.set_filters (move (curr_filters_cp));
    string usript = "editf 1 rej-senders add \"ppp\"";
    tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdEditFilter, usript,
			     tfdata);
    // cerr << res.aux_msg ();
    // cerr << res.succ_data ();
    assert (res.done ());
    assert (res.aux_msg () == "〘 TGFCMD 〙success");
    assert (res.did_what ().has_value ());
    assert (res.did_what ().value () == "editf 1 rej-senders add \"ppp\" ");
    assert (res.succ_data ().has_value ());
    assert (tfdata.get_filters () == R"(
[[focus-filter]]
title = ".*"
keywords = []
no-keywords = []
senders = []
no-senders = []
rej-senders = ["xxx", "yyy", "zzz", "ppp"]
)");
  }
}

void
tst_editf_2filter_succmsg4 ()
{
  string usript = "editf 2 keywords add \"yyy\"";
  string curr_filters = R"(
[[focus-filter]]
title = ".*"
keywords = ["xxx"]
)";
  tfdata.set_filters (move (curr_filters));
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdEditFilter, usript, tfdata);
  // cerr << res.succ_data ();
  // cerr << res.did_what ().value ();
  // cerr << res.aux_msg ();
  assert (res.done ());
  assert (res.aux_msg () == "〘 TGFCMD 〙success");
  assert (res.did_what ().has_value ());
  assert (res.did_what ().value () == "editf 2 keywords add \"yyy\" ");
  assert (res.succ_data ().has_value ());
  assert (tfdata.get_filters () == R"(
[[focus-filter]]
title = ".*"
keywords = ["xxx"]
no-keywords = []
senders = []
no-senders = []
rej-senders = []

[[focus-filter]]
title = ".*"
keywords = ["yyy"]
no-keywords = []
senders = []
no-senders = []
rej-senders = []
)");
}

void
tst_editf_2filter_succmsg5 ()
{
  string usript = "editf 9 keywords add \"yyy\"";
  string curr_filters = R"(
[[focus-filter]]
title = ".*"
keywords = ["xxx"]
)";
  tfdata.set_filters (move (curr_filters));
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdEditFilter, usript, tfdata);
  // cerr << res.succ_data ();
  // cerr << res.did_what ().value ();
  // cerr << res.aux_msg ();
  assert (res.done ());
  assert (res.aux_msg () == "〘 TGFCMD 〙success");
  assert (res.did_what ().has_value ());
  assert (res.did_what ().value () == "editf 2 keywords add \"yyy\" ");
  assert (res.succ_data ().has_value ());
  assert (tfdata.get_filters () == R"(
[[focus-filter]]
title = ".*"
keywords = ["xxx"]
no-keywords = []
senders = []
no-senders = []
rej-senders = []

[[focus-filter]]
title = ".*"
keywords = ["yyy"]
no-keywords = []
senders = []
no-senders = []
rej-senders = []
)");
}

void
tst_pause ()
{
  tgfstat::c::d::pause_do_csm_mq.store (false, mo::relaxed);
  assert (!tgfstat::c::d::pause_do_csm_mq.load (mo::relaxed));

  string usript = "   pause";
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdPause, usript, tfdata);
  // cerr << res.succ_data ();
  // cerr << res.did_what ().value ();
  // cerr << res.aux_msg ();
  assert (res.done ());
  assert (res.aux_msg () == "〘 TGFCMD 〙success");
  assert (res.did_what ().has_value ());
  assert (res.did_what ().value () == "pause"); // will trim spc
  assert (res.succ_data ().has_value ());
  assert (tgfstat::c::d::pause_do_csm_mq.load (mo::relaxed));
}

void
tst_resume ()
{
  tgfstat::c::d::pause_do_csm_mq.store (true, mo::relaxed);
  assert (tgfstat::c::d::pause_do_csm_mq.load (mo::relaxed));

  string usript = "   resume";
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdResume, usript, tfdata);
  // cerr << res.succ_data ();
  // cerr << res.did_what ().value ();
  // cerr << res.aux_msg ();
  assert (res.done ());
  assert (res.aux_msg () == "〘 TGFCMD 〙success");
  assert (res.did_what ().has_value ());
  assert (res.did_what ().value () == "resume"); // will trim spc
  assert (res.succ_data ().has_value ());
  assert (!tgfstat::c::d::pause_do_csm_mq.load (mo::relaxed));
}

void
tst_rawfilters ()
{
  string curr_filters = R"(
[[focus-filter]]
title = ".*"
keywords = ["xxx"]
)";
  string curr_filters_cp = curr_filters;
  tfdata.set_filters (move (curr_filters));

  string usript = "   rawfilters";
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdRawFilters, usript, tfdata);
  // cerr << res.succ_data ();
  // cerr << res.did_what ().value ();
  // cerr << res.aux_msg ();
  assert (res.done ());
  assert (res.aux_msg () == string (CHATCMD_RPLY_PREFIX) + curr_filters_cp);
  assert (res.did_what ().has_value ());
  assert (res.did_what ().value () == "rawfilters"); // will trim spc
  assert (res.succ_data ().has_value ());
}

void
tst_readblefilters ()
{
  string curr_filters = R"(
[[focus-filter]]
title = ".*"
keywords = ["xxx","yyy"]
no-keywords = ["zzz","asda ..."]
)";
  string curr_filters_cp = curr_filters;
  tfdata.set_filters (move (curr_filters));

  string usript = "  filters";
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdFilters, usript, tfdata);
  // cerr << res.succ_data ();
  // cerr << res.did_what ().value ();
  cerr << res.aux_msg ();
  assert (res.done ());
  assert (res.aux_msg () == string (CHATCMD_RPLY_PREFIX) + R"(
🞋 🞋 🞋 FILTER 1 🞋 🞋 🞋

🞄 Accept Title 🞄
(.*)

🞄 Accept Words 🞄
(xxx)  (yyy)

🞄 No Words 🞄
(zzz)  (asda ...)

🞄 Accept Senders 🞄

🞄 Reject Senders 🞄

🞄 No Senders 🞄

)");
  assert (res.did_what ().has_value ());
  assert (res.did_what ().value () == "filters"); // will trim spc
  assert (res.succ_data ().has_value ());
}

void
tst_rm_filter_succ ()
{
  string usript = " rmf 1";
  string curr_filters = R"(
[[focus-filter]]
title = ".*"
keywords = ["111"]
no-keywords = []
senders = []
no-senders = []
rej-senders = []

[[focus-filter]]
title = ".*"
keywords = ["222"]
no-keywords = []
senders = []
no-senders = []
rej-senders = []
)";
  tfdata.set_filters (move (curr_filters));
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdRemoveFilter, usript,
			   tfdata);
  assert (res.done ());
  assert (res.aux_msg () == "〘 TGFCMD 〙success");
  assert (res.did_what ().has_value ());
  assert (res.did_what ().value () == "rmf 1");
  assert (res.succ_data ().has_value ());
  // cerr << tfdata.get_filters () << endl;
  assert (tfdata.get_filters () == R"(
[[focus-filter]]
title = ".*"
keywords = ["222"]
no-keywords = []
senders = []
no-senders = []
rej-senders = []
)");
}

void
tst_rm_filter_fail ()
{ // cannot remove if theres only one
  string usript = " rmf 1";
  string curr_filters = R"(
[[focus-filter]]
title = ".*"
keywords = ["111"]
no-keywords = []
senders = []
no-senders = []
rej-senders = []
)";
  tfdata.set_filters (move (curr_filters));
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdRemoveFilter, usript,
			   tfdata);
  assert (!res.done ());
  assert (res.did_what ().has_value ());
  assert (res.did_what ().value () == "rmf 1 ");
  cerr << res.aux_msg () << endl;
  assert (res.aux_msg ()
	  == ("〘 TGFCMD 〙failed"
	      "\n\n" CHATCMD_RMF_USAGE));
  assert (tfdata.get_filters () == R"(
[[focus-filter]]
title = ".*"
keywords = ["111"]
no-keywords = []
senders = []
no-senders = []
rej-senders = []
)");
}

void
tst_rm_filter_fail2 ()
{
  string usript = "rmf";
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdRemoveFilter, usript,
			   tfdata);
  assert (!res.done ());
  assert (res.succ_data () == nullopt);
  assert (res.aux_msg ()
	  == ("〘 TGFCMD 〙need"
	      " " CHATCMD_RMF_ARG1 "\n\n" CHATCMD_RMF_USAGE));
}

void
tst_rm_filter_fail3 ()
{
  string usript = "rmf 0";
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdRemoveFilter, usript,
			   tfdata);
  assert (!res.done ());
  assert (res.succ_data () == nullopt);
  assert (res.aux_msg ()
	  == ("〘 TGFCMD 〙invalid"
	      " " CHATCMD_RMF_ARG1 "\n\n" CHATCMD_RMF_USAGE));
}

int
main ()
{
  tst_editf_failmsg_missingarg1 ();
  tst_editf_failmsg_missingarg2 ();
  tst_editf_failmsg_missingarg3 ();
  tst_editf_failmsg_missingarg4 ();

  tst_editf_failmsg_wrongspell ();
  tst_editf_failmsg_wrongspell2 ();
  tst_editf_failmsg_wrongspell3 ();
  tst_editf_failmsg_wrongspell4 ();
  tst_editf_failmsg_wrongspell5 ();

  tst_editf_1filter_succmsg ();
  tst_editf_1filter_succmsg2 ();
  tst_editf_1filter_succmsg3 ();
  tst_editf_2filter_succmsg4 ();
  tst_editf_2filter_succmsg5 ();

  tst_pause ();
  tst_resume ();

  tst_rawfilters ();
  tst_readblefilters();

  tst_rm_filter_succ ();
  tst_rm_filter_fail ();
  tst_rm_filter_fail2 ();
  tst_rm_filter_fail3 ();

  return 0;
}
