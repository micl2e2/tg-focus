cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(tg-focus)

# santinize, linked by all targets
if (DEFINED ENV{WITH_SANITIZERS})
  add_compile_options(-fsanitize=address -fsanitize=undefined -fno-sanitize=vptr -fsanitize=leak)
  link_libraries(asan ubsan)
endif()

# ----------------------------- tdlib -----------------------------

set(ZLIB_USE_STATIC_LIBS ON)
# might need to build from source in most distros
set(OPENSSL_USE_STATIC_LIBS TRUE) 
add_subdirectory(3rd/tdlib)

# ------------------------------ OURS ------------------------------

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  # add_compile_options(-Wall -Wextra)
  add_compile_options(-flto)
  add_link_options(-Wl,-s)
endif()

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
    PRIVATE -static -no-pie
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

target_link_libraries(
  tgf-focusd
  PRIVATE -static -no-pie
)

# ----------------------------- tests -----------------------------

enable_testing()
add_subdirectory(test)
