find_package(PkgConfig)

PKG_CHECK_MODULES(PC_GR_LILACSAT gnuradio-lilacsat)

FIND_PATH(
    GR_LILACSAT_INCLUDE_DIRS
    NAMES gnuradio/lilacsat/api.h
    HINTS $ENV{LILACSAT_DIR}/include
        ${PC_LILACSAT_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    GR_LILACSAT_LIBRARIES
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

include("${CMAKE_CURRENT_LIST_DIR}/gnuradio-lilacsatTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GR_LILACSAT DEFAULT_MSG GR_LILACSAT_LIBRARIES GR_LILACSAT_INCLUDE_DIRS)
MARK_AS_ADVANCED(GR_LILACSAT_LIBRARIES GR_LILACSAT_INCLUDE_DIRS)
