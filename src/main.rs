use chrono::{Local, TimeZone};
use std::fs;
use std::sync::{
    atomic::{AtomicBool, Ordering},
    Arc,
};
use tdlib::enums;
use tdlib::enums::{AuthorizationState, Update};
use tdlib::functions;
use tdlib::types;
use tokio::sync::mpsc::{self, Receiver, Sender};

async fn chat2str(chat_id: i64, client_id: i32) -> String {
    if let Ok(enums::Chat::Chat(chatinfo)) = functions::get_chat(chat_id, client_id).await {
        chatinfo.title
    } else {
        String::default()
    }
}

async fn date2str(tstamp: i32) -> String {
    Local
        .timestamp_opt(tstamp.into(), 0)
        .unwrap()
        .format("%Y-%m-%d %H:%M:%S")
        .to_string()
}

async fn sender2str(sender_id: enums::MessageSender, client_id: i32) -> String {
    let mut ret = String::from("???");
    match sender_id {
        enums::MessageSender::User(types::MessageSenderUser { user_id }) => {
            if let Ok(enums::User::User(types::User {
                first_name,
                last_name,
                usernames,
                ..
            })) = functions::get_user(user_id, client_id).await
            {
                let username = if let Some(types::Usernames {
                    editable_username, ..
                }) = usernames
                {
                    editable_username
                } else {
                    String::from("-")
                };
                ret = format!("<{} {} {}>", first_name, last_name, username);
            }
        }
        enums::MessageSender::Chat(types::MessageSenderChat { chat_id }) => {
            ret = format!("<chat {}>", chat_id);
        }
    }

    ret
}

async fn handle_update(
    update: Update,
    auth_tx: &Sender<AuthorizationState>,
    mq_tx: &Sender<(i64, enums::MessageSender, String, i32)>,
) {
    match update {
        Update::AuthorizationState(update) => {
            auth_tx.send(update.authorization_state).await.unwrap();
        }

        Update::NewMessage(types::UpdateNewMessage { message }) => match message.content {
            enums::MessageContent::MessageText(types::MessageText {
                text: types::FormattedText { text, .. },
                ..
            }) => {
                if let Ok(_) = mq_tx
                    .send((
                        message.chat_id,
                        message.sender_id,
                        text.clone(),
                        message.date,
                    ))
                    .await
                {}
            }
            enums::MessageContent::MessagePhoto(types::MessagePhoto {
                caption: types::FormattedText { text, .. },
                ..
            }) => {
                if let Ok(_) = mq_tx
                    .send((
                        message.chat_id,
                        message.sender_id,
                        text.clone(),
                        message.date,
                    ))
                    .await
                {}
            }
            _o => {
                dbg!(("message IGNORED", _o));
            }
        },

        _other => {
            // dbg!("message OTHER...");
            // dbg!(_other);
        }
    }
}

async fn handle_authorization_state(
    client_id: i32,
    api_id: i32,
    api_hash: String,
    mut auth_rx: Receiver<AuthorizationState>,
    run_flag: Arc<AtomicBool>,
) -> Receiver<AuthorizationState> {
    dbg!(223344);
    while let Some(state) = auth_rx.recv().await {
        match state {
            AuthorizationState::WaitTdlibParameters => {
                let response = functions::set_tdlib_parameters(
                    false,
                    "/tmp/tsttdlibs_db".into(),
                    String::new(),
                    String::new(),
                    false,
                    false,
                    false,
                    false,
                    // 12345,
                    api_id,
                    // env!("API_ID").parse().unwrap(),
                    // env!("API_HASH").into(),
                    api_hash.clone(),
                    String::from("en"),
                    String::from("Desktop"),
                    String::new(),
                    env!("CARGO_PKG_VERSION").into(),
                    false,
                    true,
                    client_id,
                )
                .await;

                if let Err(error) = response {
                    println!("{}", error.message);
                }
            }

            AuthorizationState::WaitPhoneNumber => {
                let may_phone: Option<String>;
                loop {
                    let readres = fs::read_to_string("/tmp/tg-focus/phone").unwrap();
                    if readres.trim().len() > 0 {
                        may_phone = Some(readres.trim().to_string());
                        break;
                    }
                    dbg!("waiting phone...");
                    tokio::time::sleep(std::time::Duration::from_secs(1)).await;
                }
                let response =
                    functions::set_authentication_phone_number(may_phone.unwrap(), None, client_id)
                        .await;
                match response {
                    Ok(_) => {
                        dbg!("phone ok");
                    }
                    Err(_e) => {
                        if _e.message.contains("API_ID_INVALID") {
                            dbg!("try other api id");
                        } else {
                            dbg!(_e);
                        }
                    }
                }
            }

            AuthorizationState::WaitCode(_) => {
                let may_vcode: Option<String>;
                loop {
                    let readres = fs::read_to_string("/tmp/tg-focus/vcode").unwrap();
                    if readres.trim().len() > 0 {
                        may_vcode = Some(readres.trim().to_string());
                        break;
                    }
                    dbg!("waiting vcode...");
                    tokio::time::sleep(std::time::Duration::from_secs(1)).await;
                }
                let response =
                    functions::check_authentication_code(may_vcode.unwrap(), client_id).await;
                match response {
                    Ok(_) => {
                        dbg!("vcode ok");
                    }
                    Err(_e) => {
                        dbg!(_e);
                    }
                }
            }

            AuthorizationState::Ready => {
                break;
            }

            AuthorizationState::Closed => {
                // Set the flag to false to stop receiving updates from the
                // spawned task
                dbg!(112233);
                run_flag.store(false, Ordering::Release);
                break;
            }

            _xxx => {
                dbg!(_xxx);
            }
        }
    }

    auth_rx
}

