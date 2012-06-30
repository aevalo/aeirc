project(libaeirc CXX)
cmake_minimum_required(VERSION 2.8)

set(LIBAEIRC_VERSION_MAJOR 0)
set(LIBAEIRC_VERSION_MINOR 1)
set(LIBAEIRC_VERSION_BUILD 1)
set(LIBAEIRC_VERSION "${LIBAEIRC_VERSION_MAJOR}.${LIBAEIRC_VERSION_MINOR}.${LIBAEIRC_VERSION_BUILD}")

find_package(Boost 1.47.0 COMPONENTS system regex thread REQUIRED)
find_package(OpenSSL REQUIRED)

include_directories(${Boost_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

set(aeirc-common_SOURCES ../common/Message.cpp)
set(aeirc-common_HEADERS ../common/Message.hpp)

if(WIN32)
  add_definitions(-D_WIN32_WINNT=0x0501 -D_CRT_SECURE_NO_WARNINGS -D_ATL_SECURE_NO_WARNINGS -D_AFX_SECURE_NO_WARNINGS)
  
  set(VER_FILEVERSION "${LIBAEIRC_VERSION_MAJOR},${LIBAEIRC_VERSION_MINOR},${LIBAEIRC_VERSION_BUILD},0")
  set(VER_FILEVERSION_STR "${LIBAEIRC_VERSION_MAJOR}.${LIBAEIRC_VERSION_MINOR}.${LIBAEIRC_VERSION_BUILD}.0\\0")
  
  set(VER_PRODUCTVERSION "${LIBAEIRC_VERSION_MAJOR},${LIBAEIRC_VERSION_MINOR},${LIBAEIRC_VERSION_BUILD},0")
  set(VER_PRODUCTVERSION_STR "${LIBAEIRC_VERSION_MAJOR}.${LIBAEIRC_VERSION_MINOR}\\0")
  
  configure_file("libaeirc.rc.in" "libaeirc.rc" NEWLINE_STYLE WIN32)
  
  set(aeirc-common_SOURCES ${aeirc-common_SOURCES} ${CMAKE_CURRENT_BINARY_DIR}/libaeirc.rc)
  
  message(STATUS "Current CMake binary dir: ${CMAKE_CURRENT_BINARY_DIR}")
  message(STATUS "Current CMake source dir: ${CMAKE_CURRENT_SOURCE_DIR}")
  message(STATUS ${aeirc-common_SOURCES})
endif(WIN32)

add_library(libaeirc SHARED ${aeirc-common_HEADERS} ${aeirc-common_SOURCES})

set_target_properties(libaeirc PROPERTIES PREFIX "" VERSION ${LIBAEIRC_VERSION} SOVERSION ${LIBAEIRC_VERSION})

install(TARGETS libaeirc
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)
