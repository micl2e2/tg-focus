cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# set(CMAKE_CXX_COMPILER /usr/bin/clang++) # must precede project()

project(tg-focus)


###
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

###
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

###
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  add_compile_options(-Wall -Wextra)
else()

endif()

# santinize, linked by all targets
if (DEFINED ENV{WITH_SANITIZERS})
  add_compile_options(-fsanitize=address -fsanitize=undefined -fno-sanitize=vptr -fsanitize=leak)
  link_libraries(asan ubsan)
endif()

# TDLib versions
# note: sync with tg-focus-cicd
execute_process(COMMAND bash ${PROJECT_SOURCE_DIR}/dev/sync-versions.bash)
find_package(Td 1.8.36 REQUIRED)

# tgf-conf
add_executable(
  tgf-conf
  ${PROJECT_SOURCE_DIR}/tgf-conf/tgf_conf.cc
  ${PROJECT_SOURCE_DIR}/tgf-conf/state.cc
  ${PROJECT_SOURCE_DIR}/tgf-conf/auth.cc
  ${PROJECT_SOURCE_DIR}/shared/tgf_data.cc
  ${PROJECT_SOURCE_DIR}/shared/posix_regex.cc
  ${PROJECT_SOURCE_DIR}/shared/focus_filter.cc
  ${PROJECT_SOURCE_DIR}/shared/lv_log.cc
  ${PROJECT_SOURCE_DIR}/shared/tgf_locale.cc
)
target_include_directories(
  tgf-conf
  PRIVATE ${PROJECT_SOURCE_DIR}/shared
  PRIVATE ${PROJECT_SOURCE_DIR}/3rd/toml11
)
target_link_libraries(
  tgf-conf
  PRIVATE Td::TdStatic
)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_link_libraries(
    tgf-conf
    PRIVATE -static-libgcc -static-libstdc++
  )
endif()


# tgf-focusd
add_executable(
  tgf-focusd
  ${PROJECT_SOURCE_DIR}/tgf-focusd/tgf_focusd.cc
  ${PROJECT_SOURCE_DIR}/tgf-focusd/state.cc
  ${PROJECT_SOURCE_DIR}/tgf-focusd/worker.cc
  ${PROJECT_SOURCE_DIR}/tgf-focusd/collector.cc
  ${PROJECT_SOURCE_DIR}/shared/tgf_data.cc
  ${PROJECT_SOURCE_DIR}/shared/posix_regex.cc
  ${PROJECT_SOURCE_DIR}/shared/focus_filter.cc
  ${PROJECT_SOURCE_DIR}/shared/tgf_msg.cc
  ${PROJECT_SOURCE_DIR}/shared/lv_log.cc
  ${PROJECT_SOURCE_DIR}/shared/tgf_locale.cc  
)
target_include_directories(
  tgf-focusd
  PRIVATE ${PROJECT_SOURCE_DIR}/shared
  PRIVATE ${PROJECT_SOURCE_DIR}/3rd/toml11
)
target_link_libraries(
  tgf-focusd
  PRIVATE Td::TdStatic
)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_link_libraries(
    tgf-focusd
    PRIVATE -static-libgcc -static-libstdc++
  )
endif()

# tests
enable_testing()
add_subdirectory(test)
