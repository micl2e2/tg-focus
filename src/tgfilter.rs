use regex::Regex;
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
    fn is_match(&self, msg: &CollectedMsg) -> (bool, usize) {
        let mut i_stay = 0;
        let mut decide_match = false;

        for i in 0..self.filter.len() {
            i_stay = i;
            let mut is_match = false;

            let filter = &self.filter[i];

            // title: not matching means msg not matching
            if let Some(title_f) = &filter.title {
                if title_f.is_match(msg.title) {
                    is_match = true;
                }
                if !is_match {
                    continue; // big
                }
            }

            // keyword: matching any means msg matching
            if let Some(kw_f_list) = &filter.keyword {
                let mut sub_match = false;
                for kw_f in kw_f_list {
                    if kw_f.is_match(msg.ctn) {
                        sub_match = true;
                        break; // small
                    }
                }
                is_match = sub_match;
            }

            // no_keyword: matching any means msg not matching
            if let Some(nkw_f_list) = &filter.no_keyword {
                for nkw_f in nkw_f_list {
                    if nkw_f.is_match(msg.ctn) {
                        is_match = false;
                        break; // small
                    }
                }
            }
            // last one done

            if is_match {
                decide_match = true;
                break;
            }
        }

        return (decide_match, i_stay);
    }
}

#[derive(Debug, Default)]
struct TgFilter {
    title: Option<Regex>,
    sender: Option<Vec<Regex>>,
    no_sender: Option<Vec<Regex>>,
    keyword: Option<Vec<Regex>>,
    no_keyword: Option<Vec<Regex>>,
}

mod deser {
    use super::*;
    use serde::de::{
        Deserialize, Deserializer, Error as DeError, IgnoredAny, MapAccess, Unexpected, Visitor,
    };

    enum Fields {
        Title,
        Sender,
        NoSender,
        Keyword,
        NoKeyword,
    }

    const FIELD_JSON_NAMES: &[&str] = &["title", "sender", "no_sender", "keyword", "no_keyword"];

