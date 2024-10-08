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

# add a test
set(CurrTestName focus_filter_config)
add_executable(${CurrTestName} ${CurrTestName}.cc
   ${PROJECT_SOURCE_DIR}/shared/focus_filter.cc
   ${PROJECT_SOURCE_DIR}/shared/posix_regex.cc
   ${PROJECT_SOURCE_DIR}/shared/tgf_msg.cc
   ${PROJECT_SOURCE_DIR}/shared/tgf_locale.cc
)
target_include_directories(${CurrTestName} PRIVATE
  ${PROJECT_SOURCE_DIR}/shared
  ${PROJECT_SOURCE_DIR}/3rd/toml11)
add_test(NAME ${CurrTestName} COMMAND ${CurrTestName})

# add a test
set(CurrTestName focus_filter)
add_executable(${CurrTestName} ${CurrTestName}.cc
   ${PROJECT_SOURCE_DIR}/shared/focus_filter.cc
   ${PROJECT_SOURCE_DIR}/shared/posix_regex.cc
   ${PROJECT_SOURCE_DIR}/shared/tgf_msg.cc
   ${PROJECT_SOURCE_DIR}/shared/tgf_locale.cc
)
target_include_directories(${CurrTestName} PRIVATE
  ${PROJECT_SOURCE_DIR}/shared
  ${PROJECT_SOURCE_DIR}/3rd/toml11)
add_test(NAME ${CurrTestName} COMMAND ${CurrTestName})

# add a test
set(CurrTestName tgf_data)
add_executable(${CurrTestName} ${CurrTestName}.cc
   ${PROJECT_SOURCE_DIR}/shared/tgf_data.cc
   ${PROJECT_SOURCE_DIR}/shared/tgf_locale.cc
)
target_include_directories(${CurrTestName} PRIVATE
  ${PROJECT_SOURCE_DIR}/shared)
add_test(NAME ${CurrTestName} COMMAND ${CurrTestName})

# add a test
set(CurrTestName tgf_msg)
add_executable(${CurrTestName} ${CurrTestName}.cc
  ${PROJECT_SOURCE_DIR}/shared/tgf_msg.cc
  ${PROJECT_SOURCE_DIR}/shared/tgf_locale.cc
)
target_include_directories(${CurrTestName} PRIVATE ${PROJECT_SOURCE_DIR}/shared)
add_test(NAME ${CurrTestName} COMMAND ${CurrTestName})


# add a test
set(CurrTestName tgf_locale)
add_executable(${CurrTestName} ${CurrTestName}.cc
  ${PROJECT_SOURCE_DIR}/shared/tgf_locale.cc
)
target_include_directories(${CurrTestName} PRIVATE ${PROJECT_SOURCE_DIR}/shared)
add_test(NAME ${CurrTestName} COMMAND ${CurrTestName})
