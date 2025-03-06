include(CTest)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  add_compile_options(-Wno-unused-variable)
endif()

# add a test
set(CurrTestName posix_regex)
add_executable(${CurrTestName} ${CurrTestName}.cc
   ${PROJECT_SOURCE_DIR}/shared/posix_regex.cc
)
target_include_directories(${CurrTestName} PRIVATE ${PROJECT_SOURCE_DIR}/shared)
add_test(NAME ${CurrTestName} COMMAND ${CurrTestName})

# --------------------------- Add A Test ---------------------------

set(CurrTestName TgfFilter)
add_executable(${CurrTestName} t${CurrTestName}.cpp
   ${PROJECT_SOURCE_DIR}/shared/${CurrTestName}.cpp
   ${PROJECT_SOURCE_DIR}/shared/posix_regex.cc
   ${PROJECT_SOURCE_DIR}/shared/TgfMsg.cpp
   ${PROJECT_SOURCE_DIR}/shared/TgfLocale.cpp
)
target_include_directories(${CurrTestName} PRIVATE
  ${PROJECT_SOURCE_DIR}/shared
  ${PROJECT_SOURCE_DIR}/3rd/toml11)
add_test(NAME ${CurrTestName} COMMAND ${CurrTestName})

# add a test
set(CurrTestName tgf_data)
add_executable(${CurrTestName} ${CurrTestName}.cc
   ${PROJECT_SOURCE_DIR}/shared/tgf_data.cc
   ${PROJECT_SOURCE_DIR}/shared/TgfLocale.cpp
)
target_include_directories(${CurrTestName} PRIVATE
  ${PROJECT_SOURCE_DIR}/shared)
add_test(NAME ${CurrTestName} COMMAND ${CurrTestName})

# add a test
set(CurrTestName TgfMsg)
add_executable(${CurrTestName} t${CurrTestName}.cpp
  ${PROJECT_SOURCE_DIR}/shared/TgfMsg.cpp
  ${PROJECT_SOURCE_DIR}/shared/TgfLocale.cpp
)
target_include_directories(${CurrTestName} PRIVATE ${PROJECT_SOURCE_DIR}/shared)
add_test(NAME ${CurrTestName} COMMAND ${CurrTestName})


# add a test
set(CurrTestName TgfLocale)
add_executable(${CurrTestName} t${CurrTestName}.cpp
  ${PROJECT_SOURCE_DIR}/shared/TgfLocale.cpp
)
target_include_directories(${CurrTestName} PRIVATE ${PROJECT_SOURCE_DIR}/shared)
add_test(NAME ${CurrTestName} COMMAND ${CurrTestName})
