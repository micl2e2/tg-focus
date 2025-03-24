#ifndef hh_stat
#define hh_stat

#include "std_comp.hh"
#include "user_data.hh"
#include "tg_msg.hh"
#include "td_coll.hh"
#include "ipc_msg_queue.hh"

namespace tgfstat {

extern atomic<bool> im_child;
extern tgf::UserData userdata;

namespace p {
extern atomic<bool> is_login;
extern atomic<bool> is_tdlib_closed;

} // namespace p

namespace c { // child specific

extern tgf::IpcMsgQueue q_ipcmsg;
extern atomic<bool> q_ipcmsg_has_upd;
extern atomic<bool> coll_initer_up;

namespace tryshutwk {

extern atomic<bool> conn_listener;
extern atomic<bool> conn_listener_succ;
extern atomic<bool> ipcsrv_initer;
extern atomic<bool> ipcsrv_initer_succ;
extern atomic<bool> ipcconn_handler;
extern atomic<bool> ipcconn_handler_succ;
extern atomic<bool> ipcmsg_dequer;
extern atomic<bool> ipcmsg_dequer_succ;
extern atomic<bool> coll_initer;
extern atomic<bool> coll_initer_succ;
extern atomic<bool> coll_producer;
extern atomic<bool> coll_producer_succ;
extern atomic<bool> coll_consumer;
extern atomic<bool> coll_consumer_succ;
extern atomic<bool> coll_switcher;
extern atomic<bool> coll_switcher_succ;

} // namespace tryshutwk

namespace d { // daemon specific

extern atomic<uint32_t> it_cnt_switcher;
extern atomic<uint32_t> it_cnt_producer;
extern atomic<uint32_t> it_cnt_consumer;
extern atomic<u32> it_msg_total;
extern atomic<u32> it_msg_coll;

extern vector<tgf::TgMsg> mq;
extern mutex mq_lock;
extern atomic<bool> do_csm_mq;
extern atomic<bool> pause_do_csm_mq;
extern atomic<bool> is_tdlib_auth;
extern atomic<bool> need_phone;
extern TdCollector collector;
// extern tgf::UserData tgf_data;
extern atomic<int> producer_hang_period;

} // namespace d

} // namespace c

} // namespace tgfstat

#endif
