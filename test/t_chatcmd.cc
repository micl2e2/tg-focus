#include <assert.h>
#include "std_comp.hh"
#include "chatcmd.hh"
#include "filter.hh"
#include "user_data.hh"
#include "stat.hh"

const string PRED_HOME = "/tmp/dummy_home_chatcmd";

tgf::UserData tfdata{make_optional (PRED_HOME), true};

void
tst_editf_failmsg_missingarg1 ()
{
  string usript = "filter.edit";
  string curr_filters = R"(
[[focus-filter]]
titles = [".*"]
			)";
  tfdata.set_filters (move (curr_filters));
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdEditFilter, usript, tfdata);
  tgfass (!res.done ());
  tgfass (res.succ_data () == nullopt);
  tgfass (res.aux_msg ()
	  == ("〘 TGFCMD 〙need"
	      " " CHATCMD_EDITF_ARG1 "\n\n" CHATCMD_EDITF_USAGE));
}

void
tst_editf_failmsg_missingarg2 ()
{
  string usript = "filter.edit 1";
  string curr_filters = R"(
[[focus-filter]]
titles = [".*"]
			)";
  tfdata.set_filters (move (curr_filters));
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdEditFilter, usript, tfdata);
  tgfass (!res.done ());
  tgfass (res.succ_data () == nullopt);
  // cerr << res.aux_msg () << endl;
  tgfass (res.aux_msg ()
	  == ("〘 TGFCMD 〙need"
	      " " CHATCMD_EDITF_ARG2 "\n\n" CHATCMD_EDITF_USAGE));
}

void
tst_editf_failmsg_missingarg3 ()
{
  string usript = "filter.edit 1 keywords";
  string curr_filters = R"(
[[focus-filter]]
titles = [".*"]
			)";
  tfdata.set_filters (move (curr_filters));
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdEditFilter, usript, tfdata);
  tgfass (!res.done ());
  tgfass (res.succ_data () == nullopt);
  tgfass (res.aux_msg ()
	  == ("〘 TGFCMD 〙need"
	      " " CHATCMD_EDITF_ARG3 "\n\n" CHATCMD_EDITF_USAGE));
}

void
tst_editf_failmsg_missingarg4 ()
{
  string usript = "filter.edit 1 keywords add";
  string curr_filters = R"(
[[focus-filter]]
titles = [".*"]
			)";
  tfdata.set_filters (move (curr_filters));
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdEditFilter, usript, tfdata);
  tgfass (!res.done ());
  tgfass (res.succ_data () == nullopt);
  tgfass (res.aux_msg ()
	  == ("〘 TGFCMD 〙need"
	      " " CHATCMD_EDITF_ARG4 "\n\n" CHATCMD_EDITF_USAGE));
}

void
tst_editf_failmsg_wrongspell ()
{ // actually bc of wrong spell but wont report it, instead, missing arg error
  string usript = "filter.edit 1 keywordx";
  string curr_filters = R"(
[[focus-filter]]
titles = [".*"]
			)";
  tfdata.set_filters (move (curr_filters));
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdEditFilter, usript, tfdata);
  tgfass (!res.done ());
  tgfass (res.succ_data () == nullopt);
  tgfass (res.aux_msg ()
	  == ("〘 TGFCMD 〙need"
	      " " CHATCMD_EDITF_ARG3 "\n\n" CHATCMD_EDITF_USAGE));
}

void
tst_editf_failmsg_wrongspell2 ()
{
  string usript = "filter.edit 1 keywordx add xxx";
  string curr_filters = R"(
[[focus-filter]]
titles = [".*"]
			)";
  tfdata.set_filters (move (curr_filters));
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdEditFilter, usript, tfdata);
  tgfass (!res.done ());
  tgfass (res.succ_data () == nullopt);
  tgfass (
    res.aux_msg ()
    == ("〘 TGFCMD 〙invalid " CHATCMD_EDITF_ARG2 "\n\n" CHATCMD_EDITF_USAGE));
}

