# Locate PDCurses library
# This module defines
# PDCURSES_LIBRARY, the name of the library to link against
# PDCURSES_FOUND, if false, do not try to link to PDCurses
# PDCURSES_INCLUDE_DIR, where to find curses.h

FIND_PATH(PDCURSES_INCLUDE_DIR curses.h
  HINTS
  $ENV{PDCURSESDIR}
  PATH_SUFFIXES include/pdcurses include)

FIND_LIBRARY(PDCURSES_LIBRARY
  NAMES pdcurses
  HINTS
  $ENV{PDCURSESDIR}
  PATH_SUFFIXES lib64 lib)

SET(PDCURSES_FOUND "NO")
IF(PDCURSES_INCLUDE_DIR AND PDCURSES_LIBRARY)
  message(STATUS "Found PDCurses library: ${PDCURSES_LIBRARY}")
  # Set the final string here so the GUI reflects the final state.
  SET(PDCURSES_LIBRARY PDCURSES_LIBRARYAPR_LIBRARY} CACHE STRING "Where the APDCurses Library can be found")

  SET(PDCURSES_FOUND "YES")
ENDIF(PDCURSES_INCLUDE_DIR AND PDCURSES_LIBRARY)
