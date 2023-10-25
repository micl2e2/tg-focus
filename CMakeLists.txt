cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(tg-focus)

# set(CMAKE_CXX_COMPILER clang++)

# cpp std
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# warning
add_compile_options(-Wall -Wextra)

# santinize, linked by all targets
if (DEFINED ENV{WITH_SANITIZERS})
  add_compile_options(-fsanitize=address -fsanitize=undefined -fno-sanitize=vptr -fsanitize=leak)
  link_libraries(asan ubsan)
endif()

# deps
add_subdirectory(${PROJECT_SOURCE_DIR}/3rd/fmt)
link_libraries(fmt)

# 030e440757478ef5f5fdb01f60c3c6d885330c72
#   note: sync with build/fetch-tdlib
find_package(Td 1.8.14 REQUIRED)

# tf-conf
add_executable(
  tf-conf
  ${PROJECT_SOURCE_DIR}/tf-conf/tf_conf.cc
  ${PROJECT_SOURCE_DIR}/tf-conf/state.cc
  ${PROJECT_SOURCE_DIR}/tf-conf/auth.cc
  ${PROJECT_SOURCE_DIR}/shared/tf_data.cc
  ${PROJECT_SOURCE_DIR}/shared/posix_regex.cc
  ${PROJECT_SOURCE_DIR}/shared/focus_filter.cc
)
target_include_directories(
  tf-conf
  PRIVATE ${PROJECT_SOURCE_DIR}/shared
  PRIVATE ${PROJECT_SOURCE_DIR}/3rd/toml11
)
target_link_libraries(
  tf-conf
  PRIVATE -static-libgcc -static-libstdc++
  PRIVATE Td::TdStatic
)

# tf-focusd
add_executable(
  tf-focusd
  ${PROJECT_SOURCE_DIR}/tf-focusd/tf_focusd.cc
  ${PROJECT_SOURCE_DIR}/tf-focusd/state.cc
  ${PROJECT_SOURCE_DIR}/tf-focusd/worker.cc
  ${PROJECT_SOURCE_DIR}/tf-focusd/collector.cc
  ${PROJECT_SOURCE_DIR}/shared/tf_data.cc
  ${PROJECT_SOURCE_DIR}/shared/posix_regex.cc
  ${PROJECT_SOURCE_DIR}/shared/focus_filter.cc
  ${PROJECT_SOURCE_DIR}/shared/tf_msg.cc
)
target_include_directories(
  tf-focusd
  PRIVATE ${PROJECT_SOURCE_DIR}/shared
  PRIVATE ${PROJECT_SOURCE_DIR}/3rd/toml11
)
target_link_libraries(
  tf-focusd
  PRIVATE -static-libgcc -static-libstdc++
  PRIVATE Td::TdStatic
)

# tests
enable_testing()
add_subdirectory(test)