void
tst_editf_failmsg_wrongspell3 ()
{
  string usript = "filter.edit 1 keywords addx xxx";
  string curr_filters = R"(
[[focus-filter]]
titles = [".*"]
			)";
  tfdata.set_filters (move (curr_filters));
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdEditFilter, usript, tfdata);
  tgfass (!res.done ());
  tgfass (res.succ_data () == nullopt);
  tgfass (
    res.aux_msg ()
    == ("〘 TGFCMD 〙invalid " CHATCMD_EDITF_ARG3 "\n\n" CHATCMD_EDITF_USAGE));
}

void
tst_editf_failmsg_wrongspell4 ()
{
  string usript = "filter.edit 1 keywords add xxx";
  string curr_filters = R"(
[[focus-filter]]
titles = [".*"]
			)";
  tfdata.set_filters (move (curr_filters));
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdEditFilter, usript, tfdata);
  tgfass (!res.done ());
  tgfass (res.succ_data () == nullopt);
  tgfass (
    res.aux_msg ()
    == ("〘 TGFCMD 〙invalid " CHATCMD_EDITF_ARG4 "\n\n" CHATCMD_EDITF_USAGE));
}

void
tst_editf_failmsg_wrongspell5 ()
{
  string usript = "filter.edit 1 keywords add \"xxx";
  string curr_filters = R"(
[[focus-filter]]
titles = [".*"]
			)";
  tfdata.set_filters (move (curr_filters));
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdEditFilter, usript, tfdata);
  tgfass (!res.done ());
  tgfass (res.succ_data () == nullopt);
  tgfass (
    res.aux_msg ()
    == ("〘 TGFCMD 〙invalid " CHATCMD_EDITF_ARG4 "\n\n" CHATCMD_EDITF_USAGE));
}

void
tst_editf_1filter_badre ()
{
  string usript = "filter.edit 1 keywords add \")(\"";
  string curr_filters = R"(
[[focus-filter]]
)";
  tfdata.set_filters (move (curr_filters));
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdEditFilter, usript, tfdata);
  tgfass (!res.done ());
  // cerr << res.aux_msg () << endl;
  tgfass (res.aux_msg ()
	  == ("〘 TGFCMD 〙invalid"
	      " " CHATCMD_EDITF_ARG4 "\n\n" CHATCMD_EDITF_USAGE));
  tgfass (res.did_what ().has_value ());
  tgfass (res.did_what ().value () == "filter.edit 1 keywords add \")(\" ");
  tgfass (!res.succ_data ().has_value ());
  // cerr << tfdata.get_filters () << endl;
  tgfass (tfdata.get_filters () == R"(
[[focus-filter]]
)");
}

void
tst_editf_1filter_succ_re_reserved_symbol_escape ()
{
  string usript = "filter.edit 1 titles add \"c\\+\\+\"";
  string curr_filters = R"(
[[focus-filter]]
)";
  tfdata.set_filters (move (curr_filters));
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdEditFilter, usript, tfdata);
  tgfass (res.done ());
  tgfass (res.aux_msg () == "〘 TGFCMD 〙success");
  tgfass (res.did_what ().has_value ());
  tgfass (res.did_what ().value () == R"(filter.edit 1 titles add "c\+\+" )");
  tgfass (res.succ_data ().has_value ());
  cerr << tfdata.get_filters () << endl;
  tgfass (tfdata.get_filters () == R"(
[[focus-filter]]
titles = ["c\\+\\+"]
no-titles = []
senders = []
no-senders = []
keywords = []
no-keywords = []
)");
  tgf::FilterGroupToml fg (tfdata.get_filters ());
  cerr << fg.as_fsdata () << endl;
  tgfass (fg.as_fsdata () == R"(
[[focus-filter]]
titles = ["c\\+\\+"]
no-titles = []
senders = []
no-senders = []
keywords = []
no-keywords = []
)");
  {
    tgf::TgMsg msg (""s, ""s, ""s);
    tgfass (!fg.mtch_tgmsg (msg));
  }
  {
    tgf::TgMsg msg ("c+"s, ""s, ""s);
    tgfass (!fg.mtch_tgmsg (msg));
  }
  {
    tgf::TgMsg msg ("cc"s, ""s, ""s);
    tgfass (!fg.mtch_tgmsg (msg));
  }
  {
    tgf::TgMsg msg ("c++"s, ""s, ""s);
    tgfass (fg.mtch_tgmsg (msg));
  }
}

