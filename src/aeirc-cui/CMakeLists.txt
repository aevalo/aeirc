project(aeirc-cui CXX)
cmake_minimum_required(VERSION 2.8)

set(AEIRC-CUI_VERSION_MAJOR 0)
set(AEIRC-CUI_VERSION_MINOR 1)
set(AEIRC-CUI_VERSION_BUILD 1)
set(AEIRC-CUI_VERSION "${AEIRC-CUI_VERSION_MAJOR}.${AEIRC-CUI_VERSION_MINOR}.${AEIRC-CUI_VERSION_BUILD}")

if(WIN32 AND NOT CYGWIN)
  find_package(PDCurses REQUIRED)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
else()
  find_package(Curses REQUIRED)
endif(WIN32 AND NOT CYGWIN)


if(PDCURSES_FOUND)
  set(CUILIB_INCLUDE_DIRS ${PDCURSES_INCLUDE_DIR})
  set(CUILIB_LIBRARIES ${PDCURSES_LIBRARIES})
elseif(CURSES_FOUND)
  set(CUILIB_INCLUDE_DIRS ${CURSES_INCLUDE_DIRS})
  set(CUILIB_LIBRARIES ${CURSES_LIBRARIES})
else()
  message(SEND_ERROR "Could not find CUI library.")
endif(PDCURSES_FOUND)

include_directories(${CUILIB_INCLUDE_DIRS})

set(aeirc-cui_HEADERS tui.hpp tui_internal.hpp tui_types.hpp)
set(aeirc-cui_SOURCES tui.cpp tui_internal.cpp tui_types.cpp main.cpp)

add_executable(aeirc-cui ${aeirc-cui_SOURCES})
target_link_libraries(aeirc-cui ${CUILIB_LIBRARIES})

set_target_properties(aeirc-cui PROPERTIES VERSION ${AEIRC-CUI_VERSION})

install(TARGETS aeirc-cui
      RUNTIME DESTINATION bin
      LIBRARY DESTINATION lib
      ARCHIVE DESTINATION lib/static)
