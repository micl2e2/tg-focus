use serde::Deserialize;

#[derive(Debug)]
pub struct CollectedMsg<'a> {
    pub title: &'a str,
    pub sender: &'a str,
    pub ctn: &'a str,
    pub tstamp: &'a str,
}

#[derive(Debug, Deserialize)]
struct TgFilters {
    filter: Vec<TgFilter>,
}

impl TgFilters {
    fn is_match(&self, msg: &CollectedMsg) -> bool {
        for filter in &self.filter {
            if let Some(title) = &filter.title {
                if msg.title.contains(title) {
                    return true;
                }
            }
        }

        return false;
    }
}

#[derive(Debug, Deserialize)]
struct TgFilter {
    title: Option<String>,
    keyword: Option<Vec<String>>,
    no_keyword: Option<Vec<String>>,
    // title: Option<Regex>,
    // keyword: Option<Vec<Regex>>,
    // no_keyword: Option<Vec<Regex>>,
}

// mod deser {
//     use super::*;
//     use serde::de::{Deserialize, Deserializer, Error as DeError, IgnoredAny, MapAccess, Visitor};

//     enum Fields {
//         Title,
//         Keyword,
//         NoKeyWord,
//     }

//     const FIELD_JSON_NAMES: &[&str] = &["title", "keyword"];
// }

#[cfg(test)]
mod utst {
    use super::*;

    #[test]
    fn _1() {
        let input = r#"

[[filter]]
title = "title1"
keyword = ["kw1", "kw2"]

[[filter]]
title = "title2"
no_keyword = ["kw3", "kw4"]

"#;

        let filters = toml::from_str::<TgFilters>(input).unwrap();
        dbg!(&filters);

        let msg = CollectedMsg {
            title: "title123",
            sender: "xx",
            ctn: "xx",
            tstamp: "xx",
        };

        assert!(filters.is_match(&msg));
    }
}
