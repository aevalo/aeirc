# - Try to find LibIntl
# Once done this will define
#  LIBINTL_FOUND - System has LibIntl
#  LIBINTL_INCLUDE_DIR - The LibIntl include directories
#  LIBINTL_LIBRARY - The libraries needed to use LibIntl

find_path(LIBINTL_INCLUDE_DIR libintl.h
  HINTS $ENV{LIBINTLDIR}
  PATHS /usr/ usr/local/
  PATH_SUFFIXES include)

find_library(LIBINTL_LIBRARY NAMES intl libintl
  HINTS $ENV{LIBINTLDIR}
  PATHS /usr/ usr/local/
  PATH_SUFFIXES lib)

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBINTL_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(LibIntl DEFAULT_MSG LIBINTL_LIBRARY LIBINTL_INCLUDE_DIR)

mark_as_advanced(LIBINTL_INCLUDE_DIR LIBINTL_LIBRARY)