
# Table of Contents

1.  [TG-Focus💡](#org44d079c)
2.  [Overview](#orga7e7a94)
3.  [Is tg-focus a crawler?](#orgc3fb5ca)
    1.  [It is for you if](#orgf344a38)
    2.  [It is NOT for you if](#org706402f)


<a id="org44d079c"></a>

# TG-Focus💡

<a href="https://github.com/micl2e2/tg-focus/blob/master/LICENSE-GPL">![GitHub License](https://img.shields.io/github/license/micl2e2/tg-focus?logo=gnu&color=blue&link=https%3A%2F%2Fgithub.com%2Fmicl2e2%2Ftg-focus%2Fblob%2Fmaster%2FLICENSE-GPL)</a>
<a href="https://core.telegram.org/tdlib">![Static Badge](https://img.shields.io/badge/tdlib-1.8.44-%2332a9de)</a>
<a href="https://github.com/tdlib/td">![GitHub commits difference between two branches/tags/commits](https://img.shields.io/github/commits-difference/tdlib/td?base=496450066b9e5429db0936cdccbc886c23cb6b37&head=master&logo=gitextensions&label=pending&color=lightblue&link=https%3A%2F%2Fgithub.com%2Ftdlib%2Ftd)</a>


<a id="orga7e7a94"></a>

# Overview

tg-focus is a comprehensive Telegram message filter that helps
you **focus**. A gentle user bot for message forwarding. Most of
message types are supported: public/private chats, channel updates,
and more.

By customizing *Focus Filters*, tg-focus can forward any specific
message from any chat to a dedicated chat, **TG-FOCUS**, that way
you have full control of the messages you see.

tg-focus is built on top of [TDLib](https://core.telegram.org/tdlib/), and speaks the same language that
TDLib speaks, in which way tg-focus is just inherently as consistent,
reliable and stable as TDLib itself, for any interactions against
Telegram servers. 


<a id="orgc3fb5ca"></a>

# Is tg-focus a crawler?

Despite tg-focus is essentially collecting messsages. tg-focus is not,
and will never be a crawler: forwarded messages is designed to be
difficult to parse, and the forwarding is far away from real-time. If
you use it as a crawler, you will realize this is a wrong tool over
the course of time.

I am here to help telegram users see or omit the messages according to
their own perferences. Using tg-focus to crawl messages would be
absolutely a misuse. And also we will ensure such usages
to become more and more challenging in future releases, because my
primary goal is to help catalyze more constructive and meaningful
conversions on Telegram.

Check if tg-focus is for you or not:


<a id="orgf344a38"></a>

## It is for you if

1.  You think messages are overwhelming, and still don't want to miss
    out any messages/updates that match your personal perference.

2.  Your think some kinds of messages do not match your personal
    perference and want to ignore them, yet they have reason to
    exist.


<a id="org706402f"></a>

## It is NOT for you if

1.  You want to scrape/crawl messages from chats/channels.