void
tst_editf_1filter_succmsg ()
{
  string usript = "filter.edit 1 keywords add \"xxx\"";
  string curr_filters = R"(
[[focus-filter]]
titles = [".*"]
)";
  tfdata.set_filters (move (curr_filters));
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdEditFilter, usript, tfdata);
  tgfass (res.done ());
  tgfass (res.aux_msg () == "〘 TGFCMD 〙success");
  tgfass (res.did_what ().has_value ());
  tgfass (res.did_what ().value () == "filter.edit 1 keywords add \"xxx\" ");
  tgfass (res.succ_data ().has_value ());
  // cerr << tfdata.get_filters () << endl;
  tgfass (tfdata.get_filters () == R"(
[[focus-filter]]
titles = [".*"]
no-titles = []
senders = []
no-senders = []
keywords = ["xxx"]
no-keywords = []
)");
}

void
tst_editf_1filter_succmsg2 ()
{
  string usript = "filter.edit 1 no-senders del \"yyy\"";
  string curr_filters = R"(
[[focus-filter]]
titles = [".*"]
no-senders = ["xxx","yyy","zzz"]
)";
  tfdata.set_filters (move (curr_filters));
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdEditFilter, usript, tfdata);
  // cerr << res.aux_msg ();
  // cerr << res.succ_data ();
  tgfass (res.done ());
  tgfass (res.aux_msg () == "〘 TGFCMD 〙success");
  tgfass (res.did_what ().has_value ());
  tgfass (res.did_what ().value () == "filter.edit 1 no-senders del \"yyy\" ");
  tgfass (res.succ_data ().has_value ());
  tgfass (tfdata.get_filters () == R"(
[[focus-filter]]
titles = [".*"]
no-titles = []
senders = []
no-senders = ["xxx", "zzz"]
keywords = []
no-keywords = []
)");
}

void
tst_editf_1filter_succmsg3 ()
// filters is stateless for ChatCmd
{
  string curr_filters = R"(
[[focus-filter]]
titles = [".*"]
keywords = []
no-keywords = []
senders = []
no-senders = ["xxx","yyy","zzz"]
)";
  string curr_filters_cp = curr_filters;
  {
    tfdata.set_filters (move (curr_filters));
    string usript = "filter.edit 1 no-senders del \"yyy\"";
    tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdEditFilter, usript,
			     tfdata);
    // cerr << res.aux_msg ();
    // cerr << res.succ_data ();
    tgfass (res.done ());
    tgfass (res.aux_msg () == "〘 TGFCMD 〙success");
    tgfass (res.did_what ().has_value ());
    tgfass (res.did_what ().value ()
	    == "filter.edit 1 no-senders del \"yyy\" ");
    tgfass (res.succ_data ().has_value ());
    tgfass (tfdata.get_filters () == R"(
[[focus-filter]]
titles = [".*"]
no-titles = []
senders = []
no-senders = ["xxx", "zzz"]
keywords = []
no-keywords = []
)");
  }
  {
    tfdata.set_filters (move (curr_filters_cp));
    string usript = "filter.edit 1 no-senders add \"ppp\"";
    tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdEditFilter, usript,
			     tfdata);
    // cerr << res.aux_msg ();
    // cerr << res.succ_data ();
    tgfass (res.done ());
    tgfass (res.aux_msg () == "〘 TGFCMD 〙success");
    tgfass (res.did_what ().has_value ());
    tgfass (res.did_what ().value ()
	    == "filter.edit 1 no-senders add \"ppp\" ");
    tgfass (res.succ_data ().has_value ());
    tgfass (tfdata.get_filters () == R"(
[[focus-filter]]
titles = [".*"]
no-titles = []
senders = []
no-senders = ["xxx", "yyy", "zzz", "ppp"]
keywords = []
no-keywords = []
)");
  }
}

