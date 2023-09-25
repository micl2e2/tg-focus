include(CTest)

set(CurrTestName posix_regex_test)
add_executable(${CurrTestName} ${CurrTestName}.cc
   ${PROJECT_SOURCE_DIR}/shared/posix_regex.cc
)
target_include_directories(${CurrTestName} PRIVATE ${PROJECT_SOURCE_DIR}/shared)
add_test(NAME ${CurrTestName} COMMAND ${CurrTestName})

set(CurrTestName focus_filter_test)
add_executable(${CurrTestName} ${CurrTestName}.cc
   ${PROJECT_SOURCE_DIR}/shared/focus_filter.cc
   ${PROJECT_SOURCE_DIR}/shared/posix_regex.cc
   ${PROJECT_SOURCE_DIR}/shared/tf_msg.cc
)
target_include_directories(${CurrTestName} PRIVATE
  ${PROJECT_SOURCE_DIR}/shared
  ${PROJECT_SOURCE_DIR}/3rd/toml11)
add_test(NAME ${CurrTestName} COMMAND ${CurrTestName})

set(CurrTestName tfdata_test)
add_executable(${CurrTestName} ${CurrTestName}.cc
   ${PROJECT_SOURCE_DIR}/shared/tf_data.cc
)
target_include_directories(${CurrTestName} PRIVATE
  ${PROJECT_SOURCE_DIR}/shared)
add_test(NAME ${CurrTestName} COMMAND ${CurrTestName})

set(CurrTestName tf_msg_test)
add_executable(${CurrTestName} ${CurrTestName}.cc
   ${PROJECT_SOURCE_DIR}/shared/tf_msg.cc
)
target_include_directories(${CurrTestName} PRIVATE ${PROJECT_SOURCE_DIR}/shared)
add_test(NAME ${CurrTestName} COMMAND ${CurrTestName})
