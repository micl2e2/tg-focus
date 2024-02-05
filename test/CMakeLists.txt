include(CTest)

# add a test
set(CurrTestName posix_regex_test)
add_executable(${CurrTestName} ${CurrTestName}.cc
   ${PROJECT_SOURCE_DIR}/shared/posix_regex.cc
)
target_include_directories(${CurrTestName} PRIVATE ${PROJECT_SOURCE_DIR}/shared)
add_test(NAME ${CurrTestName} COMMAND ${CurrTestName})

# add a test
set(CurrTestName tst_focus_filter_config)
add_executable(${CurrTestName} ${CurrTestName}.cc
   ${PROJECT_SOURCE_DIR}/shared/focus_filter.cc
   ${PROJECT_SOURCE_DIR}/shared/posix_regex.cc
   ${PROJECT_SOURCE_DIR}/shared/tf_msg.cc
   ${PROJECT_SOURCE_DIR}/shared/tgf_locale.cc
)
target_include_directories(${CurrTestName} PRIVATE
  ${PROJECT_SOURCE_DIR}/shared
  ${PROJECT_SOURCE_DIR}/3rd/toml11)
add_test(NAME ${CurrTestName} COMMAND ${CurrTestName})

# add a test
set(CurrTestName focus_filter_test)
add_executable(${CurrTestName} ${CurrTestName}.cc
   ${PROJECT_SOURCE_DIR}/shared/focus_filter.cc
   ${PROJECT_SOURCE_DIR}/shared/posix_regex.cc
   ${PROJECT_SOURCE_DIR}/shared/tf_msg.cc
      ${PROJECT_SOURCE_DIR}/shared/tgf_locale.cc

)
target_include_directories(${CurrTestName} PRIVATE
  ${PROJECT_SOURCE_DIR}/shared
  ${PROJECT_SOURCE_DIR}/3rd/toml11)
add_test(NAME ${CurrTestName} COMMAND ${CurrTestName})

# add a test
set(CurrTestName tfdata_test)
add_executable(${CurrTestName} ${CurrTestName}.cc
   ${PROJECT_SOURCE_DIR}/shared/tf_data.cc
   ${PROJECT_SOURCE_DIR}/shared/tgf_locale.cc
)
target_include_directories(${CurrTestName} PRIVATE
  ${PROJECT_SOURCE_DIR}/shared)
add_test(NAME ${CurrTestName} COMMAND ${CurrTestName})

# add a test
set(CurrTestName tf_msg_test)
add_executable(${CurrTestName} ${CurrTestName}.cc
  ${PROJECT_SOURCE_DIR}/shared/tf_msg.cc
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
