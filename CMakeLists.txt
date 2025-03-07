cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# set(CMAKE_CXX_COMPILER /usr/bin/clang++) # must precede project()

project(tg-focus)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  add_compile_options(-Wall -Wextra)
else()
endif()

# santinize, linked by all targets
if (DEFINED ENV{WITH_SANITIZERS})
  add_compile_options(-fsanitize=address -fsanitize=undefined -fno-sanitize=vptr -fsanitize=leak)
  link_libraries(asan ubsan)
endif()

# ----------------------------- tdlib -----------------------------

# TDLib versions
# note: sync with tg-focus-cicd
find_package(Td 1.8.44 REQUIRED)

# ------------------------------ conf ------------------------------

add_executable(
  tgf-conf
  ${PROJECT_SOURCE_DIR}/tgf-conf/TgfConf.cpp
  ${PROJECT_SOURCE_DIR}/tgf-conf/State.cpp
  ${PROJECT_SOURCE_DIR}/tgf-conf/Auth.cpp
  ${PROJECT_SOURCE_DIR}/shared/TgfData.cpp
  ${PROJECT_SOURCE_DIR}/shared/PosixRegex.cpp
  ${PROJECT_SOURCE_DIR}/shared/LvLog.cpp
  ${PROJECT_SOURCE_DIR}/shared/TgfLocale.cpp
  ${PROJECT_SOURCE_DIR}/shared/TgfFilter.cpp
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

# ----------------------------- focusd -----------------------------

add_executable(
  tgf-focusd
  ${PROJECT_SOURCE_DIR}/tgf-focusd/TgfFocusd.cpp
  ${PROJECT_SOURCE_DIR}/tgf-focusd/State.cpp
  ${PROJECT_SOURCE_DIR}/tgf-focusd/Worker.cpp
  ${PROJECT_SOURCE_DIR}/tgf-focusd/Collector.cpp
  ${PROJECT_SOURCE_DIR}/shared/TgfData.cpp
  ${PROJECT_SOURCE_DIR}/shared/PosixRegex.cpp
  ${PROJECT_SOURCE_DIR}/shared/TgfFilter.cpp
  ${PROJECT_SOURCE_DIR}/shared/TgfMsg.cpp
  ${PROJECT_SOURCE_DIR}/shared/LvLog.cpp
  ${PROJECT_SOURCE_DIR}/shared/TgfLocale.cpp  
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

# ----------------------------- tests -----------------------------

enable_testing()
add_subdirectory(test)
