project(aeirc CXX)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)

include(EnsureOutOfSourceBuild)
include(GettextHelpers)

EnsureOutOfSourceBuild("Out-of-source build required.")

find_package(Gettext REQUIRED)
find_package(LibIntl REQUIRED)

add_subdirectory(src/libaeirc)
add_subdirectory(src/aeirc-gui)
add_subdirectory(src/aeirc-cui)
add_dependencies(aeirc-gui libaeirc)
add_dependencies(aeirc-cui libaeirc)

if(WIN32 AND NOT UNIX)
  install(CODE "file(MAKE_DIRECTORY \"${CMAKE_INSTALL_PREFIX}/logs/\")")
  install(CODE "file(MAKE_DIRECTORY \"${CMAKE_INSTALL_PREFIX}/config/\")")
endif(WIN32 AND NOT UNIX)
