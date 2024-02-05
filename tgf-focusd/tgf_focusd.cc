#include <iostream>
#include <thread>
#include <string.h>

#include "lv_log.hh"
#include "state.hh"
#include "worker.hh"
#include "tgf_locale.hh"

void
print_usage (char *argv[])
{
  printf (R"(TG-Focus Daemon

Usage: %s <options>

Available Options:
  --help           Print this message
  --verbose        Turn on verbose output
)",
	  argv[0]);
}

bool
handle_opts (int argc, char *argv[])
{
  bool ret = false;
  if (argc > 1)
    {
      for (int i = 1; i < argc; i++)
	{
	  if (strcmp (argv[i], "--verbose") == 0)
	    {
	      g_log_lv = LogLv::DEBUG;
	    }
	  else if (strcmp (argv[i], "--help") == 0)
	    {
	      print_usage (argv);
	      ret = true;
	      break;
	    }
	}
    }
  return ret;
}

int
main (int argc, char *argv[])
{
  using namespace std;

  bool is_handled = handle_opts (argc, argv);
  if (is_handled)
    return 0;

  // typically initialized by tgf-conf
  tgf::PREFER_LANG = tgf_data.get_pref_lang ();

  if (!tgf::try_ensure_locale ())
    lvlog (LogLv::WARNING, "Available utf8 locales not found");
  else
    lvlog (LogLv::WARNING, tgf::HOST_LANG);

  while (!tgf_data.get_auth_hint ())
    {
      lvlog (LogLv::INFO, "Waiting for authorization");
      std::this_thread::sleep_for (std::chrono::seconds (3));
    }

  if (!tgf_data.get_auth_hint ())
    {
      lvlog (LogLv::INFO, "Not authorized");
      return 1;
    }

  // tgf_data.init()

  collector.init ();

  std::thread producer (focusd_producer);
  std::thread switcher (focusd_switcher);
  std::thread consumer (focusd_consumer);
  producer.join ();
  switcher.join ();
  consumer.join ();

  return 0;
}
