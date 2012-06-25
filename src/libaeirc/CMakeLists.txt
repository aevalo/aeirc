project(libaeirc CXX)
cmake_minimum_required(VERSION 2.8)

add_library(libaeirc SHARED ../common/Message.hpp ../common/Message.cpp)

set_target_properties(libaeirc PROPERTIES PREFIX "")

install(TARGETS libaeirc
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)