void
tst_editf_2filter_succmsg4 ()
{
  string usript = "filter.edit 2 keywords add \"yyy\"";
  string curr_filters = R"(
[[focus-filter]]
titles = [".*"]
keywords = ["xxx"]
)";
  tfdata.set_filters (move (curr_filters));
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdEditFilter, usript, tfdata);
  // cerr << res.succ_data ();
  // cerr << res.did_what ().value ();
  // cerr << res.aux_msg ();
  tgfass (res.done ());
  tgfass (res.aux_msg () == "〘 TGFCMD 〙success");
  tgfass (res.did_what ().has_value ());
  tgfass (res.did_what ().value () == "filter.edit 2 keywords add \"yyy\" ");
  tgfass (res.succ_data ().has_value ());
  // cerr << tfdata.get_filters ();
  tgfass (tfdata.get_filters () == R"(
[[focus-filter]]
titles = [".*"]
no-titles = []
senders = []
no-senders = []
keywords = ["xxx"]
no-keywords = []

[[focus-filter]]
titles = []
no-titles = []
senders = []
no-senders = []
keywords = ["yyy"]
no-keywords = []
)");
}

void
tst_editf_2filter_succmsg5 ()
{
  string usript = "filter.edit 9 keywords add \"yyy\"";
  string curr_filters = R"(
[[focus-filter]]
titles = [".*"]
keywords = ["xxx"]
)";
  tfdata.set_filters (move (curr_filters));
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdEditFilter, usript, tfdata);
  // cerr << res.succ_data ();
  // cerr << res.did_what ().value ();
  // cerr << res.aux_msg ();
  tgfass (res.done ());
  tgfass (res.aux_msg () == "〘 TGFCMD 〙success");
  tgfass (res.did_what ().has_value ());
  tgfass (res.did_what ().value () == "filter.edit 2 keywords add \"yyy\" ");
  tgfass (res.succ_data ().has_value ());
  tgfass (tfdata.get_filters () == R"(
[[focus-filter]]
titles = [".*"]
no-titles = []
senders = []
no-senders = []
keywords = ["xxx"]
no-keywords = []

[[focus-filter]]
titles = []
no-titles = []
senders = []
no-senders = []
keywords = ["yyy"]
no-keywords = []
)");
}

void
tst_editf_1filter_succmsg6 ()
{
  string usript = "filter.edit 1 keywords add \"C++\"";
  string curr_filters = R"(
[[focus-filter]]
)";
  tfdata.set_filters (move (curr_filters));
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdEditFilter, usript, tfdata);
  tgfass (res.done ());
  tgfass (res.aux_msg () == "〘 TGFCMD 〙success");
  tgfass (res.did_what ().has_value ());
  tgfass (res.did_what ().value () == "filter.edit 1 keywords add \"C++\" ");
  tgfass (res.succ_data ().has_value ());
  string fg_fsdata = tfdata.get_filters ();
  // cerr << tfdata.get_filters () << endl;
  tgfass (fg_fsdata == R"(
[[focus-filter]]
titles = []
no-titles = []
senders = []
no-senders = []
keywords = ["C++"]
no-keywords = []
)");

  tgf::FilterGroupToml fg = tgf::FilterGroupToml (tfdata.get_filters ());
  cerr << fg.as_fsdata () << endl;
  tgfass (fg.as_fsdata () == R"(
[[focus-filter]]
titles = []
no-titles = []
senders = []
no-senders = []
keywords = ["C++"]
no-keywords = []
)");
}

void
tst_editf_2filter_succ_filters_pos_are_nonreversed ()
{
  string usript = "filter.edit 9 no-senders add \"zzz\"";
  string curr_filters = R"(
[[focus-filter]]
keywords = ["yyy"]

[[focus-filter]]
keywords = ["xxx"]
)";
  tfdata.set_filters (move (curr_filters));
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdEditFilter, usript, tfdata);
  // cerr << res.succ_data ();
  // cerr << res.did_what ().value ();
  // cerr << res.aux_msg ();
  tgfass (res.done ());
  tgfass (res.aux_msg () == "〘 TGFCMD 〙success");
  tgfass (res.did_what ().has_value ());
  tgfass (res.did_what ().value () == "filter.edit 3 no-senders add \"zzz\" ");
  tgfass (res.succ_data ().has_value ());
  cerr << "|" << tfdata.get_filters () << "|";
  tgfass (tfdata.get_filters () == R"(
[[focus-filter]]
titles = []
no-titles = []
senders = []
no-senders = []
keywords = ["yyy"]
no-keywords = []

[[focus-filter]]
titles = []
no-titles = []
senders = []
no-senders = []
keywords = ["xxx"]
no-keywords = []

[[focus-filter]]
titles = []
no-titles = []
senders = []
no-senders = ["zzz"]
keywords = []
no-keywords = []
)");
}

