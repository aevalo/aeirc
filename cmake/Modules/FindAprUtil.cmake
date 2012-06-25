# Locate apr library
# This module defines
# APR_UTIL_LIBRARY, the name of the library to link against
# APR_UTIL_FOUND, if false, do not try to link to apr
# APR_UTIL_INCLUDE_DIR, where to find apr.h

FIND_PATH(APR_UTIL_INCLUDE_DIR apu.h
  HINTS
  $ENV{APRUTILDIR}
  PATH_SUFFIXES include/apr-util include)

FIND_LIBRARY(APR_UTIL_LIBRARY
  NAMES aprutil aprutil-1
  HINTS
  $ENV{APRUTILDIR}
  PATH_SUFFIXES lib64 lib)

SET(APR_UTIL_FOUND "NO")
IF(APR_UTIL_INCLUDE_DIR AND APR_UTIL_LIBRARY)
  # Set the final string here so the GUI reflects the final state.
  SET(APR_UTIL_LIBRARY ${APR_UTIL_LIBRARY} CACHE STRING "Where the APR-Utility Library can be found")

  SET(APR_UTIL_FOUND "YES")
ENDIF(APR_UTIL_INCLUDE_DIR AND APR_UTIL_LIBRARY)
