#[derive(Debug)]
pub struct CollectedMsg<'a> {
    pub title: &'a str,
    pub sender: &'a str,
    pub ctn: &'a str,
    pub tstamp: &'a str,
}

impl CollectedMsg<'_> {
    pub fn to_string(&self) -> String {
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
    // fn is_important(&self) -> bool {
    //     let tg_filters = xxx;

    //     for filter in tg_filters {
    //         if let Some(title_re_list) = filter.title {}
    //     }
    // }

    pub fn is_ctn_interesting(&self) -> bool {
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
