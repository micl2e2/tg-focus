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
    fn is_match(&self, msg: &CollectedMsg) -> bool {
        for filter in &self.filter {
            if let Some(title) = &filter.title {
                // if msg.title.contains(title) {
                //     return true;
                // }
            }
        }

        return false;
    }
}

#[derive(Debug, Default)]
struct TgFilter {
    title: Option<Regex>,
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
        Keyword,
        NoKeyword,
    }

    const FIELD_JSON_NAMES: &[&str] = &["title", "keyword", "no_keyword"];

    impl Fields {
        fn from_str(s: &str) -> Self {
            if s == FIELD_JSON_NAMES[0] {
                return Fields::Title;
            }
            if s == FIELD_JSON_NAMES[1] {
                return Fields::Keyword;
            }
            if s == FIELD_JSON_NAMES[2] {
                return Fields::NoKeyword;
            }

            unreachable!()
        }
        fn to_sstr(&self) -> &'static str {
            match self {
                Fields::Title => FIELD_JSON_NAMES[0],
                Fields::Keyword => FIELD_JSON_NAMES[1],
                Fields::NoKeyword => FIELD_JSON_NAMES[2],
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
            let mut f1: (Option<Vec<String>>, &str) = (None, Fields::Keyword.to_sstr());
            let mut f2: (Option<Vec<String>>, &str) = (None, Fields::NoKeyword.to_sstr());

            // none or wrap a duplication error
            let mut is_dup: Option<A::Error> = None;
            while let Some(key) = map.next_key()? {
                match key {
                    Fields::Title => match f0.0 {
                        None => f0.0 = Some(map.next_value()?),
                        _ => is_dup = Some(DeError::duplicate_field(f0.1)),
                    },
                    Fields::Keyword => match f1.0 {
                        None => f1.0 = Some(map.next_value()?),
                        _ => is_dup = Some(DeError::duplicate_field(f1.1)),
                    },
                    Fields::NoKeyword => match f2.0 {
                        None => f2.0 = Some(map.next_value()?),
                        _ => is_dup = Some(DeError::duplicate_field(f2.1)),
                    },
                }
            }

            if let Some(e) = is_dup {
                return Err(e);
            }

            let mut ret = TgFilter::default();

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
        let input = r#"

[[filter]]
title = "ti(tle1"
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
