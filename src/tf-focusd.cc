#include <thread>

#include "producer.hh"
#include "consumer.hh"
#include "switcher.hh"

#include "tgfocus_state.hh"

extern TdClient td_client;

extern TgFocusData tf_data;

template <class... Args>
void
log (std::format_string<Args...> fmt, Args &&...args)
{
  constexpr std::string_view header{"[tf-focusd] "};
  std::cout << header << std::format (fmt, args...) << std::endl;
}

template <class... Args>
void
log_flush (std::format_string<Args...> fmt, Args &&...args)
{
  constexpr std::string_view header{"[tf-focusd] "};
  std::cout << header << std::format (fmt, args...) << std::flush;
}

int
main ()
{
  using namespace std;

  while (!tf_data.get_auth_hint ())
    {
      log ("Waiting for authorization");
      std::this_thread::sleep_for (std::chrono::seconds (3));
    }

  if (!tf_data.get_auth_hint ())
    {
      log ("Not authorized");
      return 1;
    }

  // tf_data.init ();

  td_client.init ();

  std::thread producer (tgmsg_producer);
  std::thread switcher (tgmsg_switcher);
  std::thread consumer (tgmsg_consumer);
  producer.join ();
  switcher.join ();
  consumer.join ();

  return 0;
}
