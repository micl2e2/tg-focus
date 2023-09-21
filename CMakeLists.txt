# cmake_minimum_required(VERSION 3.20)
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

# third-party
add_subdirectory(${PROJECT_SOURCE_DIR}/deps/fmt)
link_libraries(fmt)

# a0d026239439c76847c4f75f2fa32f3e56b9b789
find_package(Td 1.8.17 REQUIRED)


# posix_regex
add_library(posix_regex ${PROJECT_SOURCE_DIR}/src/posix_regex.cc)
target_include_directories(posix_regex PRIVATE ${PROJECT_SOURCE_DIR}/include)

# focus_filter
add_library(focus_filter ${PROJECT_SOURCE_DIR}/src/focus_filter.cc)
target_include_directories(focus_filter
  PRIVATE ${PROJECT_SOURCE_DIR}/include
  PUBLIC ${PROJECT_SOURCE_DIR}/deps/toml11)
target_link_libraries(focus_filter PRIVATE posix_regex)

# tf_data
add_library(tf_data ${PROJECT_SOURCE_DIR}/src/tf_data.cc)
target_include_directories(tf_data PRIVATE ${PROJECT_SOURCE_DIR}/include)

# td_client
add_library(td_client ${PROJECT_SOURCE_DIR}/src/td_client.cc)
target_include_directories(td_client PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(td_client PRIVATE focus_filter PRIVATE tf_data)

# tf-conf
add_executable(tf-conf ${PROJECT_SOURCE_DIR}/src/tf-conf.cc)
target_include_directories(
  tf-conf
  PRIVATE ${PROJECT_SOURCE_DIR}/include
  PRIVATE ${PROJECT_SOURCE_DIR}/deps/toml11)
target_link_libraries(tf-conf PRIVATE -static-libgcc -static-libstdc++)
target_link_libraries(tf-conf PRIVATE Td::TdStatic)
target_link_libraries(tf-conf PRIVATE focus_filter PRIVATE tf_data)

# ...
# add_executable(tf-tstleak ${PROJECT_SOURCE_DIR}/src/tf-tstleak.cc)
# target_include_directories(
#   tf-tstleak
#   PRIVATE ${PROJECT_SOURCE_DIR}/include)

# focusd_state
add_library(focusd_state ${PROJECT_SOURCE_DIR}/src/focusd_state.cc)
target_include_directories(focusd_state PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(focusd_state
  PRIVATE tf_data
  PRIVATE td_client
)

# tf-focusd
add_executable(tf-focusd ${PROJECT_SOURCE_DIR}/src/tf-focusd.cc)
target_include_directories(
  tf-focusd
  PRIVATE ${PROJECT_SOURCE_DIR}/include
  PRIVATE ${PROJECT_SOURCE_DIR}/deps/toml11)
target_link_libraries(tf-focusd PRIVATE -static-libgcc -static-libstdc++)
target_link_libraries(tf-focusd
  PRIVATE focus_filter
  PRIVATE tf_data
  PRIVATE td_client
  PRIVATE focusd_state)
target_link_libraries(tf-focusd PRIVATE Td::TdStatic)

# TEST
# if (DEFINED ENV{RUN_TEST})
  enable_testing()
  add_subdirectory(test)
# endif()


