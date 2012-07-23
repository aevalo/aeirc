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
  configure_file("libaeirc.rc.in" "libaeirc.rc" NEWLINE_STYLE WIN32)
  set(aeirc-common_SOURCES ${aeirc-common_SOURCES} ${CMAKE_CURRENT_BINARY_DIR}/libaeirc.rc)
endif(WIN32)

add_library(libaeirc SHARED ${aeirc-common_HEADERS} ${aeirc-common_SOURCES})
set_target_properties(libaeirc PROPERTIES PREFIX "" VERSION ${LIBAEIRC_VERSION} SOVERSION ${LIBAEIRC_VERSION})

create_translantion_targets(${PROJECT_SOURCE_DIR}/src/hellogt.cpp fi en)

install(TARGETS libaeirc
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)
