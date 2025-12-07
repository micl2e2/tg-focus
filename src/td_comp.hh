#ifndef hh_td_comp
#define hh_td_comp

#include <td/telegram/Client.h>
#include <td/telegram/td_api.h>
#include <td/telegram/td_api.hpp>

namespace tapi = td::td_api;

using TdObjPtr = tapi::object_ptr<tapi::Object>;

template <class Type, class... Args>
td::tl_object_ptr<Type>
td_mkobj (Args &&...args)
{
  return td::td_api::make_object<Type> (std::forward<Args> (args)...);
}

template <class ToT, class FromT>
td::tl_object_ptr<ToT>
tl_movas (td::tl_object_ptr<FromT> &from)
{
  return td::move_tl_object_as<ToT> (from);
}

#endif
