project(aeirc-cui CXX)
cmake_minimum_required(VERSION 2.8)

if(WIN32 AND NOT CYGWIN)
  find_package(PDCurses REQUIRED)
else()
  find_package(Curses REQUIRED)
endif(WIN32 AND NOT CYGWIN)

if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif(WIN32)

if(PDCURSES_FOUND)
  include_directories(${PDCURSES_INCLUDE_DIR})

  set(aeirc-cui_SOURCES main.cpp)

  add_executable(aeirc-cui ${aeirc-cui_SOURCES})
  target_link_libraries(aeirc-cui ${PDCURSES_LIBRARY})

  install(TARGETS aeirc-cui
          RUNTIME DESTINATION bin
          LIBRARY DESTINATION lib
          ARCHIVE DESTINATION lib/static)
endif(PDCURSES_FOUND)
