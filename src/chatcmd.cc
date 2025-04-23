#define __TU__ "chatcmd"

#include "chatcmd.hh"
#include "stat.hh"
#include "posix_regex.hh"
#include "filter.hh"

namespace impl {

tuple<optional<string>, optional<string>, string>
handle_pause (string cmdipt)
{
  ostringstream succdata;
  ostringstream didwhat;
  ostringstream auxmsg;
  tuple<optional<string>, optional<string>, string> ret{nullopt, nullopt, "-"};

  tgfstat::c::d::pause_do_csm_mq.store (true, mo::relaxed);

  succdata << "";
  didwhat << cmdipt;
  auxmsg << "success";

  ret = {succdata.str (), didwhat.str (), auxmsg.str ()};

  return ret;
}

tuple<optional<string>, optional<string>, string>
handle_resume (string cmdipt)
{
  ostringstream succdata;
  ostringstream didwhat;
  ostringstream auxmsg;
  tuple<optional<string>, optional<string>, string> ret{nullopt, nullopt, "-"};

  tgfstat::c::d::pause_do_csm_mq.store (false, mo::relaxed);

  succdata << "";
  didwhat << cmdipt;
  auxmsg << "success";

  ret = {succdata.str (), didwhat.str (), auxmsg.str ()};

  return ret;
}

tuple<optional<string>, optional<string>, string>
handle_rawfilters (string cmdipt, tgf::UserData &p_userdata)
{
  ostringstream succdata;
  ostringstream didwhat;
  ostringstream auxmsg;
  tuple<optional<string>, optional<string>, string> ret{nullopt, nullopt, "-"};

  succdata << "";
  didwhat << cmdipt;
  auxmsg << p_userdata.get_filters ();

  ret = {succdata.str (), didwhat.str (), auxmsg.str ()};

  return ret;
}

tuple<optional<string>, optional<string>, string>
handle_filters (string cmdipt, tgf::UserData &p_userdata)
{
  ostringstream succdata;
  ostringstream didwhat;
  ostringstream auxmsg;
  tuple<optional<string>, optional<string>, string> ret{nullopt, nullopt, "-"};

  string s = p_userdata.get_filters ();
  // TODO: dont use move ctor unless it has been implemented
  tgf::FilterGroupToml fg = tgf::FilterGroupToml (s);

  succdata << "";
  didwhat << cmdipt;
  auxmsg << endl << fg.as_readable ();

  ret = {succdata.str (), didwhat.str (), auxmsg.str ()};

  return ret;
}

tuple<optional<string>, optional<string>, string>
handle_insfilter (string cmdipt, tgf::UserData &p_userdata)
{
  ostringstream succdata;
  ostringstream didwhat;
  ostringstream auxmsg;
  tuple<optional<string>, optional<string>, string> ret{nullopt, nullopt,
							"failed"};
  if (cmdipt.find (CHATCMD_INSF) != 0)
    goto bad_rtn;

  { // extra block to workaround cross-init
    string::size_type begi = 0;
    string::size_type endi = 0;

    u32 filter_id = UINT32_MAX;

    begi = cmdipt.find (CHATCMD_INSF);
    endi = cmdipt.find_first_of (" ", begi);
    if (endi != string::npos)
      {
	begi = cmdipt.find_first_not_of (" ", endi);
	if (begi != string::npos)
	  {
	    endi = cmdipt.find_first_of (" ", begi);
	    string chunk1 = cmdipt.substr (begi, endi - begi);

	    i32 may_filter_id = atoi (chunk1.c_str ());
	    if (may_filter_id <= 0)
	      {
		auxmsg << "invalid " << CHATCMD_INSF_ARG1;
		goto bad_rtn;
	      }
	    filter_id = static_cast<u32> (may_filter_id);
	  }
      }

    string s = p_userdata.get_filters ();
    // TODO: dont use move ctor unless it has been implemented
    tgf::FilterGroupToml fg = tgf::FilterGroupToml (s);
    if (fg.ins_filter (filter_id, nullopt))
      {
	p_userdata.set_filters (fg.as_fsdata ());
	succdata << "";
	didwhat << cmdipt;
	auxmsg << "success";

	ret = {succdata.str (), didwhat.str (), auxmsg.str ()};
	return ret;
      }
    else
      {
	auxmsg << "failed";
	goto bad_rtn;
      }
  }

bad_rtn:
  // succ should be nullopt, aux and didwhat should not
  didwhat << cmdipt << " ";
  auxmsg << endl << endl;
  auxmsg << CHATCMD_INSF_USAGE;
  ret = {nullopt, didwhat.str (), auxmsg.str ()};
  return ret;
}

tuple<optional<string>, optional<string>, string>
handle_rmfilter (string cmdipt, tgf::UserData &p_userdata)
{
  ostringstream succdata;
  ostringstream didwhat;
  ostringstream auxmsg;
  tuple<optional<string>, optional<string>, string> ret{nullopt, nullopt,
							"failed"};
  if (cmdipt.find (CHATCMD_RMF) != 0)
    goto bad_rtn;

  { // extra block to workaround cross-init
    string::size_type begi = 0;
    string::size_type endi = 0;

    begi = cmdipt.find (CHATCMD_RMF);
    endi = cmdipt.find_first_of (" ", begi);
    if (endi == string::npos)
      {
	auxmsg << "need"
	       << " ";
	auxmsg << CHATCMD_RMF_ARG1;
	goto bad_rtn;
      }

    begi = cmdipt.find_first_not_of (" ", endi);
    if (begi == string::npos)
      {
	auxmsg << "need"
	       << " ";
	auxmsg << CHATCMD_RMF_ARG1;
	goto bad_rtn;
      }
    endi = cmdipt.find_first_of (" ", begi);
    string chunk1 = cmdipt.substr (begi, endi - begi);

    u32 filter_id = atoi (chunk1.c_str ());
    if (filter_id <= 0)
      {
	auxmsg << "invalid " << CHATCMD_RMF_ARG1;
	goto bad_rtn;
      }

    string s = p_userdata.get_filters ();
    // TODO: dont use move ctor unless it has been implemented
    tgf::FilterGroupToml fg = tgf::FilterGroupToml (s);
    if (fg.rm_filter (filter_id))
      {
	p_userdata.set_filters (fg.as_fsdata ());
	succdata << "";
	didwhat << cmdipt;
	auxmsg << "success";

	ret = {succdata.str (), didwhat.str (), auxmsg.str ()};
	return ret;
      }
    else
      {
	auxmsg << "failed";
	goto bad_rtn;
      }
  }

bad_rtn:
  // succ should be nullopt, aux and didwhat should not
  didwhat << cmdipt << " ";
  auxmsg << endl << endl;
  auxmsg << CHATCMD_RMF_USAGE;
  ret = {nullopt, didwhat.str (), auxmsg.str ()};
  return ret;
}

tuple<optional<string>, optional<string>, string>
handle_editfilter (string cmdipt, tgf::UserData &p_userdata)
{
  ostringstream succdata;
  ostringstream auxmsg;
  ostringstream didwhat;
  tuple<optional<string>, optional<string>, string> ret{nullopt, nullopt, "-"};

  if (cmdipt.find (CHATCMD_EDITF) != 0)
    goto bad_rtn;

  { // extra block to workaround cross-init
    string::size_type begi = 0;
    string::size_type endi = 0;

    begi = cmdipt.find (CHATCMD_EDITF);
    endi = cmdipt.find_first_of (" ", begi);
    if (endi == string::npos)
      {
	auxmsg << "need"
	       << " ";
	auxmsg << CHATCMD_EDITF_ARG1;
	goto bad_rtn;
      }

    begi = cmdipt.find_first_not_of (" ", endi);
    if (begi == string::npos)
      {
	auxmsg << "need"
	       << " ";
	auxmsg << CHATCMD_EDITF_ARG1;
	goto bad_rtn;
      }
    endi = cmdipt.find_first_of (" ", begi);
    string chunk1 = cmdipt.substr (begi, endi - begi);
    if (endi == string::npos)
      {
	auxmsg << "need"
	       << " ";
	auxmsg << CHATCMD_EDITF_ARG2;
	goto bad_rtn;
      }

    begi = cmdipt.find_first_not_of (" ", endi);
    if (begi == string::npos)
      {
	auxmsg << "need"
	       << " ";
	auxmsg << CHATCMD_EDITF_ARG2;
	goto bad_rtn;
      }
    endi = cmdipt.find_first_of (" ", begi);
    string chunk2 = cmdipt.substr (begi, endi - begi);
    if (endi == string::npos)
      {
	auxmsg << "need"
	       << " ";
	auxmsg << CHATCMD_EDITF_ARG3;
	goto bad_rtn;
      }

    begi = cmdipt.find_first_not_of (" ", endi);
    if (begi == string::npos)
      {
	auxmsg << "need"
	       << " ";
	auxmsg << CHATCMD_EDITF_ARG3;
	goto bad_rtn;
      }
    endi = cmdipt.find_first_of (" ", begi);
    string chunk3 = cmdipt.substr (begi, endi - begi);
    if (endi == string::npos)
      {
	auxmsg << "need"
	       << " ";
	auxmsg << CHATCMD_EDITF_ARG4;
	goto bad_rtn;
      }

    begi = cmdipt.find_first_not_of (" ", endi);
    if (begi == string::npos)
      {
	auxmsg << "need"
	       << " ";
	auxmsg << CHATCMD_EDITF_ARG4;
	goto bad_rtn;
      }
    endi = string::npos;
    string chunk4 = cmdipt.substr (begi, endi - begi);

    u32 filter_id = atoi (chunk1.c_str ());
    if (filter_id <= 0)
      {
	auxmsg << "invalid " << CHATCMD_EDITF_ARG1;
	goto bad_rtn;
      }

    string property = chunk2;
    if (property != "titles" && property != "no-titles"
	&& property != "keywords" && property != "no-keywords"
	&& property != "senders" && property != "no-senders")
      {
	auxmsg << "invalid " << CHATCMD_EDITF_ARG2;
	goto bad_rtn;
      }

    string action = chunk3;
    if (action != "add" && action != "del")
      {
	auxmsg << "invalid " << CHATCMD_EDITF_ARG3;
	goto bad_rtn;
      }

    if (chunk4[0] != '"' || chunk4[chunk4.length () - 1] != '"')
      {
	auxmsg << "invalid " << CHATCMD_EDITF_ARG4;
	goto bad_rtn;
      }
    string unquoted_re = chunk4.substr (1, chunk4.length () - 2);
    PosixExtRegex may_re (unquoted_re);
    if (!may_re.is_pattern_accept ())
      {
	auxmsg << "invalid " << CHATCMD_EDITF_ARG4;
	goto bad_rtn;
      }

    string curr_filters = p_userdata.get_filters ();
    // TODO: dont use move ctor unless it has been implemented
    tgf::FilterGroupToml fg = tgf::FilterGroupToml (curr_filters);
    bool action_res = false;
    if (false)
      ;
    else if (property == "titles" && action == "add")
      action_res = fg.add_titles (filter_id, unquoted_re);
    else if (property == "titles" && action == "del")
      action_res = fg.del_titles (filter_id, unquoted_re);
    else if (property == "no-titles" && action == "add")
      action_res = fg.add_no_titles (filter_id, unquoted_re);
    else if (property == "no-titles" && action == "del")
      action_res = fg.del_no_titles (filter_id, unquoted_re);
    else if (property == "keywords" && action == "add")
      action_res = fg.add_keywords (filter_id, unquoted_re);
    else if (property == "keywords" && action == "del")
      action_res = fg.del_keywords (filter_id, unquoted_re);
    else if (property == "no-keywords" && action == "add")
      action_res = fg.add_no_keywords (filter_id, unquoted_re);
    else if (property == "no-keywords" && action == "del")
      action_res = fg.del_no_keywords (filter_id, unquoted_re);
    else if (property == "senders" && action == "add")
      action_res = fg.add_senders (filter_id, unquoted_re);
    else if (property == "senders" && action == "del")
      action_res = fg.del_senders (filter_id, unquoted_re);
    else if (property == "no-senders" && action == "add")
      action_res = fg.add_no_senders (filter_id, unquoted_re);
    else if (property == "no-senders" && action == "del")
      action_res = fg.del_no_senders (filter_id, unquoted_re);
    else
      {
	auxmsg << "not supported:";
	auxmsg << property;
	auxmsg << action;
	goto bad_rtn;
      }

    if (!action_res)
      {
	auxmsg << "failed: ";
	auxmsg << property;
	auxmsg << action;
	goto bad_rtn;
      }

    p_userdata.set_filters (fg.as_fsdata ());

    succdata << "";
    didwhat << CHATCMD_EDITF << " ";
    didwhat << filter_id << " ";
    didwhat << property << " ";
    didwhat << action << " ";
    didwhat << "\"" << unquoted_re << "\""
	    << " ";
    auxmsg << "success"; // leading spc makes sure wont be
			 // identical to user's cmd
    // auxmsg << endl;
    // auxmsg << fg.as_fsdata (); // fot debug
    // ret = {auxmsg.str (), "-"};
    ret = {succdata.str (), didwhat.str (), auxmsg.str ()};
    return ret;
  }

bad_rtn:
  didwhat << cmdipt << " ";
  auxmsg << endl << endl;
  auxmsg << CHATCMD_EDITF_USAGE;
  ret = {nullopt, didwhat.str (), auxmsg.str ()};
  return ret;
}

} // namespace impl

