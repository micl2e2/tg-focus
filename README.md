# TG-Focus


![GitHub Workflow Status (with event)](https://img.shields.io/github/actions/workflow/status/micl2e2/tg-focus/build-tgfocus.yml)
![Static Badge](https://img.shields.io/badge/TDLib-1.8.21-lightblue)
![Docker Image Size (tag)](https://img.shields.io/docker/image-size/micl2e2/tg-focus/latest?color=lightgreen)


# Table of Contents

- [About](#about)
- [A Simple Demo](#a-simple-demo)
- [How To Use](#how-to-use)
  - [Hardware Requirement](#hardware-requirement)
  - [Deployment](#deployment)
	- [Use DockerHub Image](#use-dockerhub-image)
	- [Use Prebuilt Binaries](#use-prebuilt-binarieswork-in-progress)
- [Filtering Rules](#filtering-rules)
  - [Filter Examples](#filter-examples)
- [Q & A](#qa)
  - [Can I trust tg-focus?](#can-i-trust-tg-focus)
  - [Too many TG-FOCUS chats created, what should I do?](#too-many-tg-focus-chats-created-what-should-i-do)
- [Development](#development)
  - [Dependencies](#dependencies)
  - [Buidling](#building)
- [Contributing](#contributing)
- [License](#license)
  


# About

tg-focus is a **T**ele**g**ram message filter that helps you
**focus**. It supports several forms of text messages, including

1. Chat Message(Raw/Reply/Forward/Quoted)
2. Media Caption
3. Emoji
4. Channel Update

By customizing *Focus Filters*, tg-focus can forward any specific
message from any chat to a dedicated chat named *TG-FOCUS*, that way
users have full control of the messages they see.

Telegram is famous for its openness. While users are free to make a
speech in any public chat, they should be equally free to choose what
they see. Telegram's official client apps already have excellent
support for the former need, tg-focus and other counterparts are
trying to fill the blank in the latter one. 

Notable features:

-   **Reliable & Consistent**: Built directly on top of official
    C++ interfaces([TDLib](https://core.telegram.org/tdlib/)). All
    communications towards Telegram servers are completely handled by
    TDLib.

-   **Easy to customize**: Users can customize *Focus Filter* any
    time they like. The filter accepts both simple text and [regular 
    expressions](https://en.wikipedia.org/wiki/Regular_expression). The
    configuration file is in human-readable 
    [TOML](https://toml.io/en/) format.

-   **Ready to deploy**: Releases are made in three forms: source,
    prebuilt OCI image(docker/podman) and prebuilt
    binary(Work-In-Progress).

# A Simple Demo

Below is a simple 9-step demonstration of a common use case:
forwarding the messages that contains the keywords we want, ignoring
the message that sent from the users we don't want.

<img src="demo/1.jpg" width="500px"/>

<img src="demo/2.jpg" width="500px"/>

<img src="demo/3.jpg" width="500px"/>

[Back to start](#a-simple-demo)

<img src="demo/4.jpg" width="500px"/>

[Back to start](#a-simple-demo)

<img src="demo/5.jpg" width="500px"/>

[Back to start](#a-simple-demo)

<img src="demo/6.jpg" width="500px"/>

[Back to start](#a-simple-demo)

<img src="demo/7.jpg" width="500px"/>

[Back to start](#a-simple-demo)

<img src="demo/8.jpg" width="500px"/>

[Back to start](#a-simple-demo)

<img src="demo/9.jpg" width="500px"/>

[Back to start](#a-simple-demo)


# How To Use

Before anything make sure you have:

1. A Telegram account
2. Telegram API ID and corresponding API HASH, they can be obtained 
   at https://my.telegram.org. 

## Hardware Requirement

There is no rigorous restriction on hardware, here is a memory usage
overview in 7 days:

| day1 | day2 | day3 | day4 | day5  | day6  | day7  |
|------|------|------|------|-------|-------|-------|
| 76MB | 84MB | 90MB | 96MB | 103MB | 108MB | 115MB |

(NOTE: this table is just for reference, it is recorded by an account
with about 30 public chats, most of which have over 1000 members and
500 messages produced per day)


## Deployment

There are two different way to deploy and launch a tg-focus instance:

1. Use DockerHub Image
2. Use Prebuilt Binaries(work-in-progress)

Suppose you are going to:

### Use DockerHub Image

Make sure have [Docker](https://docs.docker.com/engine/) or
[Podman](https://podman.io/) installed on your machine:

1. Pull the image and run it in the background, assuming the newly
created container's name is CONTAINER-NAME: 

```sh
docker pull docker.io/micl2e2/tg-focus:latest
docker run -d docker.io/micl2e2/tg-focus
```

2. Login your telegram account first, here you should provide *API ID*,
*API HASH*, and your *phone number*:

```sh
docker exec -it CONTAINER-NAME tf-conf auth
```

3. If logined successfullly, a new chat named *TG-FOCUS* will be
   created, the subsequent messages will be filtered and forwarded to
   this chat.

4. Note that by default, tg-focus will forward **all** messages
   you receive. You can change the *Focus Filter*s(the filter
   configuration) any time you like, to custom the messages you'd like
   to receive on that chat. See [examples](#filter-examples).

```sh
docker exec -it CONTAINER-NAME tf-conf filters
```

(NOTE: This will open the embeded [GNU
nano](https://www.nano-editor.org/) editor for configuration
modification. After modification, press Ctrl-O, Enter, Ctrl-X to
finish the work. If the modified one is valid, the last line of
output shall be "Saving filters...")

5. Done

6. (Optional) Use following command if one wants to destroy the
   instance and its Telegram-related credentials and resource:

```sh
docker rm --force CONTAINER-NAME
```


### Use Prebuilt Binaries(Work-In-Progress)

1. Download the binaries.

2. Login your Telegram account first, it needs API ID, API HASH, and
your phone number, the first two can be obtained in
https://my.telegram.org: 

```sh
/path/to/tf-conf auth
```

3. Launch tg-focus daemon:

```sh
/path/to/tf-focusd
```

4. If logined successfullly, a new chat named `TG-FOCUS` will be
   created, the subsequent messages will be filtered and forwarded to
   this chat.

5. Change the *Focus Filter*s any time you like, to make adjustment on
   the messages you would like to see on that chat.

```sh
/path/to/tf-conf filters
```

6. Done



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



# Q&A

## Can I trust tg-focus?

Absolutely yes!

First of all, consider that tg-focus is a GPL-licensed [free
software](https://www.gnu.org/philosophy/free-sw.en.html). 

tg-focus inherits the openness from TDLib, its source as well as its 
dependencies' source are completely open. Additionally, all its
binaries and OCI images will **not** be built by any individual, but
by Github-hosted containers. Build details such as checksums are
shipped along with binaries and images. Users are free to verify them
on the GitHub Action
[page](https://github.com/micl2e2/tg-focus/actions). 

Telegram bot is useful for some specific tasks, but not sufficient
to be a **comprehensive** message filter. Therefore, during the
authorization process, tg-focus needs to log in user's account,
which in turn needs a phone number and login code,
just as any functional Telegram client would require. It only saves
API ID/HASH on the user's machine because these two are
required each time TDLib initializes. But **under no
circumstances** would tg-focus save the user's phone number or any
other credentials.

## Too many TG-FOCUS chats created, what should I do?

tg-focus creates a TG-FOCUS chat whenever tg-focus starts or restarts,
it is not a bug or defeat, but an **intended** behavior. If there are
many TG-FOCUS chats created, it is probably a misuse case and all you can
do is delete them manually. Here are some reasons justifying that:

1. tg-focus was originally developed as a
   long-running server-side application, restarting shall not be
   performed frequently.

2. Each tg-focus instance has its own filter configuration, hence one
   chat for one instance. Each of them is independent so that no one
   could possibly interfere with another.
   
3. To be a comprehensive message filter, tg-focus has to be a User Bot,
   which has far more privileges than a general-purpose
   [Bot](https://core.telegram.org/bots).  There
   are already some examples where some of the good-purpose User Bots
   disappoint their users 
   because of their misuse of Telegram APIs, such as repeatedly sending
   random messages to users' random chats. This is probably one of the
   reasons why Telegram userbots have a bad reputation. However, tg-focus will
   not follow their paths, we have one strict principle: **all
   tg-focus's messages will only be sent to a newly created
   chat**. With this principle, it 
   is firmly guaranteed that **no** messages generated by tg-focus will be
   sent to the wrong chat. Additionally, there is **no** reason for tg-focus
   users to worry about whether any existing or new features
   will be harmful or not.
   




# Development

**Note** that currently the development is documented only for Linux 
platforms.

## Dependencies

- Bash
- Git
- C++20 compatible compiler
- CMake

## Building


```sh
# Get the source
git clone --depth=1 https://github.com/micl2e2/tg-focus

# Make sure in the source root directory
cd tg-focus

# Download the dependencies of tg-focus
bash dev/install-deps.bash

# Setup a release build
cmake -DCMAKE_BUILD_TYPE=Release -B build

# Build
cmake --build build

# Test
cd build && ctest
```

# Contributing

tg-focus is still far from perfect, any form of contribution is
welcomed! 

- Ask question (open an issue with label [![](https://img.shields.io/static/v1?label=&message=question&color=purple)](https://github.com/micl2e2/tg-focus/issues/new))
- Bug Report (open an issue with label [![](https://img.shields.io/static/v1?label=&message=bug&color=red)](https://github.com/micl2e2/tg-focus/issues/new))
- Feature Request (open an issue with label
  [![](https://img.shields.io/static/v1?label=&message=enhancement&color=lightblue)](https://github.com/micl2e2/tg-focus/issues/new))
- Documentation Improvement
- Code Contribution (tg-focus follows [GNU](https://www.gnu.org/prep/standards/standards.html) code style)



# License

tg-focus is licensed under [GNU General Public License Version
3.0](https://github.com/micl2e2/tg-focus/blob/master/LICENSE-GPL). 

