#include <iostream>
#include <thread>
#include <string.h>

#include "lv_log.hh"
#include "state.hh"
#include "worker.hh"
#include "tf_locale.hh"

void
handle_opts (int argc, char *argv[])
{
  if (argc > 1)
    {
      for (int i = 1; i < argc; i++)
	{
	  if (strcmp (argv[i], "--verbose") == 0)
	    g_log_lv = LogLv::DEBUG;
	  if (strcmp (argv[i], "--prefer-lang") == 0 && i + 1 < argc
	      && strlen (argv[i + 1]) >= 5)
	    tgf::PREFER_LANG = tgf::lang_from_cstr (argv[++i]);
	}
    }
}

int
main (int argc, char *argv[])
{
  using namespace std;

  handle_opts (argc, argv);

  if (!tgf::try_ensure_locale ())
    lvlog (LogLv::WARNING, "Available utf8 locales not found");
  else
    lvlog (LogLv::WARNING, tgf::HOST_LANG);

  while (!tf_data.get_auth_hint ())
    {
      lvlog (LogLv::INFO, "Waiting for authorization");
      std::this_thread::sleep_for (std::chrono::seconds (3));
    }

  if (!tf_data.get_auth_hint ())
    {
      lvlog (LogLv::INFO, "Not authorized");
      return 1;
    }

  // tf_data.init()

  collector.init ();

  std::thread producer (focusd_producer);
  std::thread switcher (focusd_switcher);
  std::thread consumer (focusd_consumer);
  producer.join ();
  switcher.join ();
  consumer.join ();

  return 0;
}
