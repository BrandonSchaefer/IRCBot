set (GMOCK_ROOT_DIR
     "/usr/src/gmock"
     CACHE
     PATH
     "Path to Google mock srcs"
)

set (GMOCK_INCLUDE_DIR
     "/usr/include/gmock/include"
     CACHE
     PATH
     "Path to Google mock include"
)

set (GTEST_ROOT_DIR
     "/usr/src/gtest"
     CACHE
     PATH
     "Path to Google Tests source"
     FORCE)

set (GTEST_INCLUDE_DIR
     "/usr/include/gtest"
     CACHE
     PATH
     "Path to Google Tests include"
     FORCE)

if (GTEST_INCLUDE_DIR AND GMOCK_INCLUDE_DIR)
  include_directories (
    ${GMOCK_INCLUDE_DIR}
    ${GTEST_INCLUDE_DIR}
  )
  add_subdirectory(
    ${GMOCK_ROOT_DIR}
    "${CMAKE_CURRENT_BINARY_DIR}/gmock"
  )
elseif (GTEST_INCLUDE_DIR AND NOT GMOCK_INCLUDE_DIR)
  include_directories (
    ${GTEST_INCLUDE_DIR}
  )
endif(GTEST_INCLUDE_DIR AND GMOCK_INCLUDE_DIR)

enable_testing ()

set (GMOCK_LIB gmock)
set (GMOCK_MAIN_LIB gmock_main)
find_path (GTEST_SRC_DIR gtest.cc PATHS /usr/src/gtest/src)

if (GTEST_SRC_DIR AND
    GMOCK_LIB AND
    GMOCK_MAIN_LIB)

  set (GTEST_GMOCK_LIB
       gtest
       ${GMOCK_LIB}
       ${GMOCK_MAIN_LIB}
      )

  set (GTEST_BACKEND_SOURCE
       test_command_handler.cpp
       test_irc_bot_controller.cpp
       test_load_basic_commands.cpp
       test_string_manipulation.cpp
       test_sym_weapons.cpp
       test_read_write_io.cpp
       test_main.cpp
      )

  include_directories (${IRC_BOT_SOURCE_DIR}/src ${GTEST_INCLUDE_DIR} ${CMAKE_BINARY_DIR})

  add_executable        (gtest_irc_bot ${GTEST_BACKEND_SOURCE})
  target_link_libraries (gtest_irc_bot irc_bot_lib -lcurl ${GTEST_GMOCK_LIB})

  add_custom_target (check COMMAND make -j4 COMMAND gtest_irc_bot)

endif (GTEST_SRC_DIR AND
       GMOCK_LIB AND
       GMOCK_MAIN_LIB)
