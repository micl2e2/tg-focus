use std::fs;
use std::io::prelude::*;
use std::path::PathBuf;

const DEFAULT_DATA_ROOT: &str = "/tmp/tg-focus";
const FILE_API_ID: &str = "api_id";
const FILE_API_HASH: &str = "api_hash";
const FILE_PHONE: &str = "phone";
const FILE_VCODE: &str = "vcode";
const FILE_FILTER: &str = "filter";
const FILE_SESSION: &str = "session";

#[derive(Debug, Clone)]
pub struct WorkDir {
    root: PathBuf,
}

impl WorkDir {
    pub fn file_in_cwd(&self, file_name: &str) -> PathBuf {
        PathBuf::from(file_name)
    }

    pub fn api_id(&self) -> PathBuf {
        self.root.join(FILE_API_ID)
    }

    pub fn api_hash(&self) -> PathBuf {
        self.root.join(FILE_API_HASH)
    }

    pub fn phone(&self) -> PathBuf {
        self.root.join(FILE_PHONE)
    }

    pub fn vcode(&self) -> PathBuf {
        self.root.join(FILE_VCODE)
    }

    pub fn filter(&self) -> PathBuf {
        self.root.join(FILE_FILTER)
    }

    pub fn session(&self) -> PathBuf {
        self.root.join(FILE_SESSION)
    }
}

// cannot-fail operation
pub fn init_data(pred_root: Option<&str>, reset: bool) -> WorkDir {
    let actual_root = if let Some(v) = pred_root {
        PathBuf::from(v)
    } else {
        PathBuf::from(DEFAULT_DATA_ROOT)
    };

    if reset {
        match fs::remove_dir_all(&actual_root) {
            Ok(_) => {}
            Err(_) => {}
        }
    }
    fs::create_dir_all(&actual_root).unwrap();

    let wdir = WorkDir { root: actual_root };

    // fs::create_dir_all(wdir.tddb()).unwrap();
    fs::OpenOptions::new()
        .read(true)
        .write(true)
        .create(true)
        .open(wdir.api_id())
        .unwrap();
    fs::OpenOptions::new()
        .read(true)
        .write(true)
        .create(true)
        .open(wdir.api_hash())
        .unwrap();
    fs::OpenOptions::new()
        .read(true)
        .write(true)
        .create(true)
        .open(wdir.phone())
        .unwrap();
    fs::OpenOptions::new()
        .read(true)
        .write(true)
        .create(true)
        .open(wdir.vcode())
        .unwrap();
    // do not create session
    // fs::OpenOptions::new()
    //     .read(true)
    //     .write(true)
    //     .create(true)
    //     .open(wdir.session())
    //     .unwrap();
    let mut f_flt = fs::OpenOptions::new()
        .read(true)
        .write(true)
        .create(true)
        .open(wdir.filter())
        .unwrap();

    let mut buf = [0; 4096];
    let mut ntotal = 0;
    let mut nread = 1;
    while nread > 0 {
        nread = f_flt.read(&mut buf).unwrap();
        ntotal += nread;
    }
    let s = String::from_utf8_lossy(&buf[0..ntotal]);
    if s.trim().len() < 10 {
        f_flt.rewind().unwrap();
        f_flt
            .write_all(
                r#"[[filter]]
title = ".*"
"#
                .as_bytes(),
            )
            .unwrap();
    }

    wdir
}

#[cfg(test)]
mod utst {
    use super::*;

    #[test]
    fn _1() {
        let wdir = init_data(Some("/tmp/tsttgfocus"), true);

        // let dir_entry_list = fs::read_dir(wdir.tddb()).unwrap();
        // assert_eq!(dir_entry_list.count(), 0);

        let file_ctn = fs::read_to_string(wdir.api_id()).unwrap();
        assert_eq!(file_ctn, r#""#);

        let file_ctn = fs::read_to_string(wdir.api_hash()).unwrap();
        assert_eq!(file_ctn, r#""#);

        let file_ctn = fs::read_to_string(wdir.phone()).unwrap();
        assert_eq!(file_ctn, r#""#);

        let file_ctn = fs::read_to_string(wdir.vcode()).unwrap();
        assert_eq!(file_ctn, r#""#);

        if let Ok(true) = wdir.session().try_exists() {
            assert!(false, "session should not exist at first init");
        }

        let file_ctn = fs::read_to_string(wdir.filter()).unwrap();
        assert_eq!(
            file_ctn,
            r#"[[filter]]
title = ".*"
"#
        );
    }
}
