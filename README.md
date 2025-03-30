
# Table of Contents

1.  [Overview](#org1b4be68)
    1.  [tg-focus is for you if](#orgc4bcaef)
    2.  [tg-focus is NOT for you if](#orge582069)
2.  [Is tg-focus a crawler?](#org37594d4)
3.  [Quick Start](#orgea9ecec)


<a id="org1b4be68"></a>

# Overview

tg-focus is a comprehensive \_T<sub>ele</sub>\*\*g\*\*ram message filter that helps
you ****focus****. A gentle user bot for message forwarding. Most of
message types are supported: public/private chats, channel updates,
and more.

By customizing **Focus Filters**, tg-focus can forward any specific
message from any chat to a dedicated chat, **TG-FOCUS**, that way
you have full control of the messages you see.

tg-focus is built on top of TDLib, and speak the same language that
TDLib speaks, in which way tg-focus is just inherently as consistent,
reliable and stable as TDLib itself. 

Despite tg-focus is essentially a messsage collector, it is not a
general purpose collector, it is designed for some specific use cases,
and also not for some others:


<a id="orgc4bcaef"></a>

## tg-focus is for you if

1.  You think messages are overwhelming, but still don't want to miss
    out any messages/updates that match your personal perference.

2.  Your personal perference for them changes from time to time.


<a id="orge582069"></a>

## tg-focus is NOT for you if

1.  You want to scrape/crawl messages from chats/channels.


<a id="org37594d4"></a>

# Is tg-focus a crawler?

tg-focus is not, and will never be a crawler: forwarded
messages is designed to be difficult to parse, and the forwarding is
far away from real-time. If you use it as a crawler,
you will realize this is a wrong tool over the course of time. 

I am here to help telegram users see or omit the messages according to
their own perferences. Using tg-focus to crawl messages would be a
completely misuse. And also we will ensure such usages
to become more and more challenging in future releases. My primary
goal is to help catalyze more constructive and meaningful conversions
on Telegram.

Notable features:

-   ****Reliable & Consistent****: Built directly on top of official
    C++ interfaces([TDLib](<https://core.telegram.org/tdlib/>)). All
    communications towards Telegram servers are completely handled by
    TDLib.

-   ****Easy to customize****: Users can customize **Focus Filter** any
    time they like.

-   ****Ready to deploy****: Releases are in three forms: source, prebuilt binaries
    and OCI images.


<a id="orgea9ecec"></a>

# Quick Start

Below is a simple 9-step demonstration of a common use case:
forwarding the messages that contains the keywords we want, ignoring
the message that sent from the users we don't want.