void
tst_editf_how_to_block_sometitle ()
{
  string usript = "filter.edit 2 no-titles add \"aaa\"";
  string curr_filters = R"(
[[focus-filter]]
)";
  tfdata.set_filters (move (curr_filters));
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdEditFilter, usript, tfdata);
  // cerr << res.succ_data ();
  // cerr << res.did_what ().value ();
  // cerr << res.aux_msg ();
  tgfass (res.done ());
  tgfass (res.aux_msg () == "〘 TGFCMD 〙success");
  tgfass (res.did_what ().has_value ());
  tgfass (res.did_what ().value () == "filter.edit 2 no-titles add \"aaa\" ");
  tgfass (res.succ_data ().has_value ());
  // cerr << "|" << tfdata.get_filters ()<<"|";
  tgfass (tfdata.get_filters () == R"(
[[focus-filter]]
titles = []
no-titles = []
senders = []
no-senders = []
keywords = []
no-keywords = []

[[focus-filter]]
titles = []
no-titles = ["aaa"]
senders = []
no-senders = []
keywords = []
no-keywords = []
)");
}

void
tst_pause ()
{
  tgfstat::c::d::pause_do_csm_mq.store (false, mo::relaxed);
  tgfass (!tgfstat::c::d::pause_do_csm_mq.load (mo::relaxed));

  string usript = "   pause";
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdPause, usript, tfdata);
  // cerr << res.succ_data ();
  // cerr << res.did_what ().value ();
  // cerr << res.aux_msg ();
  tgfass (res.done ());
  tgfass (res.aux_msg () == "〘 TGFCMD 〙success");
  tgfass (res.did_what ().has_value ());
  tgfass (res.did_what ().value () == "pause"); // will trim spc
  tgfass (res.succ_data ().has_value ());
  tgfass (tgfstat::c::d::pause_do_csm_mq.load (mo::relaxed));
}

void
tst_resume ()
{
  tgfstat::c::d::pause_do_csm_mq.store (true, mo::relaxed);
  tgfass (tgfstat::c::d::pause_do_csm_mq.load (mo::relaxed));

  string usript = "   resume";
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdResume, usript, tfdata);
  // cerr << res.succ_data ();
  // cerr << res.did_what ().value ();
  // cerr << res.aux_msg ();
  tgfass (res.done ());
  tgfass (res.aux_msg () == "〘 TGFCMD 〙success");
  tgfass (res.did_what ().has_value ());
  tgfass (res.did_what ().value () == "resume"); // will trim spc
  tgfass (res.succ_data ().has_value ());
  tgfass (!tgfstat::c::d::pause_do_csm_mq.load (mo::relaxed));
}

void
tst_rawfilters ()
{
  string curr_filters = R"(
[[focus-filter]]
titles = [".*"]
keywords = ["xxx"]
)";
  string curr_filters_cp = curr_filters;
  tfdata.set_filters (move (curr_filters));

  string usript = "   rawfilters";
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdRawFilters, usript, tfdata);
  // cerr << res.succ_data ();
  // cerr << res.did_what ().value ();
  // cerr << res.aux_msg ();
  tgfass (res.done ());
  tgfass (res.aux_msg () == string (CHATCMD_RPLY_PREFIX) + curr_filters_cp);
  tgfass (res.did_what ().has_value ());
  tgfass (res.did_what ().value () == "rawfilters"); // will trim spc
  tgfass (res.succ_data ().has_value ());
}

void
tst_rawfilters2 ()
{
  string curr_filters = R"(
[[focus-filter]]
titles = [".*"]
keywords = ["xxx"]
[[focus-filter]]
titles = [".*"]
keywords = ["yyy"]
)";
  string curr_filters_cp = curr_filters;
  tfdata.set_filters (move (curr_filters));

  string usript = "   rawfilters";
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdRawFilters, usript, tfdata);
  // cerr << res.succ_data ();
  // cerr << res.did_what ().value ();
  // cerr << res.aux_msg ();
  tgfass (res.done ());
  tgfass (res.aux_msg () == string (CHATCMD_RPLY_PREFIX) + curr_filters_cp);
  tgfass (res.did_what ().has_value ());
  tgfass (res.did_what ().value () == "rawfilters"); // will trim spc
  tgfass (res.succ_data ().has_value ());
}

