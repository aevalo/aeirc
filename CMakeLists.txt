project(aeirc CXX)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/cmake/Modules
)

include(EnsureOutOfSourceBuild)

EnsureOutOfSourceBuild("Out-of-source build required.")

find_package(Boost 1.47.0 COMPONENTS system regex thread)
find_package(OpenSSL)
if(Boost_FOUND AND OPENSSL_FOUND)
    include_directories(${Boost_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR})
    link_directories(${Boost_LIBRARY_DIRS})
    
    if(WIN32)
        add_definitions(-D_WIN32_WINNT=0x0501 -D_CRT_SECURE_NO_WARNINGS -D_ATL_SECURE_NO_WARNINGS -D_AFX_SECURE_NO_WARNINGS)
    endif(WIN32)
    
    add_subdirectory(src/libaeirc)
    
    if(WIN32 AND NOT UNIX)
      install(CODE "file(MAKE_DIRECTORY \"${CMAKE_INSTALL_PREFIX}/logs/\")")
      install(CODE "file(MAKE_DIRECTORY \"${CMAKE_INSTALL_PREFIX}/config/\")")
    endif(WIN32 AND NOT UNIX)
endif(Boost_FOUND AND OPENSSL_FOUND)
