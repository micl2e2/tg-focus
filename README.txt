                             ━━━━━━━━━━━━━
                                TG-FOCUS

                              Michael Lee
                             ━━━━━━━━━━━━━


Table of Contents
─────────────────

TG-Focus💡
Quickstart
TGFCMD Examples
Filters Rules
Contributing
FAQ
License





TG-Focus💡
══════════

  <a
  href="<https://github.com/micl2e2/tg-focus/blob/master/LICENSE-GPL>">![GitHub
  License](<https://img.shields.io/github/license/micl2e2/tg-focus?logo=gnu&color=blue&link=https%3A%2F%2Fgithub.com%2Fmicl2e2%2Ftg-focus%2Fblob%2Fmaster%2FLICENSE-GPL>)</a>
  <a href="<https://core.telegram.org/tdlib>">![Static
  Badge](<https://img.shields.io/badge/tdlib-1.8.46-%2332a9de>)</a> <a
  href="<https://github.com/tdlib/td>">![GitHub commits difference
  between two
  branches/tags/commits](<https://img.shields.io/github/commits-difference/tdlib/td?base=347bd31a30dd479fc4c5fa966316fb2834802bf8&head=master&logo=gitextensions&label=pending&color=lightblue&link=https%3A%2F%2Fgithub.com%2Ftdlib%2Ftd>)</a>

  tg-focus is a comprehensive Telegram message filter that helps you
  *focus*. A gentle user bot for message forwarding. Most of message
  types are supported: public/private chats, channel updates, and more.

  By customizing /filters/ through TGFCMD, tg-focus can forward any
  specific message from any chat to a dedicated chat, *TG-FOCUS*, that
  way you have full control of the messages you see.

  tg-focus is built on top of [TDLib] directly, in which way tg-focus is
  inherently as consistent, reliable and stable as TDLib itself for most
  Telegram-specific features.

  The supported platforms are:

  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━
   OS \ ARCH  x86 64  aarch64 
  ────────────────────────────
   Linux      ✅      ✅      
   Windows    🔨      🔨      
   BSD        🔨      🔨      
   macOS      🔨      🔨      
                            
  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━

  Supported(✅). Work In Progress(🔨).


[TDLib] <https://core.telegram.org/tdlib/>


Quickstart
══════════

  The following 6 steps will help you start up tg-focus and filter some
  specific messages.

  1. Download the binaries at GitHub releases [page].

  2. Extract the files:

     ┌────
     │ # linux 
     │ tar xf tg-focus-3.0-linux-x86_64.tar.gz
     │ cd tg-focus-3.0-linux-x86_64
     └────

  3. Log in your Telegram account firstly, it needs your phone number
     and a login code. Phone number should be typed in international
     format, e.g., `+1 23 4567' if your phone number is `23 4567' with
     region code `+1' (spaces are allowed).

     ┌────
     │ ./tgfocus auth
     │ 
     │ # enter your phone and login code...
     └────

     If everthing works fine, you should see:

     ┌────
     │ Logged in successfullly! 
     └────

  4. Start up tg-focus daemon:

     ┌────
     │ ./tgfocus quickstart
     └────

     If everthing works fine, you should see:

     ┌────
     │ startup: done
     │ focus-start: done
     └────

  5. Go back to your Telgram application, wait for the TG-FOCUS group
     showing up.

     *If* TG-FOCUS chat does not appear, you can send a message
     "tg-focus" in a random group(e.g. Saved Messages) to get it shown
     up. This is because by default, only messages contain "tg-focus"
     will be filtered and forwarded.

  6. Once the chat shown up, you send a message `TGFCMD help' to get
     started. The following image tutorials will show you how to use it.

  7. Image Tutorials: Say there is a Telegram user, called Bob. He owns
     a group. Every day, plenty of messages are sending out in that
     group, it is too overwhelming, Bob wants to focus on the messages
     that strongly interest him, he decides to use tg-focus. After the
     previous 6-step setup, he now is going to use TGFCMD to configure
     his tg-focus to forward the messages he is strongly interested in:

     <img src="docs/imgtuto/tuto1.png" width="500px"/><br/> <img
     src="docs/imgtuto/tuto2.png" width="800px"/><br/> <img
     src="docs/imgtuto/tuto3.png" width="500px"/><br/> <img
     src="docs/imgtuto/tuto4.png" width="550px"/><br/> <img
     src="docs/imgtuto/tuto5.png" width="700px"/><br/> <img
     src="docs/imgtuto/tuto6.png" width="600px"/><br/> <img
     src="docs/imgtuto/tuto7.png" width="550px"/><br/>


[page] <https://github.com/micl2e2/tg-focus/releases>


TGFCMD Examples
═══════════════

  /Note that TGFCMD commands are to be sent out in the TG-FOCUS group/

  To forward the messages that belong to the chat "Freedom User Group":

  ┌────
  │ TGFCMD filter.edit 1 titles add "Freedom User Group"
  └────

  To forward the messages that sent by the user "Some One":

  ┌────
  │ TGFCMD filter.edit 1 senders add "Some One"
  └────

  To forward the messages that contain a word "freedom":

  ┌────
  │ TGFCMD filter.edit 1 keywords add "freedom"  
  └────

  To forward two types of messages, the first contains a word "free
  things" and belongs to the chat "Nonfree Lovers", the second contains
  a word "nonfree things" and belongs to "Free Lovers":

  ┌────
  │ TGFCMD filter.edit 1 titles add "Nonfree Lovers"
  │ TGFCMD filter.edit 1 keywords add "free things"
  │ TGFCMD filter.edit 2 titles add "Free Lovers"
  │ TGFCMD filter.edit 2 keywords add "nonfree things"
  └────

  Not to forward the messages that sent by the user "Nonfree Lover":

  ┌────
  │ TGFCMD filter.edit 1 no-senders add "Nonfree Lover"
  └────

  Check current filters configuration:

  ┌────
  │ TGFCMD filters
  └────

  Pause forwarding:

  ┌────
  │ TGFCMD pause
  └────

  Resume forwarding:

  ┌────
  │ TGFCMD resume
  └────


Filters Rules
═════════════

  Filters configurationa is composed of many /Focus Filters/. A /Focus
  Filter/ is a filter to match against the message's
  chat/sender/content. They have several rules(or properties):

  • /titles/
  • /keywords/
  • /senders/
  • /no-titles/
  • /no-senders/
  • /no-keywords/

  Simply put, /titles/, /keywords/, /senders/ are the lists of chat
  titles, keywords, senders that you are interested in respectively.
  /no-titles/, /no-keywords/, /no-senders/ are the ones you don't want
  to see.

  Users can add values to those rules as many as they like, they accept
  **simple text** or **regular expression**, as well as filters.

  When a message comes, filters are tried one by one, if there is any
  filter than can *match* the message and not *reject* it, it will be
  forwarded to TG-FOCUS chat.

  A focus filter *matches* the message if it matches any filter's:

  • /titles/ and
  • /senders/ and
  • /keywords/

  Note that an empty rule(i.e., you haven't configure it) matches
  everthing.

  If a focus filter fails to match the incoming message, the next focus
  filter will be tried. If all focus filters fail to match it, the
  message will be discarded.

  A focus filter *rejects* the message if it matches any filter's:

  • /no-title/ or
  • /no senders/ or
  • /no-keywords/

  Note thta if any focus filter rejects the message, the next focus
  filter will not be tried, and the message will be discarded.

  (You can think of the first three rules as /whitelist/, the last three
  rules as /blacklist/)


Contributing
════════════

  tg-focus is still far from perfect, any form of contribution is
  welcomed!

  • Ask question (open an issue with label "question")
  • Bug Report (open an issue with label "bug")
  • Feature Request (open an issue with label "enhancement")
  • Documentation Improvement
  • Code Contribution (tg-focus follows [GCC](<https://gcc.gnu.org/>)
    code style)


ToDo list
─────────

  This is a list of TODOs that are certainly to be finished in the near
  future:

  • ☐ Project Logo (will be used as TG-FOCUS chat icon)
  • ☐ Docker/Podman Images
  • ☐ Forwarded message's Look
  • ☐ Configuration in JSON Format
  • ☐ Windows Port
  • ☐ BSD Port
  • ☐ macOS Port


Development
───────────

Linux
╌╌╌╌╌

  Distribution:
  • Debian 11 (Bullseye)
    • /note: bullseye-like distros might work, but it won't be
      considered an issue if not working there./
  Software:
  • bash, git, sudo, apt, etc.
    • run `bash ci/prep-libs-deb11-x86_64.bash' to automatically prepare
      them.
  • custom built GCC
    • run `bash ci/prep-toolc-deb11-x86_64.bash' to automatically
      prepare them.
  Building:
  • run `bash ci/build-tgfocus-deb11-x86_64.bash' to get a Release
    build.
  • run `DDD=1 bash ci/build-tgfocus-deb11-x86_64.bash' to get a Debug
    build.


FAQ
═══

Why yet another Telegram message forwarder?
───────────────────────────────────────────

  There are already many TG message forwarders over there, and some of
  them have been continuously developed for a long time. I do think most
  of them are doing a great job. However what I was looking for was a
  message forwarder with a combination of stability, reliability,
  transparency, and flexibility, none of them could completely meet my
  personal preference, at least in the days before this project started.


Can I trust tg-focus?
─────────────────────

  Yes.

  First of all, consider that tg-focus is a GPL-licensed [free
  software](<https://www.gnu.org/philosophy/free-sw.en.html>). As for
  privacy, only credentials necessary for tdlib initialization are saved
  on the user's machine. And they are completely /yours-irrelavent/. As
  for source code, tg-focus's source as well as its dependencies' source
  are completely open.

  And one more step we take is that, all of released binaries are built
  by *not* any individual, but *Github-hosted* machines.  Check [github
  actions] for more details.


[github actions] <https://github.com/micl2e2/tg-focus/actions>


License
═══════

  tg-focus is licensed under /GNU General Public License Version
  3.0/. There is NO WARRANTY, to the extent permitted by law.
