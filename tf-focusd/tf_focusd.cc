#include <iostream>
#include <thread>

#include "lv_log.hh"
#include "state.hh"
#include "worker.hh"

void
handle_opts (int argc, char *argv[])
{
  if (argc > 1)
    if (strcmp (argv[1], "--verbose") == 0)
      g_log_lv = LogLv::DEBUG;
}

int
main (int argc, char *argv[])
{
  using namespace std;

  setlocale (LC_ALL, "en_US.UTF-8");

  handle_opts (argc, argv);

  while (!tf_data.get_auth_hint ())
    {
      lv_log (LogLv::INFO, "Waiting for authorization");
      std::this_thread::sleep_for (std::chrono::seconds (3));
    }

  if (!tf_data.get_auth_hint ())
    {
      lv_log (LogLv::INFO, "Not authorized");
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
