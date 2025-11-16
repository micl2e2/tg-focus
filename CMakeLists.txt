cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(tg-focus)

option(IS_LINUX OFF)
option(IS_CENT9 OFF)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
  set(IS_LINUX ON)
  file(READ "/etc/os-release" OS_RELEASE_CONTENT)
  if(OS_RELEASE_CONTENT MATCHES "CentOS Stream 9")
    set(IS_CENT9 ON)
  endif()
endif()

if(IS_LINUX)
  set(ZLIB_USE_STATIC_LIBS ON)
  if(IS_CENT9)
    set(OPENSSL_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/ci/openssl/build)
    set(OPENSSL_INCLUDE_DIR ${OPENSSL_INSTALL_PREFIX}/include/openssl)
    set(OPENSSL_CRYPTO_LIBRARY ${OPENSSL_INSTALL_PREFIX}/lib64/libcrypto.a)
    set(OPENSSL_SSL_LIBRARY ${OPENSSL_INSTALL_PREFIX}/lib64/libssl.a)
  endif()
endif()

if (DEFINED ENV{WITH_SANITIZERS})
  add_compile_options(-fsanitize=address -fsanitize=undefined -fno-sanitize=vptr -fsanitize=leak)
  link_libraries(asan ubsan)
endif()

add_subdirectory(ci/tdlib)

# # # # # # # # # # TGF # # # # # # # # # #

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  # add_compile_options(-Wall -Wextra)
  add_link_options(-Wl,-s)
endif()

add_executable(tgfocus
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
  ${PROJECT_SOURCE_DIR}/src/utfutils.cc)

target_include_directories(tgfocus
  PRIVATE ${PROJECT_SOURCE_DIR}/src
  PRIVATE ${PROJECT_SOURCE_DIR}/ci/toml11)

target_link_libraries(tgfocus
  PRIVATE Td::TdStatic)

target_link_libraries(tgfocus
  PRIVATE -static-libgcc -static-libstdc++)

enable_testing()
add_subdirectory(test)
