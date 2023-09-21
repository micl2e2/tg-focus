include(CTest)

set(CurrTestName posix_regex_test)
add_executable(${CurrTestName} ${CurrTestName}.cc)
target_include_directories(${CurrTestName} PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(${CurrTestName} posix_regex)
add_test(NAME ${CurrTestName} COMMAND ${CurrTestName})

set(CurrTestName focus_filter_test)
add_executable(${CurrTestName} ${CurrTestName}.cc)
target_include_directories(${CurrTestName} PRIVATE
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/deps/toml11)
target_link_libraries(${CurrTestName} focus_filter)
add_test(NAME ${CurrTestName} COMMAND ${CurrTestName})

set(CurrTestName tfdata_test)
add_executable(${CurrTestName} ${CurrTestName}.cc)
target_include_directories(${CurrTestName} PRIVATE
  ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(${CurrTestName} tf_data)
add_test(NAME ${CurrTestName} COMMAND ${CurrTestName})

