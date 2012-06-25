project(libaeirc CXX)
cmake_minimum_required(VERSION 2.8)

set(aeirc-common_SOURCES ../common/Message.cpp)
set(aeirc-common_HEADERS ../common/Message.hpp)

add_library(libaeirc SHARED ${aeirc-common_HEADERS} ${aeirc-common_SOURCES})

set_target_properties(libaeirc PROPERTIES PREFIX "")

install(TARGETS libaeirc
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)