void
tst_readblefilters ()
{
  string curr_filters = R"(
[[focus-filter]]
titles = [".*"]
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
  tgfass (res.done ());
  tgfass (res.aux_msg () == string (CHATCMD_RPLY_PREFIX) + R"(

🞆 🞆 🞆 🞆 🞆 FILTER 1 🞆 🞆 🞆 🞆 🞆
🞆
🞆 Titles <titles>
⤷ (.*)
🞆 Senders <senders>
⤷ 
🞆 Keywords <keywords>
⤷ (xxx)  (yyy)
🞆 NO Titles <no-titles>
⤷ 
🞆 NO Senders <no-senders>
⤷ 
🞆 NO Keywords <no-keywords>
⤷ (zzz)  (asda ...)
)");
  tgfass (res.did_what ().has_value ());
  tgfass (res.did_what ().value () == "filters"); // will trim spc
  tgfass (res.succ_data ().has_value ());
}

void
tst_readblefilters_nonreverse_order ()
{
  string curr_filters = R"(
[[focus-filter]]
titles = [".*"]
keywords = ["xxx","yyy"]
no-keywords = ["zzz","asda ..."]

[[focus-filter]]
titles = []
)";
  string curr_filters_cp = curr_filters;
  tfdata.set_filters (move (curr_filters));

  string usript = "  filters";
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdFilters, usript, tfdata);
  // cerr << res.succ_data ();
  // cerr << res.did_what ().value ();
  // cerr << res.aux_msg ();
  tgfass (res.done ());
  tgfass (res.aux_msg () == string (CHATCMD_RPLY_PREFIX) + R"(

🞆 🞆 🞆 🞆 🞆 FILTER 1 🞆 🞆 🞆 🞆 🞆
🞆
🞆 Titles <titles>
⤷ (.*)
🞆 Senders <senders>
⤷ 
🞆 Keywords <keywords>
⤷ (xxx)  (yyy)
🞆 NO Titles <no-titles>
⤷ 
🞆 NO Senders <no-senders>
⤷ 
🞆 NO Keywords <no-keywords>
⤷ (zzz)  (asda ...)

🞆 🞆 🞆 🞆 🞆 FILTER 2 🞆 🞆 🞆 🞆 🞆
🞆
🞆 Titles <titles>
⤷ 
🞆 Senders <senders>
⤷ 
🞆 Keywords <keywords>
⤷ 
🞆 NO Titles <no-titles>
⤷ 
🞆 NO Senders <no-senders>
⤷ 
🞆 NO Keywords <no-keywords>
⤷ 
)");
  tgfass (res.did_what ().has_value ());
  tgfass (res.did_what ().value () == "filters"); // will trim spc
  tgfass (res.succ_data ().has_value ());
}

void
tst_filter_insert_succ ()
{
  string usript = " filter.insert 1";
  string curr_filters = R"(
[[focus-filter]]
titles = [".*"]
keywords = ["111"]
no-keywords = []
senders = []
no-senders = []
)";
  tfdata.set_filters (move (curr_filters));
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdInsertFilter, usript,
			   tfdata);
  // cerr << res.aux_msg () << endl;
  tgfass (res.done ());
  tgfass (res.aux_msg () == "〘 TGFCMD 〙success");
  tgfass (res.did_what ().has_value ());
  tgfass (res.did_what ().value () == "filter.insert 1");
  tgfass (res.succ_data ().has_value ());
  cerr << tfdata.get_filters () << endl;
  tgfass (tfdata.get_filters () == R"(
[[focus-filter]]
titles = []
no-titles = []
senders = []
no-senders = []
keywords = ["tg-focus"]
no-keywords = []

[[focus-filter]]
titles = [".*"]
no-titles = []
senders = []
no-senders = []
keywords = ["111"]
no-keywords = []
)");
}