    impl Fields {
        fn from_str(s: &str) -> Self {
            if s == FIELD_JSON_NAMES[0] {
                return Fields::Title;
            }
            if s == FIELD_JSON_NAMES[1] {
                return Fields::Sender;
            }
            if s == FIELD_JSON_NAMES[2] {
                return Fields::NoSender;
            }
            if s == FIELD_JSON_NAMES[3] {
                return Fields::Keyword;
            }
            if s == FIELD_JSON_NAMES[4] {
                return Fields::NoKeyword;
            }

            unreachable!()
        }
        fn to_sstr(&self) -> &'static str {
            match self {
                Fields::Title => FIELD_JSON_NAMES[0],
                Fields::Sender => FIELD_JSON_NAMES[1],
                Fields::NoSender => FIELD_JSON_NAMES[2],
                Fields::Keyword => FIELD_JSON_NAMES[3],
                Fields::NoKeyword => FIELD_JSON_NAMES[4],
            }
        }
    }

    struct FieldVisitor;

    struct StructVisitor;

    impl<'de> Visitor<'de> for FieldVisitor {
        type Value = Fields;
        fn expecting(&self, formatter: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
            write!(formatter, "expecting field")
        }
        fn visit_str<E>(self, v: &str) -> Result<Self::Value, E>
        where
            E: DeError,
        {
            Ok(Fields::from_str(v))
        }
    }

    impl<'de> Visitor<'de> for StructVisitor {
        type Value = super::TgFilter;
        fn expecting(&self, formatter: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
            write!(formatter, "expecting struct")
        }

        fn visit_map<A>(self, mut map: A) -> Result<Self::Value, A::Error>
        where
            A: MapAccess<'de>,
        {
            let mut f0: (Option<String>, &str) = (None, Fields::Title.to_sstr());
            let mut f1: (Option<Vec<String>>, &str) = (None, Fields::Sender.to_sstr());
            let mut f2: (Option<Vec<String>>, &str) = (None, Fields::NoSender.to_sstr());
            let mut f3: (Option<Vec<String>>, &str) = (None, Fields::Keyword.to_sstr());
            let mut f4: (Option<Vec<String>>, &str) = (None, Fields::NoKeyword.to_sstr());

            // none or wrap a duplication error
            let mut is_dup: Option<A::Error> = None;
            while let Some(key) = map.next_key()? {
                match key {
                    Fields::Title => match f0.0 {
                        None => f0.0 = Some(map.next_value()?),
                        _ => is_dup = Some(DeError::duplicate_field(f0.1)),
                    },
                    Fields::Sender => match f1.0 {
                        None => f1.0 = Some(map.next_value()?),
                        _ => is_dup = Some(DeError::duplicate_field(f1.1)),
                    },
                    Fields::NoSender => match f2.0 {
                        None => f2.0 = Some(map.next_value()?),
                        _ => is_dup = Some(DeError::duplicate_field(f2.1)),
                    },
                    Fields::Keyword => match f3.0 {
                        None => f3.0 = Some(map.next_value()?),
                        _ => is_dup = Some(DeError::duplicate_field(f3.1)),
                    },
                    Fields::NoKeyword => match f4.0 {
                        None => f4.0 = Some(map.next_value()?),
                        _ => is_dup = Some(DeError::duplicate_field(f4.1)),
                    },
                }
            }

            if let Some(e) = is_dup {
                return Err(e);
            }

            let mut ret = TgFilter::default();

            // title
            if let Some(v) = f0.0 {
                let may_re = Regex::new(&v);
                if let Err(e) = may_re {
                    return Err(DeError::invalid_value(
                        Unexpected::Other(&v),
                        &"valid regular expression",
                    ));
                }
                ret.title = Some(may_re.unwrap());
            }

            // keyword
            if let Some(s_list) = f3.0 {
                let mut kw_list = vec![];
                for s in &s_list {
                    let may_re = Regex::new(s);
                    if let Err(e) = may_re {
                        return Err(DeError::invalid_value(
                            Unexpected::Other(s),
                            &"valid regular expression",
                        ));
                    }
                    kw_list.push(may_re.unwrap())
                }
                ret.keyword = Some(kw_list);
            }

            // no_keyword
            if let Some(s_list) = f4.0 {
                let mut kw_list = vec![];
                for s in &s_list {
                    let may_re = Regex::new(s);
                    if let Err(e) = may_re {
                        return Err(DeError::invalid_value(
                            Unexpected::Other(s),
                            &"valid regular expression",
                        ));
                    }
                    kw_list.push(may_re.unwrap())
                }
                ret.no_keyword = Some(kw_list);
            }

            Ok(ret)
        }
    }

    impl<'de> Deserialize<'de> for Fields {
        fn deserialize<D>(deserializer: D) -> Result<Self, D::Error>
        where
            D: Deserializer<'de>,
        {
            deserializer.deserialize_identifier(FieldVisitor)
        }
    }

    impl<'de> Deserialize<'de> for TgFilter {
        fn deserialize<D>(deserializer: D) -> Result<Self, D::Error>
        where
            D: Deserializer<'de>,
        {
            deserializer.deserialize_struct("TgFilter", FIELD_JSON_NAMES, StructVisitor)
        }
    }
}

#[cfg(test)]
mod utst {
    use super::*;

    #[test]
    fn _1() {
        // parse ok
        let input = r#"

[[filter]]
title = "title1"
keyword = ["kw1", "kw2"]

"#;

        let may_filters = toml::from_str::<TgFilters>(input);

        assert!(may_filters.is_ok());
    }

    #[test]
    fn _2() {
        // invalid re
        let input = r#"

[[filter]]
title = "ti(tle1"
keyword = ["kw1", "kw2"]

"#;

        let may_filters = toml::from_str::<TgFilters>(input);

        assert!(may_filters.is_err());

        match may_filters {
            Err(e) => {
                assert_eq!(
                    e.message(),
                    "invalid value: ti(tle1, expected valid regular expression"
                );
            }
            _ => assert!(false),
        }
    }

