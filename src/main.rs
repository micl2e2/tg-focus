use futures_util::future::{select, Either};
use futures_util::pin_mut;
use grammers_client::types;
use grammers_client::SignInError;
use grammers_client::{Client, Config, InitParams, Update};
use grammers_session::Session;
use log;
use simple_logger::SimpleLogger;
use std::env;
use std::pin::pin;
use tokio::{runtime, task};

use std::fs;

use tg_focus::init_data;
use tg_focus::CollectedMsg;
use tg_focus::TgFilters;
use tg_focus::WorkDir;

type Result<T> = std::result::Result<T, Box<dyn std::error::Error>>;

async fn handle_update(client: Client, update: Update) -> Result<()> {
    match update {
        Update::NewMessage(message) if !message.outgoing() => {
            let chat = message.chat();
            let mut sender = String::from("???");
            if let Some(types::chat::Chat::User(usr)) = message.sender() {
                sender = format!(
                    "{} @{}",
                    usr.full_name(),
                    usr.username().unwrap_or_else(|| "???")
                )
            }
            println!(
                "<{}> <{}> <{}>: {}",
                chat.name(),
                sender,
                message.date().format("%Y-%m-%d %H:%M:%S%z").to_string(),
                message.text(),
            );
            // client.send_message(&chat, message.text()).await?;
        }
        _ => {}
    }

    Ok(())
}

fn ask_user(hint: &str) -> String {
    println!("{}", hint);
    let mut input = String::new();
    std::io::stdin().read_line(&mut input).unwrap();
    String::from(input.trim())
}

async fn async_main() -> Result<()> {
    let wdir = init_data(None, false);

    SimpleLogger::new()
        .with_level(log::LevelFilter::Info)
        // .with_level(log::LevelFilter::Debug)
        .init()
        .unwrap();

    dbg!("waiting api id...");
    let may_api_id: Option<i32>;
    loop {
        let readres = fs::read_to_string(wdir.api_id()).unwrap(); // FIXME: too many io
        if let Ok(got_api_id) = readres.trim().parse::<i32>() {
            may_api_id = Some(got_api_id);
            break;
        }
    }

    dbg!("waiting api hash...");
    let may_api_hash: Option<String>;
    loop {
        let readres = fs::read_to_string(wdir.api_hash()).unwrap();
        if readres.trim().len() > 0 {
            may_api_hash = Some(readres.trim().to_string());
            break;
        }
    }

    dbg!("waiting phone...");
    let may_phone: Option<String>;
    loop {
        let readres = fs::read_to_string(wdir.phone()).unwrap();
        if readres.trim().len() > 0 {
            may_phone = Some(readres.trim().to_string());
            break;
        }
    }

    let api_id = may_api_id.unwrap();
    let api_hash = may_api_hash.unwrap();
    let phone = may_phone.unwrap();

    dbg!((&api_id, &api_hash, &phone));

    log::info!("Connecting to Telegram...");
    let client = Client::connect(Config {
        session: Session::load_file_or_create(wdir.session())?,
        api_id,
        api_hash: api_hash.clone(),
        params: InitParams {
            // Fetch the updates we missed while we were offline
            catch_up: true,
            ..Default::default()
        },
    })
    .await?;
    println!("Connected!");

    if !client.is_authorized().await? {
        println!("Signing in...");
        let token = client.request_login_code(&phone, api_id, &api_hash).await?;

        dbg!("waiting phone...");
        let may_vcode: Option<String>;
        loop {
            let readres = fs::read_to_string(wdir.vcode()).unwrap();
            if readres.trim().len() > 0 {
                may_vcode = Some(readres.trim().to_string());
                break;
            }
        }
        let vcode = may_vcode.unwrap();

        // let vcode = ask_user("Please enter six-digit login code:");
        let user = match client.sign_in(&token, &vcode).await {
            Ok(user) => user,
            Err(SignInError::PasswordRequired(_token)) => panic!("Please provide a password"),
            Err(SignInError::SignUpRequired {
                terms_of_service: tos,
            }) => panic!("Sign up required"),
            Err(err) => {
                println!("Failed to sign in as a user: {}", err);
                return Err(err.into());
            }
        };

        println!("Signed in as {}!", user.first_name());

        client.session().save_to_file(wdir.session())?;
        println!("Signed in!");
    }

    println!("Waiting for messages...");

    loop {
        let update = {
            let exit = async { tokio::signal::ctrl_c().await };
            let upd = async { client.next_update().await };
            pin_mut!(exit);
            pin_mut!(upd);

            match select(exit, upd).await {
                Either::Left((_, _)) => None,
                Either::Right((u, _)) => Some(u),
            }
        };

        let update = match update {
            None | Some(Ok(None)) => break,
            Some(u) => u?.unwrap(),
        };

        let handle = client.clone();
        task::spawn(async move {
            if let Err(e) = handle_update(handle, update).await {
                eprintln!("Error handling updates!: {}", e)
            }
            // match handle_update(handle, update).await {
            //     Ok(_) => {}
            //     Err(e) => eprintln!("Error handling updates!: {}", e),
            // }
        });
    }

    println!("Saving session file and exiting...");
    client.session().save_to_file(wdir.session())?;
    Ok(())
}

fn main() -> Result<()> {
    runtime::Builder::new_current_thread()
        .enable_all()
        .build()
        .unwrap()
        .block_on(async_main())
}
