include(CTest)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  add_compile_options(-Wno-unused-variable)
endif()

# --------------------------- Add A Test ---------------------------

set(CurrTestName posix_regex)
add_executable(${CurrTestName} t_${CurrTestName}.cc
   ${PROJECT_SOURCE_DIR}/src/${CurrTestName}.cc)
target_include_directories(${CurrTestName} PRIVATE
  ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(
  ${CurrTestName}
  PRIVATE -static-libgcc -static-libstdc++)
add_test(NAME ${CurrTestName} COMMAND ${CurrTestName})

# --------------------------- Add A Test ---------------------------

set(CurrTestName filter)
add_executable(${CurrTestName} t_${CurrTestName}.cc
   ${PROJECT_SOURCE_DIR}/src/${CurrTestName}.cc
   ${PROJECT_SOURCE_DIR}/src/posix_regex.cc
   ${PROJECT_SOURCE_DIR}/src/tg_msg.cc
   ${PROJECT_SOURCE_DIR}/src/locale_conf.cc)
target_include_directories(${CurrTestName} PRIVATE
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_SOURCE_DIR}/3rd/toml11)
target_link_libraries(
  ${CurrTestName}
  PRIVATE -static-libgcc -static-libstdc++)
add_test(NAME ${CurrTestName} COMMAND ${CurrTestName})

# --------------------------- Add A Test ---------------------------

set(CurrTestName user_data)
add_executable(${CurrTestName} t_${CurrTestName}.cc
   ${PROJECT_SOURCE_DIR}/src/${CurrTestName}.cc
   ${PROJECT_SOURCE_DIR}/src/locale_conf.cc
)
target_include_directories(${CurrTestName} PRIVATE
  ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(
  ${CurrTestName}
  PRIVATE -static-libgcc -static-libstdc++ Td::TdStatic)
add_test(NAME ${CurrTestName} COMMAND ${CurrTestName})

# --------------------------- Add A Test ---------------------------

set(CurrTestName tg_msg)
add_executable(${CurrTestName} t_${CurrTestName}.cc
  ${PROJECT_SOURCE_DIR}/src/${CurrTestName}.cc
  ${PROJECT_SOURCE_DIR}/src/locale_conf.cc)
target_include_directories(${CurrTestName} PRIVATE
  ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(
  ${CurrTestName}
  PRIVATE -static-libgcc -static-libstdc++)
add_test(NAME ${CurrTestName} COMMAND ${CurrTestName})

# --------------------------- Add A Test ---------------------------

set(CurrTestName locale_conf)
add_executable(${CurrTestName} t_${CurrTestName}.cc
  ${PROJECT_SOURCE_DIR}/src/${CurrTestName}.cc)
target_include_directories(${CurrTestName} PRIVATE
  ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(
  ${CurrTestName}
  PRIVATE -static-libgcc -static-libstdc++)
add_test(NAME ${CurrTestName} COMMAND ${CurrTestName})

# --------------------------- Add A Test ---------------------------

set(CurrTestName chatcmd)
add_executable(${CurrTestName} t_${CurrTestName}.cc
  ${PROJECT_SOURCE_DIR}/src/${CurrTestName}.cc
  ${PROJECT_SOURCE_DIR}/src/posix_regex.cc
  ${PROJECT_SOURCE_DIR}/src/filter.cc
  ${PROJECT_SOURCE_DIR}/src/stat.cc
  ${PROJECT_SOURCE_DIR}/src/user_data.cc
  ${PROJECT_SOURCE_DIR}/src/tg_msg.cc
  ${PROJECT_SOURCE_DIR}/src/td_coll.cc
  ${PROJECT_SOURCE_DIR}/src/ipc_msg_queue.cc
  ${PROJECT_SOURCE_DIR}/src/locale_conf.cc
  ${PROJECT_SOURCE_DIR}/src/log.cc)
target_include_directories(${CurrTestName} PRIVATE
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_SOURCE_DIR}/3rd/toml11)
target_link_libraries(
  ${CurrTestName}
  PRIVATE -static-libgcc -static-libstdc++ Td::TdStatic)
add_test(NAME ${CurrTestName} COMMAND ${CurrTestName})


