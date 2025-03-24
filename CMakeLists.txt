cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(tg-focus)

# santinize, linked by all targets
if (DEFINED ENV{WITH_SANITIZERS})
  add_compile_options(-fsanitize=address -fsanitize=undefined -fno-sanitize=vptr -fsanitize=leak)
  link_libraries(asan ubsan)
endif()

# ----------------------------- tdlib -----------------------------

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  set(ZLIB_USE_STATIC_LIBS ON)
  # might need to build from source in most distros
  set(OPENSSL_USE_STATIC_LIBS ON)
else()
  option(BUILD_TESTING OFF)
  option(TD_BUILD_BENCHMARK OFF)
endif()

add_subdirectory(3rd/tdlib)

# ------------------------------ OURS ------------------------------

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  # add_compile_options(-Wall -Wextra)
  add_compile_options(-flto)
  add_link_options(-Wl,-s)
else()
  add_compile_options(-g)
endif()

# ------------------------------ conf ------------------------------

# add_executable(
#   tgf-conf
#   ${PROJECT_SOURCE_DIR}/tgf-conf/TgfConf.cpp
#   ${PROJECT_SOURCE_DIR}/tgf-conf/State.cpp
#   ${PROJECT_SOURCE_DIR}/tgf-conf/Auth.cpp
#   ${PROJECT_SOURCE_DIR}/shared/TgfData.cpp
#   ${PROJECT_SOURCE_DIR}/shared/PosixRegex.cpp
#   ${PROJECT_SOURCE_DIR}/shared/LvLog.cpp
#   ${PROJECT_SOURCE_DIR}/shared/TgfLocale.cpp
#   ${PROJECT_SOURCE_DIR}/shared/TgfFilter.cpp
# )
# target_include_directories(
#   tgf-conf
#   PRIVATE ${PROJECT_SOURCE_DIR}/shared
#   PRIVATE ${PROJECT_SOURCE_DIR}/3rd/toml11
# )
# target_link_libraries(
#   tgf-conf
#   PRIVATE Td::TdStatic
# )

# if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
#   target_link_libraries(
#     tgf-conf
#     PRIVATE -static -no-pie
#   )
# endif()

# # ----------------------------- focusd -----------------------------

# add_executable(
#   tgf-focusd
#   ${PROJECT_SOURCE_DIR}/tgf-focusd/TgfFocusd.cpp
#   ${PROJECT_SOURCE_DIR}/tgf-focusd/State.cpp
#   ${PROJECT_SOURCE_DIR}/tgf-focusd/Worker.cpp
#   ${PROJECT_SOURCE_DIR}/tgf-focusd/Collector.cpp
#   ${PROJECT_SOURCE_DIR}/shared/TgfData.cpp
#   ${PROJECT_SOURCE_DIR}/shared/PosixRegex.cpp
#   ${PROJECT_SOURCE_DIR}/shared/TgfFilter.cpp
#   ${PROJECT_SOURCE_DIR}/shared/TgfMsg.cpp
#   ${PROJECT_SOURCE_DIR}/shared/LvLog.cpp
#   ${PROJECT_SOURCE_DIR}/shared/TgfLocale.cpp  
# )

# target_include_directories(
#   tgf-focusd
#   PRIVATE ${PROJECT_SOURCE_DIR}/shared
#   PRIVATE ${PROJECT_SOURCE_DIR}/3rd/toml11
# )

# target_link_libraries(
#   tgf-focusd
#   PRIVATE Td::TdStatic
# )

# if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
#   target_link_libraries(
#     tgf-conf
#     PRIVATE -static -no-pie
#   )
# endif()


# ------------------------------ tgfocus exe ------------------------------

add_executable(
  tgfocus
  ${PROJECT_SOURCE_DIR}/src/log.cc
  ${PROJECT_SOURCE_DIR}/src/stat.cc
  ${PROJECT_SOURCE_DIR}/src/worker/conn_listener.cc
  ${PROJECT_SOURCE_DIR}/src/worker/ipc_conn_handler.cc
  ${PROJECT_SOURCE_DIR}/src/worker/ipc_msg_dequer.cc
  ${PROJECT_SOURCE_DIR}/src/worker/ipc_server_initer.cc
  ${PROJECT_SOURCE_DIR}/src/worker/coll_initer.cc
  ${PROJECT_SOURCE_DIR}/src/worker/coll_producer.cc
  ${PROJECT_SOURCE_DIR}/src/worker/coll_consumer.cc
  ${PROJECT_SOURCE_DIR}/src/worker/coll_switcher.cc
  ${PROJECT_SOURCE_DIR}/src/ipc_msg_queue.cc
  ${PROJECT_SOURCE_DIR}/src/ipc_msg.cc
  ${PROJECT_SOURCE_DIR}/src/tgfocus.cc
  ${PROJECT_SOURCE_DIR}/src/ipc_server.cc
  ${PROJECT_SOURCE_DIR}/src/ipc_client.cc
  ${PROJECT_SOURCE_DIR}/src/tg_msg.cc
  ${PROJECT_SOURCE_DIR}/src/td_auth.cc
  ${PROJECT_SOURCE_DIR}/src/td_coll.cc
  ${PROJECT_SOURCE_DIR}/src/user_data.cc
  ${PROJECT_SOURCE_DIR}/src/posix_regex.cc
  ${PROJECT_SOURCE_DIR}/src/locale_conf.cc
  ${PROJECT_SOURCE_DIR}/src/filter.cc
  ${PROJECT_SOURCE_DIR}/src/chatcmd.cc
)
target_include_directories(
  tgfocus
  PRIVATE ${PROJECT_SOURCE_DIR}/src
  PRIVATE ${PROJECT_SOURCE_DIR}/3rd/toml11
)
target_link_libraries(
  tgfocus
  PRIVATE Td::TdStatic
)
if((CMAKE_CXX_COMPILER_ID STREQUAL "GNU") AND (CMAKE_BUILD_TYPE STREQUAL "Release"))
  target_link_libraries(
    tgfocus
    PRIVATE -static -no-pie
  )
endif()



# ----------------------------- tests -----------------------------

enable_testing()
add_subdirectory(test)