void
tst_filter_insert_succ2 ()
{
  string usript = " filter.insert 2";
  string curr_filters = R"(
[[focus-filter]]
titles = [".*"]
keywords = ["111"]
no-keywords = []
senders = []
no-senders = []
)";
  tfdata.set_filters (move (curr_filters));
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdInsertFilter, usript,
			   tfdata);
  // cerr << res.aux_msg () << endl;
  tgfass (res.done ());
  tgfass (res.aux_msg () == "〘 TGFCMD 〙success");
  tgfass (res.did_what ().has_value ());
  tgfass (res.did_what ().value () == "filter.insert 2");
  tgfass (res.succ_data ().has_value ());
  cerr << tfdata.get_filters () << endl;
  tgfass (tfdata.get_filters () == R"(
[[focus-filter]]
titles = [".*"]
no-titles = []
senders = []
no-senders = []
keywords = ["111"]
no-keywords = []

[[focus-filter]]
titles = []
no-titles = []
senders = []
no-senders = []
keywords = ["tg-focus"]
no-keywords = []
)");
}

void
tst_filter_insert_succ3 ()
{
  string usript = " filter.insert 5";
  string curr_filters = R"(
[[focus-filter]]
titles = [".*"]
keywords = ["111"]
no-keywords = []
senders = []
no-senders = []
)";
  tfdata.set_filters (move (curr_filters));
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdInsertFilter, usript,
			   tfdata);
  // cerr << res.aux_msg () << endl;
  tgfass (res.done ());
  tgfass (res.aux_msg () == "〘 TGFCMD 〙success");
  tgfass (res.did_what ().has_value ());
  tgfass (res.did_what ().value () == "filter.insert 5");
  tgfass (res.succ_data ().has_value ());
  cerr << tfdata.get_filters () << endl;
  tgfass (tfdata.get_filters () == R"(
[[focus-filter]]
titles = [".*"]
no-titles = []
senders = []
no-senders = []
keywords = ["111"]
no-keywords = []

[[focus-filter]]
titles = []
no-titles = []
senders = []
no-senders = []
keywords = ["tg-focus"]
no-keywords = []
)");
}

void
tst_filter_insert_succ4 ()
{
  string usript = " filter.insert 2";
  string curr_filters = R"(
[[focus-filter]]
titles = ["111"]
keywords = ["111"]
no-keywords = []
senders = []
no-senders = []

[[focus-filter]]
titles = ["222"]
keywords = ["222"]
no-keywords = []
senders = []
no-senders = []

[[focus-filter]]
titles = ["333"]
keywords = ["333"]
no-keywords = []
senders = []
no-senders = []
)";
  tfdata.set_filters (move (curr_filters));
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdInsertFilter, usript,
			   tfdata);
  // cerr << res.aux_msg () << endl;
  tgfass (res.done ());
  tgfass (res.aux_msg () == "〘 TGFCMD 〙success");
  tgfass (res.did_what ().has_value ());
  tgfass (res.did_what ().value () == "filter.insert 2");
  tgfass (res.succ_data ().has_value ());
  cerr << tfdata.get_filters () << endl;
  tgfass (tfdata.get_filters () == R"(
[[focus-filter]]
titles = ["111"]
no-titles = []
senders = []
no-senders = []
keywords = ["111"]
no-keywords = []

[[focus-filter]]
titles = []
no-titles = []
senders = []
no-senders = []
keywords = ["tg-focus"]
no-keywords = []

[[focus-filter]]
titles = ["222"]
no-titles = []
senders = []
no-senders = []
keywords = ["222"]
no-keywords = []

[[focus-filter]]
titles = ["333"]
no-titles = []
senders = []
no-senders = []
keywords = ["333"]
no-keywords = []
)");
}

void
tst_filter_insert_fail ()
{
  string usript = "filter.insert 0";
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdInsertFilter, usript,
			   tfdata);
  tgfass (!res.done ());
  tgfass (res.succ_data () == nullopt);
  tgfass (res.aux_msg ()
	  == ("〘 TGFCMD 〙invalid"
	      " " CHATCMD_INSF_ARG1 "\n\n" CHATCMD_INSF_USAGE));
}

