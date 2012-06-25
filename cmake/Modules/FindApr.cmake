# Locate apr library
# This module defines
# APR_LIBRARY, the name of the library to link against
# APR_FOUND, if false, do not try to link to apr
# APR_INCLUDE_DIR, where to find apr.h

FIND_PATH(APR_INCLUDE_DIR apr.h
  HINTS
  $ENV{APRDIR}
  PATH_SUFFIXES include/apr include)

FIND_LIBRARY(APR_LIBRARY
  NAMES apr apr-1
  HINTS
  $ENV{APRDIR}
  PATH_SUFFIXES lib64 lib)

SET(APR_FOUND "NO")
IF(APR_INCLUDE_DIR AND APR_LIBRARY)
  # Set the final string here so the GUI reflects the final state.
  SET(APR_LIBRARY ${APR_LIBRARY} CACHE STRING "Where the Apache Portable Runtime (apr) Library can be found")

  SET(APR_FOUND "YES")
ENDIF(APR_INCLUDE_DIR AND APR_LIBRARY)
