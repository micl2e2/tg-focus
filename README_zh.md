# TG-Focus

![GitHub License](https://img.shields.io/github/license/micl2e2/tg-focus?logo=gnu&color=blue&link=https%3A%2F%2Fgithub.com%2Fmicl2e2%2Ftg-focus%2Fblob%2Fmaster%2FLICENSE-GPL)
![Docker Image Size (tag)](https://img.shields.io/docker/image-size/micl2e2/tg-focus/latest?logo=docker&logoColor=white&color=lightgreen&link=https%3A%2F%2Fhub.docker.com%2Fr%2Fmicl2e2%2Ftg-focus)
![GitHub commits difference between two branches/tags/commits](https://img.shields.io/github/commits-difference/tdlib/td?base=cd25b29452644856b7472dd0cd69391b2226d3c3&head=master&logo=gitextensions&label=pending%20TDLib%20updates&color=lightblue&link=https%3A%2F%2Fgithub.com%2Ftdlib%2Ftd)



# 目录

- [关于](#关于)
- [一个简单的演示](#一个简单的演示)
- [支持的平台](#支持的平台)
- [支持的语言](#支持的语言)
- [如何使用](#如何使用)
  - [硬件要求](#硬件要求)
  - [部署](#部署)
    - [使用预编译二进制文件](#使用预编译二进制文件)
    - [使用 Docker 镜像](#使用-docker-镜像)
- [过滤规则](#过滤规则)
  - [过滤器示例](#过滤器示例)
- [Q\&A](#qa)
  - [我能相信 tg-focus 吗？](#我能相信-tg-focus-吗)
  - [什么使得 tg-focus 成为一个温和的 User Bot？](#什么使得-tg-focus-成为一个温和的-user-bot)
- [开发](#开发)
- [贡献](#贡献)
- [许可证](#许可证)
  


# 关于

tg-focus 是一个全面的帮助您集中注意力的 **T**ele**g**ram 消息过滤器。一个用于消息转发的温和的 User Bot。

支持多种形式的文本消息：

1. 聊天消息（原始/回复/转发/引用）
2. 媒体标题
3. Emoji 表情符号
4. 频道更新

通过定制 *Focus 过滤器*，tg-focus 可以将任何特定的消息从任何聊天转发到一个名为 *TG-FOCUS*  的专用聊天中，这样用户就可以完全控制他们看到的消息。

Telegram 以其开放性而闻名。虽然用户可以在任何公共聊天中自由发言，但他们同样应该有权选择他们想看到的内容。Telegram 的官方客户端已经很好地支持了前者，tg-focus 和其他类似工具则试图填补后者的空白。

显著特点：

-   **可靠性和一致性**：直接构建在官方 C++ 接口（[TDLib](https://core.telegram.org/tdlib/)）之上。所有与 Telegram 服务器的通信完全由 TDLib 处理。

-   **易于定制**：用户可以随时自定义 *Focus Filter* 。该过滤器接受简单文本和[正则表达式](https://en.wikipedia.org/wiki/Regular_expression)。配置文件采用人类可读的 [TOML](https://toml.io/en/) 格式。

-   **随时部署**：以三种形式发布：源代码、Docker 镜像和预编译的二进制文件。

# 一个简单的演示

以下是一个常见用例的简单 9 步演示：转发包含我们想要的关键词的消息，忽略来自我们不想要的用户发送的消息。

<img src="demo/zh/1.png" width="500px"/>

<img src="demo/zh/2.png" width="500px"/>

<img src="demo/zh/3.png" width="500px"/>

[回到开始](#一个简单的演示)

<img src="demo/zh/4.png" width="500px"/>

[回到开始](#一个简单的演示)

<img src="demo/zh/5.png" width="500px"/>

[回到开始](#一个简单的演示)

<img src="demo/zh/6.png" width="500px"/>

[回到开始](#一个简单的演示)

<img src="demo/zh/7.png" width="500px"/>

[回到开始](#一个简单的演示)

<img src="demo/zh/8.png" width="500px"/>

[回到开始](#一个简单的演示)

<img src="demo/zh/9.png" width="500px"/>

[回到开始](#一个简单的演示)


# 支持的平台

| Operating System    | Support(x86_64) |
|---------------------|-----------------|
| Windows             | ❌              |
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


# 支持的语言

| Language                 | Status                                    |
|--------------------------|-------------------------------------------|
| English (Hong Kong)      | ✅                                        |
| English (United States)  | ✅                                        |
| English (Zimbabwe)       | ✅                                        |
| Chinese (Mainland China) | ✅                                        |
| Chinese (Hong Kong)      | ✅                                        |
| Others                   | [Add Your Language](#supported-languages) |
|                          |                                           |

**Note:** More languages can be supported! Follow the internationalization
instructions(`docs/i18n.org`) to add your language.

# 如何使用

Before anything make sure you have:

1. A Telegram account
2. A Telegram API ID and the corresponding API HASH, they can be
   obtained at https://my.telegram.org. 

## 硬件要求

There is no rigorous restriction on hardware, here is a memory usage
overview in 7 days:

| day1 | day2 | day3 | day4 | day5  | day6  | day7  |
|------|------|------|------|-------|-------|-------|
| 76MB | 84MB | 90MB | 96MB | 103MB | 108MB | 115MB |

(NOTE: this table is just for reference, it is recorded by an account
with about 30 public chats, most of which have over 1000 members and
500 messages produced per day)


## 部署

There are two different way to deploy:

### 使用预编译二进制文件

**Step 1:**

Download the binaries at GitHub releases
[page](https://github.com/micl2e2/tg-focus/releases). Please check
whether your [platform](#supported-platforms) is supported or not.

**Step 2:**

Login your Telegram account first, it needs API ID, API HASH,
your phone number (in international format, cannot contain spaces, e.g., input `+1234567` if your country code is `+1` and phone number is `234567`) and a login code, the first two
can be obtained in https://my.telegram.org:

```sh
/path/to/tgf-conf auth
```
If you've logged in successfully, the last three lines will be:
```
[tgf INFO] Log in successfully!
[tgf INFO] Closing...
[tgf INFO] Closed
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

If logined successfullly, a new chat named `TG-FOCUS` will be
created, the subsequent messages will be filtered and forwarded to
this chat.

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


### 使用 Docker 镜像

Make sure have [Docker](https://docs.docker.com/engine/) or
[Podman](https://podman.io/) installed on your machine:

**Step 1:**

Pull the image and run it in the background, assuming the newly
created container's name is CONTAINER-NAME: 

```sh
docker pull docker.io/micl2e2/tg-focus:latest
docker run -d docker.io/micl2e2/tg-focus
```

**Step 2:**

Login your telegram account first, here you should provide API ID,
API HASH, your phone number(in international format), and a login
code, the first two can be obtained in https://my.telegram.org:

```sh
docker exec -it CONTAINER-NAME tgf-conf auth
```
If you've logged in successfully, the last three lines will be:
```
[tgf INFO] Log in successfully!
[tgf INFO] Closing...
[tgf INFO] Closed
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

If logined successfullly, a new chat named *TG-FOCUS* will be
created, the subsequent messages will be filtered and forwarded to
this chat.

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


# 过滤规则

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


## 过滤器示例

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

## 我能相信 tg-focus 吗？

Absolutely yes!

First of all, consider that tg-focus is a GPL-licensed [free
software](https://www.gnu.org/philosophy/free-sw.en.html). 

tg-focus inherits the openness of TDLib, its source as well as its 
dependencies' source are completely open. Additionally, all its
binaries and OCI images will **not** be built by any individual, but
by Github-hosted machines. All build details can be found in a
dedicated repository
[tg-focus-cicd](https://github.com/micl2e2/tg-focus-cicd). 

Telegram bot is useful for some specific tasks, but not sufficient
to be a **comprehensive** message filter. Therefore, during the
authorization process, tg-focus needs to log in user's account,
which in turn needs a phone number and login code,
just as any functional Telegram client would require. It only saves
API ID/HASH on the user's machine because these two are
required each time client initialization. But **under no
circumstances** would tg-focus save user's phone number or any
other credentials.


## 什么使得 tg-focus 成为一个温和的 User Bot？

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
principle: **all message collected by tg-focus will only be forwarded
to a newly created chat**. 

With this principle, it is firmly guaranteed that **no** messages
generated by tg-focus will be ever sent to the wrong chat. Additionally,
there is **no** reason for tg-focus users to worry about whether any
existing or new features will be harmful or not. By the time of
updates, tg-focus will remain **gentle**.

# 开发

See `docs/development.org`.

# 贡献

tg-focus is still far from perfect, any form of contribution is
welcomed! 

- Ask question (open an issue with label [![](https://img.shields.io/static/v1?label=&message=question&color=purple)](https://github.com/micl2e2/tg-focus/issues/new))
- Bug Report (open an issue with label [![](https://img.shields.io/static/v1?label=&message=bug&color=red)](https://github.com/micl2e2/tg-focus/issues/new))
- Feature Request (open an issue with label
  [![](https://img.shields.io/static/v1?label=&message=enhancement&color=lightblue)](https://github.com/micl2e2/tg-focus/issues/new))
- Documentation Improvement
- Code Contribution (tg-focus follows [GCC](https://gcc.gnu.org/) code style)



# 许可证

tg-focus is licensed under [GNU General Public License Version
3.0](https://github.com/micl2e2/tg-focus/blob/master/LICENSE-GPL). 

