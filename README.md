# TG-Focus💡

<a href="https://github.com/micl2e2/tg-focus/blob/master/LICENSE-GPL">![GitHub License](https://img.shields.io/github/license/micl2e2/tg-focus?logo=gnu&color=blue&link=https%3A%2F%2Fgithub.com%2Fmicl2e2%2Ftg-focus%2Fblob%2Fmaster%2FLICENSE-GPL)</a>
<a href="https://core.telegram.org/tdlib">![Static Badge](https://img.shields.io/badge/tdlib-1.8.44-%2332a9de)</a>
<a href="https://github.com/tdlib/td">![GitHub commits difference between two branches/tags/commits](https://img.shields.io/github/commits-difference/tdlib/td?base=496450066b9e5429db0936cdccbc886c23cb6b37&head=master&logo=gitextensions&label=pending&color=lightblue&link=https%3A%2F%2Fgithub.com%2Ftdlib%2Ftd)</a>


# Table of Contents

- [Overview](#overview)
- [Quick Start](#quick-start)
- [Supported Platforms](#supported-platforms)
- [How To Use](#how-to-use)
  - [Binaries](#binaries)
  - [Docker/Podman](#dockerpodman)
- [Filtering Rules](#filtering-rules)
  - [Filter Examples](#filter-examples)
- [Troubleshooting](#troubleshooting)
- [Q & A](#q--a)
- [Development](#development)
- [Contributing](#contributing)
- [License](#license)
  


# Overview

tg-focus is a comprehensive **T**ele**g**ram message filter that helps
you **focus**. A gentle User Bot for message forwarding.

Most of the message types are supported:

1. Chat Message
2. Channel Update
3. ...and more

By customizing *Focus Filters*, tg-focus can forward any specific
message from any chat to a dedicated chat named *TG-FOCUS*, that way
users have full control of the messages they see.

Notable features:

-   **Reliable & Consistent**: Built directly on top of official
    C++ interfaces([TDLib](https://core.telegram.org/tdlib/)). All
    communications towards Telegram servers are completely handled by
    TDLib.

-   **Easy to customize**: Users can customize *Focus Filter* any
    time they like.

-   **Ready to deploy**: Releases are in three forms: source, prebuilt binaries
    and OCI images. 

# Quick Start

Below is a simple 9-step demonstration of a common use case:
forwarding the messages that contains the keywords we want, ignoring
the message that sent from the users we don't want.

<img src="demo/1.jpg" width="500px"/>

<img src="demo/2.jpg" width="500px"/>

<img src="demo/3.jpg" width="500px"/>

[Back to start](#quick-start)

<img src="demo/4.jpg" width="500px"/>

[Back to start](#quick-start)

<img src="demo/5.jpg" width="500px"/>

[Back to start](#quick-start)

<img src="demo/6.jpg" width="500px"/>

[Back to start](#quick-start)

<img src="demo/7.jpg" width="500px"/>

[Back to start](#quick-start)

<img src="demo/8.jpg" width="500px"/>

[Back to start](#quick-start)

<img src="demo/9.jpg" width="500px"/>

[Back to start](#quick-start)


# Supported Platforms

## Platforms

Never(❌). Maybe In Future(❓). Currently Supported(✅).


| Operating System    | Support(x86_64) |
|---------------------|-----------------|
| macOS               | ❌              |
|                     |                 |
| Windows             | ❓              |
| Windows with WSL 1  | ✅              |
| Windows with WSL 2  | ✅              |
|                     |                 |
| AlmaLinux 9         | ✅              |
| AlmaLinux 8         | ✅              |
| Amazon Linux 2023   | ✅              |
| Amazon Linux 2      | ❌              |
| Amazon Linux 1      | ❌              |
| Arch Linux          | ✅              |
| Debian 12           | ✅              |
| Debian 11           | ✅              |
| Debian 10           | ✅              |
| Fedora 39           | ✅              |
| Fedora 38           | ✅              |
| Fedora 37           | ✅              |
| openSUSE Leap 15.5  | ✅              |
| openSUSE Leap 15.4  | ✅              |
| openSUSE Tumbleweed | ✅              |
| Oracle Linux 9      | ✅              |
| Oracle Linux 8      | ✅              |
| Oracle Linux 7      | ❌              |
| Rocky Linux 9       | ✅              |
| Rocky Linux 8       | ✅              |
| Slackware 15.0      | ✅              |
| Slackware 14.2      | ❌              |
| Solus               | ✅              |
| Ubuntu 23.10        | ✅              |
| Ubuntu 23.04        | ✅              |
| Ubuntu 22.04        | ✅              |
| Ubuntu 20.04        | ✅              |
| Void Linux          | ✅              |
|                     |                 |

**Note**: this is not an exhaustive list, missing platforms can be added
to the list by opening an issue or a pull request.

## Languages:

| Language                 | Status                                    |
|--------------------------|-------------------------------------------|
| English (United States)  | ✅                                        |
| Others                   | [Add Your Language](#supported-languages) |
|                          |                                           |

**Note:** More languages can be supported! Follow the internationalization
instructions(`docs/i18n.org`) to add your language.

# How To Use

Before anything make sure you have:

1. A Telegram account

There are two different ways to deploy tg-focus, through
Docker/Podman, or using prebuilt binary: 

### Binaries

**Step 1:**

Download the binaries at GitHub releases
[page](https://github.com/micl2e2/tg-focus/releases). Please check
whether your [platform](#supported-platforms) is supported or not.

**Step 2:**

Login your Telegram account firstly, it needs your phone number (in
international format, e.g. `+1234567` if your phone number is
`234567` with region code `+1`) and a login code:
```sh
/path/to/tgf-conf auth
```

If you've logged in successfully, the last line should be:
```
[tgf INFO] Logged in!
```

**Step 3: (Optional)**

 Using different language other than default(English) by:

```sh
/path/to/tgf-conf lang XX-YY
```
Note: XX is the language code, YY is the region code, e.g. "en-US" represents
"English (United States)", "en-GB" represents "English (United Kingdom)", etc. 
All valid codes can be found at 
http://www.lingoes.net/en/translator/langcode.htm 

**Step 4:**

Launch tg-focus daemon:

```sh
# run in foreground
/path/to/tgf-focusd

# or run in background
nohup /path/to/tgf-focusd &
```

**Step 5:**

If logined successfullly, a new chat `TG-FOCUS` will be
created, the subsequent messages will be filtered and forwarded to
that chat.

**Step 6:**

Note that by default, tg-focus will forward **all** messages
you receive. You can change the *Focus Filter*s(the filter
configuration) any time you like, to custom the messages you'd like
to receive on that chat. See [examples](#filter-examples).

```sh
/path/to/tgf-conf filters
```

(NOTE: This will open the [GNU nano](https://www.nano-editor.org/)
editor for configuration modification. After modification, press
**Ctrl-O**, **Enter**, **Ctrl-X** to finish the work. As long as the
configuration is valid, the last line of output shall be "Saving
filters...") 

**Step 7:**

Done.

### Docker/Podman

Make sure have [Docker](https://docs.docker.com/engine/) or
[Podman](https://podman.io/) installed on your machine:

**Step 1:**

Pull the image and run it in the background, assuming the newly
created container's name is CONTAINER-NAME: 

```sh
docker pull micl2e2/tg-focus
docker run -d micl2e2/tg-focus
```

**Step 2:**

Login your Telegram account firstly, it needs your phone number (in
international format, e.g. `+1 234567` if your phone number is
`234567` with region code `+1`) and a login code:
```sh
docker exec -it CONTAINER-NAME tgf-conf auth
```

If you've logged in successfully, the last line should be:
```
[tgf INFO] Logged in!
```

**Step 3: (Optional)**

Using different language other than default(English) by:

```sh
docker exec -it CONTAINER-NAME tgf-conf lang XX-YY
```
Note: XX is the language code, YY is the region code, e.g. "en-US" represents
"English (United States)", "en-GB" represents "English (United Kingdom)", etc. 
All valid codes can be found at 
http://www.lingoes.net/en/translator/langcode.htm 

**Step 4:**

If logined successfullly, a new chat *TG-FOCUS* will be
created, the subsequent messages will be filtered and forwarded to
that chat.

**Step 5:**

Note that by default, tg-focus will forward **all** messages
you receive. You can change the *Focus Filter*s(the filter
configuration) any time you like, to custom the messages you'd like
to receive on that chat. See [examples](#filter-examples).

```sh
docker exec -it CONTAINER-NAME tgf-conf filters
```

(NOTE: This will open the embeded [GNU
nano](https://www.nano-editor.org/) editor for configuration
modification. After modification, press **Ctrl-O**, **Enter**, **Ctrl-X**
to finish the work. If the modified one is valid, the last line of
output shall be "Saving filters...")

**Step 6:**

Done.

**Step 7: (Optional)**

Use following command if you want to destroy the
instance and its Telegram-related credentials and resource:

```sh
docker rm --force CONTAINER-NAME
```


# Filtering Rules

A *Focus Filter* is a filter used by tg-focus to match against the
message's text content(text, emoji, or media caption). They are:
- `title`
- `keywords`
- `no-keywords`
- `senders`
- `no-senders`
- `rej-senders`

Users can add as many filters as they like, they accept **simple
text** or **regular expression**. 

When a message comes, they are tried one by one, if there is any
filter than can *match* the message and not *skip* the message , it
will be forwarded.

Currently *Focus Filter* can *match*:

- Chat title(`title`)
- Keywords(`keywords`)
- Senders(`senders`)

can *skip*:

- Keywords(`no-keywords`)
- Senders(`no-senders`)

can *reject*:

- Senders(`rej-senders`)

A message is forwarded **if and only if**:

- it is *matched* by a *Focus Filter*, **and**
- it is *not skipped* by that *Focus Filter*, **and**
- it is *not rejected* by any previous *Focus Filter*.

(Note that the first three rules can be regarded as **whitelist**,
the second two rules can be regarded as **weak blacklist**, the third
one can be regarded as **strong blacklist**) 


## Filter Examples

Say we want to forward all messages in a chat named
"👍AnAwesomeChat👍": 

works, the chat title is fully matched: 

```toml
[[focus-filter]]
title = "👍AnAwesomeChat👍"
```

works, the chat title is partly matched:

```toml
[[focus-filter]]
title = "AnAwesomeChat"
```

works, the chat title is partly matched: 

```toml
[[focus-filter]]
title = "Awesome"
```

works, the chat title is partly matched:

```toml
[[focus-filter]]
title = "👍"
```

works, the regular expression matches the chat title:

```toml
[[focus-filter]]
title = ".*Awesome.*"
```

Say we want to forward the messages that contain "football", but not
"basketball", from a chat named "Sports User Group". We can write our filters as

```toml
[[focus-filter]]
title = "Sports User Group"
keywords = ["football", "soccer"]
no-keywords = ["basketball"]
```

Say we want the messages containing "football", or any other
messages except the ones containing "basketball", and the ones sent by
the user whose full name is "Basketball Lover", or the ones sent by the
user whose user ID is "alice_love_basketball", from a chat named
"SportsUserGroup". We can write the filter as:

```toml
# 1st
[[focus-filter]]
title = "Sports User Group"
keywords = ["football", "soccer"]

# 2nd
[[focus-filter]]
title = "Sports User Group"
no-keywords = ["basketball"]
no-senders = ["Basketball Love", "@alice_love_basketball"]
```

So these message will be forwarded:

- `Alice: I love football!` (matching the 1st)
- `Bob: I play soccer.` (matching the 1st)
- `Alice: I am good at swimming.` (matching the 2nd)
- `Bob: Sometime I also play badminton.` (matching the 2nd)
- `Alice: I play football and basketball.` (matching the 1st, skip the 2nd)

these will NOT be forwarded:

- `Alice: I play badminton and basketball.` (matching the 2nd's
  "no-keywords" means a *reject*)
- `Basketball Lover: Hellop everyone!` (a *reject* by the 2nd)
- `Alice @alice_love_basketball: Hi, I am Alice, I love sports.`
  (a *reject* by the 2nd)


# Troubleshooting

## My TG-FOCUS stops formarding message

Try to restart `tgf-focusd` program or container.


# Q & A

## Can I trust tg-focus?

Yes.

First of all, consider that tg-focus is a GPL-licensed [free
software](https://www.gnu.org/philosophy/free-sw.en.html). 

tg-focus's source as well as its 
dependencies' source are completely open. Additionally, all its
binaries and OCI images will **not** be built by any individual, but
by Github-hosted machines. All build details can be found in the
dedicated repository
[tg-focus-cicd](https://github.com/micl2e2/tg-focus-cicd). 

It only saves Telegram server credentials on the user's machine because they are
necessary during initialization. And they are completely **yours-irrelavent**.

However, since tg-focus is largely built on top of TDLib, how much you can trust
tg-focus **largely** depends on how much you trust TDLib and Telegram's governance.
Hence use **at your own risk**.


## What makes tg-focus a gentle User Bot?

TDLib states that it is _a library for building Telegram
clients_. By embedding TDLib, tg-focus, like any other User
Bot, is able to use all TDLib's public APIs. While it opens the
possibility for many powerful features, it also means User Bots have
far more privileges than a general-purpose
[Bot](https://core.telegram.org/bots). This can be a double-edged
sword.

There are already some examples where some of the good-purpose User
Bots disappoint their users because of their misuse of Telegram APIs,
such as repeatedly sending random messages to users' random
chats, or other unmanaged behaviors. This is probably one of
the reasons why Telegram User Bots have a bad reputation. 

However, tg-focus will not follow their paths, we have one strict
principle: all message collected by tg-focus will only be forwarded
to a newly created chat. 

With this principle, it is firmly guaranteed that **no** messages
generated by tg-focus will be ever sent to the wrong chat. Additionally,
there is **no** reason for tg-focus users to worry about whether any
existing or new features will be harmful or not. By the time of
updates, tg-focus will remain **gentle**.

# Development

See `docs/development.org`.

# Contributing

tg-focus is still far from perfect, any form of contribution is
welcomed! 

- Ask question (open an issue with label [![](https://img.shields.io/static/v1?label=&message=question&color=purple)](https://github.com/micl2e2/tg-focus/issues/new))
- Bug Report (open an issue with label [![](https://img.shields.io/static/v1?label=&message=bug&color=red)](https://github.com/micl2e2/tg-focus/issues/new))
- Feature Request (open an issue with label
  [![](https://img.shields.io/static/v1?label=&message=enhancement&color=lightblue)](https://github.com/micl2e2/tg-focus/issues/new))
- Documentation Improvement
- Code Contribution (tg-focus follows [GCC](https://gcc.gnu.org/) code style)



# License

tg-focus is licensed under [GNU General Public License Version
3.0](https://github.com/micl2e2/tg-focus/blob/master/LICENSE-GPL). 