void
tst_filter_insert_fail2 ()
{
  string usript = "filter.insert -1";
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdInsertFilter, usript,
			   tfdata);
  tgfass (!res.done ());
  tgfass (res.succ_data () == nullopt);
  tgfass (res.aux_msg ()
	  == ("〘 TGFCMD 〙invalid"
	      " " CHATCMD_INSF_ARG1 "\n\n" CHATCMD_INSF_USAGE));
}

void
tst_rm_filter_succ ()
{
  string usript = " filter.remove 1";
  string curr_filters = R"(
[[focus-filter]]
titles = [".*"]
keywords = ["111"]
no-keywords = []
senders = []
no-senders = []

[[focus-filter]]
titles = [".*"]
keywords = ["222"]
no-keywords = []
senders = []
no-senders = []
)";
  tfdata.set_filters (move (curr_filters));
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdRemoveFilter, usript,
			   tfdata);
  tgfass (res.done ());
  tgfass (res.aux_msg () == "〘 TGFCMD 〙success");
  tgfass (res.did_what ().has_value ());
  tgfass (res.did_what ().value () == "filter.remove 1");
  tgfass (res.succ_data ().has_value ());
  cerr << tfdata.get_filters () << endl;
  tgfass (tfdata.get_filters () == R"(
[[focus-filter]]
titles = [".*"]
no-titles = []
senders = []
no-senders = []
keywords = ["222"]
no-keywords = []
)");
}

void
tst_rm_filter_fail ()
{ // cannot remove if theres only one
  string usript = " filter.remove 1";
  string curr_filters = R"(
[[focus-filter]]
titles = [".*"]
keywords = ["111"]
no-keywords = []
senders = []
no-senders = []
)";
  tfdata.set_filters (move (curr_filters));
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdRemoveFilter, usript,
			   tfdata);
  tgfass (!res.done ());
  tgfass (res.did_what ().has_value ());
  tgfass (res.did_what ().value () == "filter.remove 1 ");
  tgfass (res.aux_msg ()
	  == ("〘 TGFCMD 〙failed"
	      "\n\n" CHATCMD_RMF_USAGE));
  cerr << tfdata.get_filters () << endl;
  // set/get filters are just raw IO operations
  tgfass (tfdata.get_filters () == R"(
[[focus-filter]]
titles = [".*"]
keywords = ["111"]
no-keywords = []
senders = []
no-senders = []
)");
}

void
tst_rm_filter_fail2 ()
{
  string usript = "filter.remove";
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdRemoveFilter, usript,
			   tfdata);
  tgfass (!res.done ());
  tgfass (res.succ_data () == nullopt);
  tgfass (res.aux_msg ()
	  == ("〘 TGFCMD 〙need"
	      " " CHATCMD_RMF_ARG1 "\n\n" CHATCMD_RMF_USAGE));
}

void
tst_rm_filter_fail3 ()
{
  string usript = "filter.remove 0";
  tgf::ChatCmdHandler res (tgf::ChatCmdType::ChatCmdRemoveFilter, usript,
			   tfdata);
  tgfass (!res.done ());
  tgfass (res.succ_data () == nullopt);
  tgfass (res.aux_msg ()
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

  tst_editf_1filter_badre ();
  tst_editf_1filter_succmsg ();
  tst_editf_1filter_succmsg2 ();
  tst_editf_1filter_succmsg3 ();
  tst_editf_2filter_succmsg4 ();
  tst_editf_2filter_succmsg5 ();
  tst_editf_1filter_succmsg6 ();
  tst_editf_2filter_succ_filters_pos_are_nonreversed ();
  tst_editf_how_to_block_sometitle ();

  tst_pause ();
  tst_resume ();

  tst_rawfilters ();
  tst_rawfilters2 ();
  tst_readblefilters ();
  tst_readblefilters_nonreverse_order ();

  tst_filter_insert_succ ();
  tst_filter_insert_succ2 ();
  tst_filter_insert_succ3 ();
  tst_filter_insert_succ4 ();

  tst_filter_insert_fail ();
  tst_filter_insert_fail2 ();

  tst_rm_filter_succ ();
  tst_rm_filter_fail ();
  tst_rm_filter_fail2 ();
  tst_rm_filter_fail3 ();

  tst_editf_1filter_succ_re_reserved_symbol_escape ();

  return 0;
}