tgf::ChatCmdHandler::ChatCmdHandler (tgf::ChatCmdType typ, string ipt,
				     tgf::UserData &p_userdata)
{
  ipt.erase (0, ipt.find_first_not_of (" "));
  __aux_msg = string (CHATCMD_RPLY_PREFIX);

  if (false)
    ;
  else if (typ == tgf::ChatCmdType::ChatCmdPause)
    {
      tuple<optional<string>, optional<string>, string> res
	= impl::handle_pause (ipt);
      __succ_data = move (get<0> (res));
      __did_what = move (get<1> (res));
      __aux_msg += move (get<2> (res));
    }
  else if (typ == tgf::ChatCmdType::ChatCmdResume)
    {
      tuple<optional<string>, optional<string>, string> res
	= impl::handle_resume (ipt);
      __succ_data = move (get<0> (res));
      __did_what = move (get<1> (res));
      __aux_msg += move (get<2> (res));
    }
  else if (typ == tgf::ChatCmdType::ChatCmdRawFilters)
    {
      tuple<optional<string>, optional<string>, string> res
	= impl::handle_rawfilters (ipt, p_userdata);
      __succ_data = move (get<0> (res));
      __did_what = move (get<1> (res));
      __aux_msg += move (get<2> (res));
    }
  else if (typ == tgf::ChatCmdType::ChatCmdFilters)
    {
      tuple<optional<string>, optional<string>, string> res
	= impl::handle_filters (ipt, p_userdata);
      __succ_data = move (get<0> (res));
      __did_what = move (get<1> (res));
      __aux_msg += move (get<2> (res));
    }
  else if (typ == tgf::ChatCmdType::ChatCmdEditFilter)
    {
      tuple<optional<string>, optional<string>, string> res
	= impl::handle_editfilter (ipt, p_userdata);
      __succ_data = move (get<0> (res));
      __did_what = move (get<1> (res));
      __aux_msg += move (get<2> (res));
    }
  else if (typ == tgf::ChatCmdType::ChatCmdInsertFilter)
    {
      tuple<optional<string>, optional<string>, string> res
	= impl::handle_insfilter (ipt, p_userdata);
      __succ_data = move (get<0> (res));
      __did_what = move (get<1> (res));
      __aux_msg += move (get<2> (res));
    }
  else if (typ == tgf::ChatCmdType::ChatCmdRemoveFilter)
    {
      tuple<optional<string>, optional<string>, string> res
	= impl::handle_rmfilter (ipt, p_userdata);
      __succ_data = move (get<0> (res));
      __did_what = move (get<1> (res));
      __aux_msg += move (get<2> (res));
    }
}
