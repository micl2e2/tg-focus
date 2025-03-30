
# Table of Contents

1.  [TG-Focus💡](#org1494378)
2.  [Overview](#orgd450f1c)
3.  [Quickstart](#org2ffa245)
4.  [TGFCMD Examples](#orgd0b1a1b)
5.  [Filters Rules](#org33718b3)
6.  [Can I trust tg-focus?](#org72a1105)
7.  [Is tg-focus a crawler?](#org5b6033a)
    1.  [It's for you if](#orgd5d23dc)
    2.  [It's NOT for you if](#orgf6a73d3)



<a id="org1494378"></a>

# TG-Focus💡

<a href="https://github.com/micl2e2/tg-focus/blob/master/LICENSE-GPL">![GitHub License](https://img.shields.io/github/license/micl2e2/tg-focus?logo=gnu&color=blue&link=https%3A%2F%2Fgithub.com%2Fmicl2e2%2Ftg-focus%2Fblob%2Fmaster%2FLICENSE-GPL)</a>
<a href="https://core.telegram.org/tdlib">![Static Badge](https://img.shields.io/badge/tdlib-1.8.44-%2332a9de)</a>
<a href="https://github.com/tdlib/td">![GitHub commits difference between two branches/tags/commits](https://img.shields.io/github/commits-difference/tdlib/td?base=496450066b9e5429db0936cdccbc886c23cb6b37&head=master&logo=gitextensions&label=pending&color=lightblue&link=https%3A%2F%2Fgithub.com%2Ftdlib%2Ftd)</a>


<a id="orgd450f1c"></a>

# Overview

tg-focus is a comprehensive Telegram message filter that helps
you **focus**. A gentle user bot for message forwarding. Most of
message types are supported: public/private chats, channel updates,
and more.

By customizing *focus filters*, tg-focus can forward any specific
message from any chat to a dedicated chat, **TG-FOCUS**, that way
you have full control of the messages you see.

tg-focus is built on top of [TDLib](https://core.telegram.org/tdlib/), and speaks the same language that
TDLib speaks, in which way tg-focus is just inherently as consistent,
reliable and stable as TDLib itself for most Telegram-specific
functionalities.


<a id="org2ffa245"></a>

# Quickstart

The following 7 steps will help you start up tg-focus and filter some
specific messages.

1.  Download the binaries at GitHub releases [page](https://github.com/micl2e2/tg-focus/releases).

2.  Extract the files:
    
        # linux 
        tar xf tg-focus-XXX-linux-gnu-x86_64.tgz
        cd tg-focus-XXX-linux-gnu-x86_64

3.  Log in your Telegram account firstly, it needs your phone number
    and a login code. Phone number should be typed in international
    format, e.g., `+1 23 4567` if your phone number is `23 4567` with
    region code `+1`.
    
        ./tgfocus auth
        
        # enter your phone and login code...
    
    If everthing works fine, you should see:
    
        Logged in successfullly!

4.  Start up tg-focus daemon:
    
        ./tgfocus quickstart
    
    If everthing works fine, you should see:
    
        startup: done
        focus-start: done

5.  Go back to your Telgram application, open the TG-FOCUS
    chat.
    
    *(NOTE) since the default filter does not filter any messages, you
    might see many continuously incoming messages.*

6.  Configure the focus filters. Suppose you only want to see the
    messages where the word "freedom" is mentioned, you add it to the
    keyword list by sending this message to the TG-FOCUS chat:
    
        TGFCMD editf 1 keywords add "freedom"
    
    *(NOTE) don't forget two double quotes.*
    
    If everthing works fine, you should see:
    
        [ TGFCMD ] success
    
    You can also check your focus filters by sending this message:
    
        TGFCMD filters

7.  Then wait patiently at TG-FOCUS, and enjoy your every 
    freedom-related conversion :-)


<a id="orgd0b1a1b"></a>

# TGFCMD Examples

*Note that all of command mentioned in this section are to be sent out at
TG-FOCUS chat*

To forward the messages that belong to the chat "Freedom User Group":

    TGFCMD editf 1 titles add "Freedom User Group"

To forward the messages that sent by the user "Some One":

    TGFCMD editf 1 senders add "Some One"

To forward the messages that contain a word "freedom":

    TGFCMD editf 1 keywords add "freedom"  

To forward two types of messages, the first contains a word "free
things" and belongs to the chat "Nonfree Lovers", the second contains
a word "nonfree things" and belongs to "Free Lovers":

    TGFCMD editf 1 titles add "Nonfree Lovers"
    TGFCMD editf 1 keywords add "free things"
    TGFCMD editf 2 titles add "Free Lovers"
    TGFCMD editf 2 keywords add "nonfree things"

Not to forward the messages that sent by the user "Nonfree Lover":

    TGFCMD editf 1 no-senders add "Nonfree Lover"

Check current filters configuration:

    TGFCMD filters

Pause forwarding:

    TGFCMD pause

Resume forwarding:

    TGFCMD resume


<a id="org33718b3"></a>

# Filters Rules

Filters configurationa is composed of many *Focus Filters*. A *Focus
Filter* is a filter to match against the message's 
chat/sender/content. They have several rules(or properties):

-   *titles*
-   *keywords*
-   *senders*
-   *no-titles*
-   *no-senders*
-   *no-keywords*

Simply put, *titles*, *keywords*, *senders* are the lists of
chat titles, keywords, senders that you are interested in respectively.
*no-titles*, *no-keywords*, *no-senders* are the ones you don't want
to see.

Users can add values to those rules as many as they like, they
accept ****simple text**** or ****regular expression****, as well as filters.

When a message comes, filters are tried one by one, if there is any
filter than can **match** the message and not **reject** it, it
will be forwarded to TG-FOCUS chat. 

A focus filter **matches** the message if it matches any filter's:

-   *titles* and
-   *senders* and
-   *keywords*

Note that an empty rule(i.e., you haven't configure it) matches
everthing.

If a focus filter  fails to match the incoming message, the next focus
filter will be tried. If all focus filters fail to match it, the
message will be discarded.

A focus filter **rejects** the message if it matches any filter's:

-   *no-title* or
-   *no senders* or
-   *no-keywords*

Note thta if any focus filter rejects the message, the next focus
filter will not be tried, and the message will be discarded.

(You can think of the first three rules as *whitelist*,
the last three rules as *blacklist*)


<a id="org72a1105"></a>

# Can I trust tg-focus?

Yes.

First of all, consider that tg-focus is a GPL-licensed [free
software](<https://www.gnu.org/philosophy/free-sw.en.html>). As for
privacy, only credentials necessary for tdlib initialization are saved
on the user's machine. And they are completely *yours-irrelavent*. As
for source code, tg-focus's source as well as its dependencies' source
are completely open.

Most importantly, all of released binaries are built by **not** any
individual, **but** Github-hosted machines. Check [github actions](https://github.com/micl2e2/tg-focus/actions) for
more details.


<a id="org5b6033a"></a>

# Is tg-focus a crawler?

Despite tg-focus is essentially collecting messsages, tg-focus is not,
and will never be a crawler: forwarded messages is designed to be
difficult to parse, and the forwarding is far away from real-time. If
you use it as a crawler, you will realize this is a wrong tool over
the course of time.

I am here to help telegram users see or omit the messages according to
their own perferences. Using tg-focus to crawl messages would be
absolutely a misuse. And also we will ensure such usages
to become more and more challenging in future releases, because my
**primary goal** is to help catalyze more constructive and meaningful
conversions on Telegram.

Check if tg-focus is for you or not:


<a id="orgd5d23dc"></a>

## It's for you if

1.  You think messages are overwhelming, and still don't want to miss
    out any messages/updates that match your personal perference.

2.  Your think some kinds of messages do not match your personal
    perference and want to ignore them, yet they have reason to
    exist.


<a id="orgf6a73d3"></a>

## It's NOT for you if

1.  You want to scrape/crawl messages from chats/channels.

