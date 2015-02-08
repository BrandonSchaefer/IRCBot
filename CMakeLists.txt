project (IRC_BOT)
cmake_minimum_required (VERSION 2.8.9)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -g3 -std=c++0x -Werror -Wall -pedantic -Wcast-align -Wempty-body -Wformat-security -Winit-self -Warray-bounds")

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

set (CMAKE_CXX_FLAGS_DEBUG "-g3 -O0")
set (CMAKE_CXX_FLAGS_RELEASE "-O2")

set (CMAKE_AUTOMOC ON)
set (CMAKE_INCLUDE_CURRENT_DIR ON)

set (SAVESDIR "${CMAKE_SOURCE_DIR}/saves")
set (TESTDIR  "${CMAKE_SOURCE_DIR}/tests")
configure_file (${CMAKE_SOURCE_DIR}/config.h.cmake ${CMAKE_BINARY_DIR}/config.h)

if(WIN32)
  if(NOT WINDOWS)
    set(WINDOWS TRUE)
  endif()
elseif(UNIX)
  if(CMAKE_SYSTEM_NAME MATCHES ".*Linux")
    set(LINUX TRUE)
  endif()
endif()

add_subdirectory(src)

# FIXME Figure out how to get gtest working on windows...
if (LINUX)
  option(ENABLE_TESTS "Bulid tests" ON)

  if (ENABLE_TESTS)
    add_subdirectory(tests)
  endif ()
endif()
