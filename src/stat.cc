#define __TU__ "stat"

#include "stat.hh"

namespace tgfstat {

atomic<bool> im_child{false};
tgf::UserData userdata{nullopt, false};

namespace p {

atomic<bool> is_login{false};
atomic<bool> is_tdlib_closed{false};

} // namespace p

namespace c { // child specific

tgf::IpcMsgQueue q_ipcmsg{};
atomic<bool> q_ipcmsg_has_upd{false};
atomic<bool> coll_initer_up{false};

namespace tryshutwk {

atomic<bool> conn_listener{false};
atomic<bool> conn_listener_succ{false};
atomic<bool> ipcsrv_initer{false};
atomic<bool> ipcsrv_initer_succ{false};
atomic<bool> ipcconn_handler{false};
atomic<bool> ipcconn_handler_succ{false};
atomic<bool> ipcmsg_dequer{false};
atomic<bool> ipcmsg_dequer_succ{false};
atomic<bool> coll_initer{false};
atomic<bool> coll_initer_succ{false};
atomic<bool> coll_producer{false};
atomic<bool> coll_producer_succ{false};
atomic<bool> coll_consumer{false};
atomic<bool> coll_consumer_succ{false};
atomic<bool> coll_switcher{false};
atomic<bool> coll_switcher_succ{false};

} // namespace tryshutwk

namespace d { // daemon specific

atomic<uint32_t> it_cnt_switcher;
atomic<uint32_t> it_cnt_producer;
atomic<uint32_t> it_cnt_consumer;
atomic<u32> it_msg_total{0};
atomic<u32> it_msg_coll{0};

vector<tgf::TgMsg> mq;
mutex mq_lock;
atomic<bool> do_csm_mq;
atomic<bool> pause_do_csm_mq;
atomic<bool> is_tdlib_auth;
atomic<bool> need_phone;
TdCollector collector;
// tgf::UserData tgf_data{nullopt, false};
atomic<int> producer_hang_period{0};

} // namespace d

} // namespace c

} // namespace tgfstat