#[tokio::main]
async fn main() {
    fs::create_dir_all("/tmp/tg-focus").unwrap();
    fs::OpenOptions::new()
        .read(true)
        .write(true)
        .create(true)
        .open("/tmp/tg-focus/api_id")
        .unwrap();
    fs::OpenOptions::new()
        .read(true)
        .write(true)
        .create(true)
        .open("/tmp/tg-focus/api_hash")
        .unwrap();
    fs::OpenOptions::new()
        .read(true)
        .write(true)
        .create(true)
        .open("/tmp/tg-focus/phone")
        .unwrap();
    fs::OpenOptions::new()
        .read(true)
        .write(true)
        .create(true)
        .open("/tmp/tg-focus/vcode")
        .unwrap();

    let may_api_id: Option<i32>;
    loop {
        let readres = fs::read_to_string("/tmp/tg-focus/api_id").unwrap();
        if let Ok(got_api_id) = readres.trim().parse::<i32>() {
            may_api_id = Some(got_api_id);
            break;
        }
        dbg!("waiting api id...");
        tokio::time::sleep(std::time::Duration::from_secs(1)).await;
    }

    let may_api_hash: Option<String>;
    loop {
        let readres = fs::read_to_string("/tmp/tg-focus/api_hash").unwrap();
        if readres.trim().len() > 0 {
            may_api_hash = Some(readres.trim().to_string());
            break;
        }
        dbg!("waiting api hash...");
        tokio::time::sleep(std::time::Duration::from_secs(1)).await;
    }

    dbg!(&may_api_id, &may_api_hash);

    // Create the client object
    let client_id = tdlib::create_client();

    // Create a mpsc channel for handling AuthorizationState updates separately
    // from the task
    let (auth_tx, auth_rx) = mpsc::channel(5);
    let (mq_tx, mut mq_rx) = mpsc::channel(5);

    // Create a flag to make it possible to stop receiving updates
    let run_flag = Arc::new(AtomicBool::new(true));
    let run_flag_clone = run_flag.clone();

    // Spawn a task to receive updates/responses
    let handle = tokio::spawn(async move {
        while run_flag_clone.load(Ordering::Acquire) {
            if let Some((update, _client_id)) = tdlib::receive() {
                handle_update(update, &auth_tx, &mq_tx).await;
            }
        }
    });

    // Set a fairly low verbosity level. We mainly do this because tdlib
    // requires to perform a random request with the client to start receiving
    // updates for it.
    functions::set_log_verbosity_level(1, client_id)
        .await
        .unwrap();

    // Handle the authorization state to authenticate the client
    let auth_rx = handle_authorization_state(
        client_id,
        may_api_id.unwrap(),
        may_api_hash.unwrap(),
        auth_rx,
        run_flag.clone(),
    )
    .await;

    // create a new collector, with title and timestamp
    let may_got_chat = functions::create_new_basic_group_chat(
        vec![],
        chrono::Local::now()
            .format("TG-FOCUS %Y-%m-%d %H:%M:%S%z")
            .to_string(),
        0,
        client_id,
    )
    .await;

    if let Ok(got_chat) = may_got_chat {
        let tdlib::enums::Chat::Chat(chat_meta) = got_chat;

        // if let tdlib::enums::Chat::Chat(chat_meta) = got_chat {
        let collector_chat_id = chat_meta.id;

        while let Some((chat_id, sender_id, msg_ctn, date)) = mq_rx.recv().await {
            let chat_title = chat2str(chat_id, client_id).await;
            if chat_title.contains("TG-FOCUS") {
                continue;
            }
            let sender_name = sender2str(sender_id, client_id).await;
            let date = date2str(date).await;
            let coll_msg = CollectedMsg {
                title: &chat_title,
                sender: &sender_name,
                ctn: &msg_ctn,
                tstamp: &date,
            };

            if coll_msg.is_ctn_interesting() {
                collect_msg(coll_msg.to_string(), collector_chat_id, client_id).await;
            }
        }
        // }
    }

    tokio::time::sleep(std::time::Duration::from_secs(3000)).await;

    // Tell the client to close
    functions::close(client_id).await.unwrap();

    // Handle the authorization state to wait for the "Closed" state
    handle_authorization_state(client_id, 0, "".into(), auth_rx, run_flag.clone()).await;

    // Wait for the previously spawned task to end the execution
    handle.await.unwrap();
}

#[derive(Debug)]
struct CollectedMsg<'a> {
    title: &'a str,
    sender: &'a str,
    ctn: &'a str,
    tstamp: &'a str,
}

impl CollectedMsg<'_> {
    fn to_string(&self) -> String {
        format!(
            "
CHAT : {}
SENDER : {}
CTN : {}
DATE : {}
                ",
            self.title, self.sender, self.ctn, self.tstamp,
        )
        .to_string()
    }

    fn is_ctn_interesting(&self) -> bool {
        let relist = [
            regex::Regex::new("Ubuntu").unwrap(),
            regex::Regex::new("linux").unwrap(),
        ];

        for re in relist {
            // FIXME: remove me
            if re.is_match(self.title) {
                return true;
            }
            if re.is_match(self.ctn) {
                return true;
            }
        }

        return false;
    }
}

async fn collect_msg(msg: String, chat_id: i64, client_id: i32) {
    let mut msg_to_send = types::InputMessageText::default();
    msg_to_send.text.text = msg;

    let res_sent = functions::send_message(
        chat_id,
        0,
        None,
        None,
        None,
        enums::InputMessageContent::InputMessageText(msg_to_send),
        client_id,
    )
    .await;

    dbg!(res_sent.is_ok());

    tokio::time::sleep(std::time::Duration::from_millis(300)).await;
}
