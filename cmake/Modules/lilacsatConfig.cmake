INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_LILACSAT lilacsat)

FIND_PATH(
    LILACSAT_INCLUDE_DIRS
    NAMES lilacsat/api.h
    HINTS $ENV{LILACSAT_DIR}/include
        ${PC_LILACSAT_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    LILACSAT_LIBRARIES
    NAMES gnuradio-lilacsat
    HINTS $ENV{LILACSAT_DIR}/lib
        ${PC_LILACSAT_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/lilacsatTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(LILACSAT DEFAULT_MSG LILACSAT_LIBRARIES LILACSAT_INCLUDE_DIRS)
MARK_AS_ADVANCED(LILACSAT_LIBRARIES LILACSAT_INCLUDE_DIRS)
