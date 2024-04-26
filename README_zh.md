# TG-Focus

![GitHub License](https://img.shields.io/github/license/micl2e2/tg-focus?logo=gnu&color=blue&link=https%3A%2F%2Fgithub.com%2Fmicl2e2%2Ftg-focus%2Fblob%2Fmaster%2FLICENSE-GPL)
![Docker Image Size (tag)](https://img.shields.io/docker/image-size/micl2e2/tg-focus/latest?logo=docker&logoColor=white&color=lightgreen&link=https%3A%2F%2Fhub.docker.com%2Fr%2Fmicl2e2%2Ftg-focus)
![GitHub commits difference between two branches/tags/commits](https://img.shields.io/github/commits-difference/tdlib/td?base=cd25b29452644856b7472dd0cd69391b2226d3c3&head=master&logo=gitextensions&label=pending%20TDLib%20updates&color=lightblue&link=https%3A%2F%2Fgithub.com%2Ftdlib%2Ftd)

[**English**](README.md)&nbsp;&nbsp;&nbsp;[**简体中文**](README_zh.md)

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

它支持多种形式的文本消息：

1. 聊天消息（原始/回复/转发/引用）
2. 媒体标题
3. Emoji 表情符号
4. 频道更新

通过定制 *Focus 过滤器*，tg-focus 可以将任何特定的消息从任何聊天转发到一个名为 *TG-FOCUS*  的专用聊天中，这样用户就可以完全控制他们看到的消息。

Telegram 以其开放性而闻名。虽然用户可以在任何公共聊天室中自由发言，但他们同样应该有权选择他们想看到的内容。Telegram 的官方客户端已经很好地支持了前者，tg-focus 和其他类似工具则试图填补后者的空白。

显著特点：

-   **可靠性和一致性**：tg-focus 直接构建在官方 C++ 接口（[TDLib](https://core.telegram.org/tdlib/)）之上。所有与 Telegram 服务器的通信完全由 TDLib 处理。

-   **易于定制**：用户可以随时自定义 *Focus Filter* 。该过滤器接受简单文本和 [正则表达式](https://en.wikipedia.org/wiki/Regular_expression) 。配置文件采用人类可读的 [TOML](https://toml.io/en/) 格式。

-   **随时部署**：tg-focus 以三种形式发布：源代码、Docker 镜像和预编译的二进制文件。

# 一个简单的演示

以下是一个常见用例的简单 9 步演示：转发包含我们想要的关键词的消息，忽略来自我们不想要的用户发送的消息。

<img src="demo/zh/1.png" width="800px"/>

<img src="demo/zh/2.png" width="800px"/>

<img src="demo/zh/3.png" width="800px"/>

[回到开始](#一个简单的演示)

<img src="demo/zh/4.png" width="800px"/>

[回到开始](#一个简单的演示)

<img src="demo/zh/5.png" width="800px"/>

[回到开始](#一个简单的演示)

<img src="demo/zh/6.png" width="800px"/>

[回到开始](#一个简单的演示)

<img src="demo/zh/7.png" width="800px"/>

[回到开始](#一个简单的演示)

<img src="demo/zh/8.png" width="800px"/>

[回到开始](#一个简单的演示)

<img src="demo/zh/9.png" width="800px"/>

[回到开始](#一个简单的演示)


# 支持的平台

| 操作系统    | 是否支持(x86_64) |
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

**注意**：这不是一个详尽的列表，您可以通过发起 issue 或 pull request 来添加缺失的平台。


# 支持的语言

| 语言                 | 状态                                    |
|--------------------------|-------------------------------------------|
| 英文（香港）     | ✅                                        |
| 英文（美国）  | ✅                                        |
| 英文（津巴布韦）       | ✅                                        |
| 中文（大陆） | ✅                                        |
| 中文（香港） | ✅                                        |
| 其他                   | [添加您的语言](#支持的语言) |
|                          |                                           |

**注意**：可支持更多语言！请根据国际化指示文档（ `docs/i18n.org` ）来添加您的语言。

# 如何使用

在您做任何事情之前，请确保您有：

1. 一个 Telegram 账号
2. 一个 Telegram API ID 和 the 相应的 API HASH，可从 https://my.telegram.org 获取。 

## 硬件要求

硬件上没有严格的限制，这里是 7 天内的内存使用情况概览：

| 第 1 天 | 第 2 天 | 第 3 天 | 第 4 天 | 第 5 天  | 第 6 天  | 第 7 天  |
|------|------|------|------|-------|-------|-------|
| 76MB | 84MB | 90MB | 96MB | 103MB | 108MB | 115MB |

（注意：此表仅供参考，它是由一个拥有约 30 个公共聊天室的账号记录的，其中大多数聊天室都有超过 1000 名成员，并且每天产生 500 条消息）


## 部署

有两种不同的部署方式：

### 使用预编译二进制文件

**步骤 1：**

请在 [GitHub 发布页面](https://github.com/micl2e2/tg-focus/releases) 下载二进制文件。请检查您的平台是否受支持。支持的平台列表请参见 [支持的平台](#支持的平台) 部分。

**步骤 2：**

请先登录您的 Telegram 帐户，需要 API ID、API HASH、您的电话号码（国际格式，不能包含空格，例如：如果您的国家代码是 `+1` 且电话号码是 `234567` ，则输入 `+1234567` ），以及一个登陆验证码。前两者可以在 https://my.telegram.org 中获取。

```sh
/path/to/tgf-conf auth
```

如果您成功登录，控制台最后输出的三行将是：

```
[tgf INFO] Log in successfully!
[tgf INFO] Closing...
[tgf INFO] Closed
```

**步骤 3：（可选）**

 使用默认语言（英语）以外的其他语言：

```sh
/path/to/tgf-conf lang XX-YY
```

注意：XX 为语言代码，YY 为地区代码，例如：“en-US”代表
“英语（美国）”、“en-GB”代表“英语（英国）”等。
所有有效代码均可在以下位置找到：

http://www.lingoes.net/en/translator/langcode.htm 

**步骤 4：**

启动 tg-focus 守护进程：

```sh
# 在前台运行
/path/to/tgf-focusd

# 或在后台运行
nohup /path/to/tgf-focusd &
```

**步骤 5：**

如果您成功登录，tg-focus 将会创建一个名为 `TG-FOCUS` 的新聊天室，随后的消息将被过滤并转发到该聊天室中。

**步骤 6：**

请注意，默认情况下，tg-focus 将转发您收到的**所有**消息。您可以随时更改 *Focus 过滤器*（过滤器配置），以定制您想要在该聊天中接收的消息。请参阅 [示例](#过滤器示例)。

```sh
/path/to/tgf-conf filters
```

（注意：该操作将会打开 [GNU nano](https://www.nano-editor.org/) 编辑器进行配置修改。修改完成后，按 **Ctrl-O**、**Enter**、**Ctrl-X** 保存。只要配置有效，控制台输出的最后一行应为“Saving filters...”）

**步骤 7：**

完成。


### 使用 Docker 镜像

请确保您的计算机上已安装 [Docker](https://docs.docker.com/engine/) 或 [Podman](https://podman.io/) 。

**步骤 1：**

拉取镜像并在后台运行，假设新创建的容器名称为 CONTAINER-NAME：

```sh
docker pull docker.io/micl2e2/tg-focus:latest
docker run -d docker.io/micl2e2/tg-focus
```

**步骤 2：**

请先登录您的 Telegram 帐户，需要 API ID、API HASH、您的电话号码（国际格式，不能包含空格，例如：如果您的国家代码是 `+1` 且电话号码是 `234567` ，则输入 `+1234567` ），以及一个登陆验证码。前两者可以在 https://my.telegram.org 中获取。

```sh
docker exec -it CONTAINER-NAME tgf-conf auth
```

如果您成功登录，控制台最后输出的三行将是：

```
[tgf INFO] Log in successfully!
[tgf INFO] Closing...
[tgf INFO] Closed
```

**步骤 3：（可选）**

 使用默认语言（英语）以外的其他语言：

```sh
docker exec -it CONTAINER-NAME tgf-conf lang XX-YY
```

注意：XX 为语言代码，YY 为地区代码，例如：“en-US”代表
“英语（美国）”、“en-GB”代表“英语（英国）”等。
所有有效代码均可在以下位置找到：

http://www.lingoes.net/en/translator/langcode.htm 

**步骤 4：**

如果您成功登录，tg-focus 将会创建一个名为 `TG-FOCUS` 的新聊天室，随后的消息将被过滤并转发到该聊天室中。

**步骤 5：**

请注意，默认情况下，tg-focus 将转发您收到的**所有**消息。您可以随时更改 *Focus 过滤器*（过滤器配置），以定制您想要在该聊天中接收的消息。请参阅 [示例](#过滤器示例)。

```sh
docker exec -it CONTAINER-NAME tgf-conf filters
```

（注意：该操作将会打开 [GNU nano](https://www.nano-editor.org/) 编辑器进行配置修改。修改完成后，按 **Ctrl-O**、**Enter**、**Ctrl-X** 保存。只要配置有效，控制台输出的最后一行应为“Saving filters...”）

**步骤 6：**

完成。

**步骤 7：（可选）**

如果您要销毁容器实例及其与 Telegram 相关的凭据和资源，请使用以下命令：

```sh
docker rm --force CONTAINER-NAME
```


# 过滤规则

一个 *Focus 过滤器*  是由 tg-focus 使用的过滤器，用于匹配消息的文本内容（文本、表情符号或媒体标题）。它们包括：
- `title`
- `keywords`
- `no-keywords`
- `senders`
- `no-senders`
- `rej-senders`

用户可以添加任意多的过滤器，它们能使用**简单文本**或**正则表达式**进行匹配。

当收到一条消息时，它们将逐个尝试，如果有任何过滤器可以*匹配*该消息而不是*跳过*该消息，则会被转发。

当前 *Focus 过滤器* 可*匹配*：

- 聊天人/聊天室/频道标题（`title`）
- 关键字（`keywords`）
- 发送者（`senders`）

可*跳过*：

- 关键字（`no-keywords`）
- 发送者（`no-senders`）

可*拒绝*：

- 发送者（`rej-senders`）

一条消息**如果且只有**触发以下条件时，才会被转发：

- 它被一条 *Focus 过滤器*  匹配，**并且**
- 它没有被该 *Focus 过滤器*  跳过，**并且**
- 它没有被任何先前的 *Focus 过滤器*  拒绝。

（请注意，前三条规则可以被视为**白名单**，后两条规则可以被视为**弱黑名单**，最后一条规则可以被视为**强黑名单**。）


## 过滤器示例

我们想要转发“👍很棒的聊天👍”聊天室中的所有消息。

有效，聊天标题完全匹配：

```toml
[[focus-filter]]
title = "👍很棒的聊天室👍"
```

有效，聊天标题部分匹配：

```toml
[[focus-filter]]
title = "很棒的聊天室"
```

有效，聊天标题部分匹配： 

```toml
[[focus-filter]]
title = "很棒"
```

有效，聊天标题部分匹配：

```toml
[[focus-filter]]
title = "👍"
```

有效，正则表达式与聊天标题匹配：

```toml
[[focus-filter]]
title = ".*很棒.*"
```

我们想要转发包含“足球”但不包含“篮球”的消息，来自名为“体育交流群”的聊天。我们可以将过滤器写成

```toml
[[focus-filter]]
title = "体育交流群"
keywords = ["足球", "⚽"]
no-keywords = ["篮球"]
```

我们想要包含“足球”或任何其他消息，但不包括包含“篮球”的消息，以及由全名为“篮球爱好者”的用户发送的消息，或由用户ID为“alice_love_basketball”的用户发送的消息，在名为“体育交流群”的聊天中。我们可以将过滤器写成：

```toml
# 第一条过滤器
[[focus-filter]]
title = "体育交流群"
keywords = ["足球", "⚽"]

# 第二条过滤器
[[focus-filter]]
title = "体育交流群"
no-keywords = ["篮球"]
no-senders = ["篮球爱好者", "@alice_love_basketball"]
```

这些消息将被转发：

- `Alice: 我爱足球！` （被第一条过滤器匹配）
- `Bob: 我踢⚽。` （被第一条过滤器匹配）
- `Alice: 我擅长游泳。` （被第二条过滤器匹配）
- `Bob: 我有时候打羽毛球。` （被第二条过滤器匹配）
- `Alice: 我踢足球和打羽毛球。` （被第一条过滤器匹配，跳过第二条过滤器）

这些消息将不会被转发：

- `Alice: 我打篮球和羽毛球。` （被第二条过滤器“no-keywords”匹配，相当于*拒绝*）
- `篮球爱好者: Hellop everyone!` （被第二条过滤器*拒绝*）
- `Alice @alice_love_basketball: 你们好，我是 Alice ，我热爱体育。`
  （被第二条过滤器*拒绝*）



# Q&A

## 我能相信 tg-focus 吗？

绝对可以！

首先，考虑到 tg-focus 是一款遵循 GPL 许可证的[自由软件](https://www.gnu.org/philosophy/free-sw.zh-cn.html)。

tg-focus 继承了 TDLib 的开放性，其源代码以及依赖项的源代码都是完全开放的。此外，所有二进制文件和 OCI 镜像将**不会**由任何个人构建，而是由托管在 Github 上的机器构建。所有构建细节可以在专用存储库 [tg-focus-cicd](https://github.com/micl2e2/tg-focus-cicd) 中找到。

Telegram bot 对于某些特定任务非常有用，但并不足以成为一个**全面**的消息过滤器。因此，在授权过程中，就像任何完备的 Telegram 客户端所需一样，tg-focus 需要登录用户的账号，所以需要手机号码和登录验证码。它只会在用户设备上保存 API ID/HASH ，因为每次客户端初始化都需要这两者。
但 **无论如何** tg-focus 都不会保存用户手机号码或其他凭据。


## 什么使得 tg-focus 成为一个温和的 User Bot（用户机器人）？

TDLib 声称它是 _一个用于构建 Telegram 客户端的库_。通过嵌入 TDLib ，就像任何其他 User Bot 一样，tg-focus 能够使用所有 TDLib 的公共 API 。虽然这为许多强大功能打开了可能性，但也意味着 User Bot 比通用用途 [Bot](https://core.telegram.org/bots) 拥有更多权限。这可能是一把双刃剑。

已经有一些例子表明，一些无恶意的 User Bot 由于滥用 Telegram API 而令其用户失望，例如反复向用户的随机聊天发送随机消息或其他不受控制的行为。这很可能是Telegram User Bot 声誉不佳的原因之一。

然而， tg-focus 将不会走他们的路线，我们有一个严格原则：**tg-focus 收集到的所有消息都只会被转发到新创建的聊天中**。

遵循此原则可以坚定保证 **tg-focus 生成的任何消息都绝对不会被发送到错误聊天中**。此外，没有理由让 tg-focus 用户担心任何现有或新功能是否具有危害性。在更新时， tg-focus 将始终保持**温和**。

# 开发

见 `docs/development.org`。

# 贡献

tg-focus 仍有改进空间，我们欢迎任何形式的贡献！

- 询问问题（使用标签 [![](https://img.shields.io/static/v1?label=&message=question&color=purple)](https://github.com/micl2e2/tg-focus/issues/new) 发起 issue ）
- 报告错误（使用标签 [![](https://img.shields.io/static/v1?label=&message=bug&color=red)](https://github.com/micl2e2/tg-focus/issues/new) 发起 issue ）
- 请求功能（使用标签
  [![](https://img.shields.io/static/v1?label=&message=enhancement&color=lightblue)](https://github.com/micl2e2/tg-focus/issues/new) 发起 issue ）
- 完善文档
- 贡献代码（ tg-focus 遵循 [GCC](https://gcc.gnu.org/) 代码风格）



# 许可证

tg-focus 是根据 [GNU通用公共许可证第3版](https://github.com/micl2e2/tg-focus/blob/master/LICENSE-GPL) 授权许可的。

