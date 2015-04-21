set(MATHSAT_PREFIX "" CACHE PATH "path ")

find_path(MATHSAT_INCLUDE_DIR mathsat.h
  PATHS ${MATHSAT_PREFIX}/include /usr/include /usr/local/include )

find_library(MATHSAT_LIBRARY NAMES mathsat libmathsat
  PATHS ${MATHSAT_PREFIX}/lib /usr/lib /usr/local/lib )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MathSAT  DEFAULT_MSG
                                  MATHSAT_LIBRARY MATHSAT_INCLUDE_DIR)