    #[test]
    fn _3() {
        // invalid re
        let input = r#"

[[filter]]
title = "title1"
keyword = ["kw1", "k)w2"]

"#;

        let may_filters = toml::from_str::<TgFilters>(input);

        assert!(may_filters.is_err());

        match may_filters {
            Err(e) => {
                assert_eq!(
                    e.message(),
                    "invalid value: k)w2, expected valid regular expression"
                );
            }
            _ => assert!(false),
        }
    }

    #[test]
    fn _4() {
        // one filter matching msg
        let input = r#"

[[filter]]
title = "title1"
keyword = ["kw1", "kw2"]

"#;

        let filters = toml::from_str::<TgFilters>(input).unwrap();

        assert_eq!(filters.filter.len(), 1);

        let msg = CollectedMsg {
            title: "title123",
            sender: "xxx",
            ctn: "kw123",
            tstamp: "xxx",
        };

        assert_eq!(filters.is_match(&msg), (true, 0));
    }

    #[test]
    fn _5() {
        // one filter matching msg
        let input = r#"

    [[filter]]
    title = "title1"
    no_keyword = ["kw1", "kw2"]

    "#;

        let filters = toml::from_str::<TgFilters>(input).unwrap();

        assert_eq!(filters.filter.len(), 1);

        let msg = CollectedMsg {
            title: "title123",
            sender: "xxx",
            ctn: "kw123",
            tstamp: "xxx",
        };

        assert_eq!(filters.is_match(&msg), (false, 0));
    }

    #[test]
    fn _6() {
        // one filter matching msg
        let input = r#"

    [[filter]]
    title = "title1"
    no_keyword = ["kw1", "kw2"]

    "#;

        let filters = toml::from_str::<TgFilters>(input).unwrap();

        assert_eq!(filters.filter.len(), 1);

        let msg = CollectedMsg {
            title: "title123",
            sender: "xxx",
            ctn: "xxx",
            tstamp: "xxx",
        };

        assert_eq!(filters.is_match(&msg), (true, 0));
    }

    #[test]
    fn _7() {
        // one filter matching msg
        let input = r#"

    [[filter]]
    keyword = ["kw1", "kw2"]
    no_keyword = ["kw5", "kw6"]

    "#;

        let filters = toml::from_str::<TgFilters>(input).unwrap();

        assert_eq!(filters.filter.len(), 1);

        let msg = CollectedMsg {
            title: "xxx",
            sender: "xxx",
            ctn: "kw2, kw6",
            tstamp: "xxx",
        };

        assert_eq!(filters.is_match(&msg), (false, 0));
    }

    #[test]
    fn _8() {
        // more filters
        let input = r#"

    [[filter]]
    no_keyword = ["kw1", "kw2"]

    [[filter]]
    keyword = ["kw5", "kw6"]

    "#;

        let filters = toml::from_str::<TgFilters>(input).unwrap();

        assert_eq!(filters.filter.len(), 2);

        let msg = CollectedMsg {
            title: "xxx",
            sender: "xxx",
            ctn: "kw2, kw6",
            tstamp: "xxx",
        };

        assert_eq!(filters.is_match(&msg), (true, 1));

        let msg = CollectedMsg {
            title: "xxx",
            sender: "xxx",
            ctn: "kw2, kw",
            tstamp: "xxx",
        };

        assert_eq!(filters.is_match(&msg), (false, 1));

        let msg = CollectedMsg {
            title: "xxx",
            sender: "xxx",
            ctn: "xxx",
            tstamp: "xxx",
        };

        assert_eq!(filters.is_match(&msg), (false, 1));
    }

