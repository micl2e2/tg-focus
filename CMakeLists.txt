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
  set(CMAKE_CXX_FLAGS_RELEASE "-O3" CACHE STRING "" FORCE)
else()

endif()

# santinize, linked by all targets
if (DEFINED ENV{WITH_SANITIZERS})
  add_compile_options(-fsanitize=address -fsanitize=undefined -fno-sanitize=vptr -fsanitize=leak)
  link_libraries(asan ubsan)
endif()

# deps
add_subdirectory(${PROJECT_SOURCE_DIR}/3rd/fmt)
link_libraries(fmt)

# TDLib versions
# note: sync with tg-focus-cicd
execute_process(COMMAND bash ${PROJECT_SOURCE_DIR}/dev/before-compile.bash)
find_package(Td 1.8.23 REQUIRED)

# tf-conf
add_executable(
  tf-conf
  ${PROJECT_SOURCE_DIR}/tf-conf/tf_conf.cc
  ${PROJECT_SOURCE_DIR}/tf-conf/state.cc
  ${PROJECT_SOURCE_DIR}/tf-conf/auth.cc
  ${PROJECT_SOURCE_DIR}/shared/tf_data.cc
  ${PROJECT_SOURCE_DIR}/shared/posix_regex.cc
  ${PROJECT_SOURCE_DIR}/shared/focus_filter.cc
  ${PROJECT_SOURCE_DIR}/shared/lv_log.cc
)
target_include_directories(
  tf-conf
  PRIVATE ${PROJECT_SOURCE_DIR}/shared
  PRIVATE ${PROJECT_SOURCE_DIR}/3rd/toml11
)
target_link_libraries(
  tf-conf
  PRIVATE Td::TdStatic
)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_link_libraries(
    tf-conf
    PRIVATE -static-libgcc -static-libstdc++
  )
endif()


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
  ${PROJECT_SOURCE_DIR}/shared/lv_log.cc
)
target_include_directories(
  tf-focusd
  PRIVATE ${PROJECT_SOURCE_DIR}/shared
  PRIVATE ${PROJECT_SOURCE_DIR}/3rd/toml11
)
target_link_libraries(
  tf-focusd
  PRIVATE Td::TdStatic
)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_link_libraries(
    tf-focusd
    PRIVATE -static-libgcc -static-libstdc++
  )
endif()

# tests
enable_testing()
add_subdirectory(test)