    #[test]
    fn _9() {
        // more filters
        let input = r#"

    [[filter]]
    title = "group-a"
    keyword = ["kw1", "kw2"]

    [[filter]]
    title = "group-b"
    no_keyword = ["kw3", "kw4"]

    [[filter]]
    title = "group-c"
    keyword = ["kw5", "kw6"]

    "#;

        let filters = toml::from_str::<TgFilters>(input).unwrap();

        assert_eq!(filters.filter.len(), 3);

        let msg = CollectedMsg {
            title: "group-a",
            sender: "xxx",
            ctn: "kw1",
            tstamp: "xxx",
        };
        assert_eq!(filters.is_match(&msg), (true, 0));

        let msg = CollectedMsg {
            title: "group-b",
            sender: "xxx",
            ctn: "xxx",
            tstamp: "xxx",
        };
        assert_eq!(filters.is_match(&msg), (true, 1));

        let msg = CollectedMsg {
            title: "group-c",
            sender: "xxx",
            ctn: "xxx",
            tstamp: "xxx",
        };
        assert_eq!(filters.is_match(&msg), (false, 2));

        let msg = CollectedMsg {
            title: "group-c",
            sender: "xxx",
            ctn: "kw6",
            tstamp: "xxx",
        };
        assert_eq!(filters.is_match(&msg), (true, 2));
    }

    #[test]
    fn _10() {
        // more filters
        let input = r#"

    [[filter]]
    title = "group-a"
    keyword = ["kw1", "kw2"]

    [[filter]]
    title = "group-b"
    keyword = ["kw3", "kw4"]

    [[filter]]
    title = "group-c"
    keyword = ["kw5", "kw6"]

    "#;

        let filters = toml::from_str::<TgFilters>(input).unwrap();

        assert_eq!(filters.filter.len(), 3);

        let msg = CollectedMsg {
            title: "group-a",
            sender: "xxx",
            ctn: "kw1",
            tstamp: "xxx",
        };
        assert_eq!(filters.is_match(&msg), (true, 0));

        let msg = CollectedMsg {
            title: "group-b",
            sender: "xxx",
            ctn: "xxx",
            tstamp: "xxx",
        };
        assert_eq!(filters.is_match(&msg), (false, 2)); // will advance

        let msg = CollectedMsg {
            title: "group-b",
            sender: "xxx",
            ctn: "kw4",
            tstamp: "xxx",
        };
        assert_eq!(filters.is_match(&msg), (true, 1));

        let msg = CollectedMsg {
            title: "group-c",
            sender: "xxx",
            ctn: "xxx",
            tstamp: "xxx",
        };
        assert_eq!(filters.is_match(&msg), (false, 2));

        let msg = CollectedMsg {
            title: "group-c",
            sender: "xxx",
            ctn: "kw6",
            tstamp: "xxx",
        };
        assert_eq!(filters.is_match(&msg), (true, 2));
    }

    #[test]
    fn _11() {
        // more filters
        let input = r#"

    [[filter]]
    title = "group-a"
    keyword = ["kw1", "kw2"]

    [[filter]]
    title = "group-b"
    keyword = ["kw3", "kw4"]

    [[filter]]
    title = "group"
    no_keyword = ["kw5", "kw6"]

    "#;

        let filters = toml::from_str::<TgFilters>(input).unwrap();

        assert_eq!(filters.filter.len(), 3);

        let msg = CollectedMsg {
            title: "group-a",
            sender: "xxx",
            ctn: "kw1",
            tstamp: "xxx",
        };
        assert_eq!(filters.is_match(&msg), (true, 0));

        let msg = CollectedMsg {
            title: "group-b",
            sender: "xxx",
            ctn: "xxx",
            tstamp: "xxx",
        };
        assert_eq!(filters.is_match(&msg), (true, 2)); // will advance

        let msg = CollectedMsg {
            title: "group-b",
            sender: "xxx",
            ctn: "kw4",
            tstamp: "xxx",
        };
        assert_eq!(filters.is_match(&msg), (true, 1));

        let msg = CollectedMsg {
            title: "group-c",
            sender: "xxx",
            ctn: "xxx",
            tstamp: "xxx",
        };
        assert_eq!(filters.is_match(&msg), (true, 2));

        let msg = CollectedMsg {
            title: "group-c",
            sender: "xxx",
            ctn: "kw6",
            tstamp: "xxx",
        };
        assert_eq!(filters.is_match(&msg), (false, 2));
    }
}
