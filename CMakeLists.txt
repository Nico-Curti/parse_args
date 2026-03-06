cmake_minimum_required (VERSION 3.18)

# -------------------------------------------------------------------------
# Version / Description
# -------------------------------------------------------------------------
if (NOT ParseArgs_VERSION)
  set(ParseArgs_VERSION "1.0.0")
endif ()
if (NOT CMAKE_PROJECT_DESCRIPTION)
  set(CMAKE_PROJECT_DESCRIPTION "Parse Command Line Arguments in C++")
endif ()
message (STATUS "ParseArgs VERSION: ${ParseArgs_VERSION}")

string(REPLACE "." ";" VERSION_LIST ${ParseArgs_VERSION})
if (NOT ParseArgs_MAJOR)
  list (GET VERSION_LIST 0 ParseArgs_MAJOR)
endif ()
if (NOT ParseArgs_MINOR)
  list (GET VERSION_LIST 1 ParseArgs_MINOR)
endif ()
if (NOT ParseArgs_REVISION)
  list (GET VERSION_LIST 2 ParseArgs_REVISION)
endif ()

# -------------------------------------------------------------------------
# VCPKG detection
# -------------------------------------------------------------------------
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
  message(STATUS "VCPKG found: $ENV{VCPKG_ROOT}")
  message(STATUS "Using VCPKG integration")
  message(STATUS "VCPKG_MANIFEST_FEATURES: ${VCPKG_MANIFEST_FEATURES}")
  if(DEFINED ENV{VCPKG_DEFAULT_TRIPLET})
    message(STATUS "Setting default vcpkg target triplet to $ENV{VCPKG_DEFAULT_TRIPLET}")
    set(VCPKG_TARGET_TRIPLET $ENV{VCPKG_DEFAULT_TRIPLET})
  endif()
endif()

project (libParseArgs
  LANGUAGES CXX
  VERSION ${ParseArgs_VERSION}
  DESCRIPTION ${CMAKE_PROJECT_DESCRIPTION}
)

# this is mandatory for scikit-build check
enable_language(C)
enable_language(CXX)

# required by parse_args library
if ( ( "${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU" AND ${CMAKE_CXX_COMPILER_VERSION} LESS 7) OR ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" AND ${CMAKE_CXX_COMPILER_VERSION} LESS_EQUAL 4) )
  set (CMAKE_CXX_STANDARD 14)
  add_definitions(-D__old_cpp_std__)
  message (STATUS "The provided compiler version does not support the c++17 standard. Switch to old gcc compatibility.")
else ()
  set (CMAKE_CXX_STANDARD 17)
endif()
set (CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions (
  -DMAJOR=${ParseArgs_MAJOR}
  -DMINOR=${ParseArgs_MINOR}
  -DREVISION=${ParseArgs_REVISION}
)

#################################################################
#                         COMPILE OPTIONS                       #
#################################################################

option (BUILD_DOCS  "Enable Documentaion builid   support" OFF )

#################################################################
#                         SETTING VARIABLES                     #
#################################################################

# -------------------------------------------------------------------------
# CMake modules path
# -------------------------------------------------------------------------
set(CMAKE_MODULE_PATH
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/"
  ${CMAKE_MODULE_PATH}
)

include(GNUInstallDirs)

if ( NOT APPLE )
  set (CMAKE_SKIP_BUILD_RPATH             FALSE )
  set (CMAKE_BUILD_WITH_INSTALL_RPATH     FALSE )
  set (CMAKE_INSTALL_RPATH_USE_LINK_PATH  TRUE  )
endif ()

# make sure that the default is a RELEASE
set (default_build_type "Release")
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message (STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set (CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build." FORCE)
  set_property (CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif ()

# -------------------------------------------------------------------------
# Compile flags section
# -------------------------------------------------------------------------
if ( CMAKE_COMPILER_IS_GNUCXX )
  add_compile_options (-Wall -Wextra -Wno-unused-result -Wno-unknown-pragmas -Wfatal-errors -Wno-narrowing -Wpedantic -Wno-dev)
  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    string (REGEX REPLACE "-O[0123]" "-Og" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG" )
    set (CMAKE_CXX_FLAGS_RELEASE "")
  elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG" )
    set (CMAKE_CXX_FLAGS_DEBUG "")
  endif()
endif()

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  add_compile_options(-Wno-deprecated -Wno-writable-strings -Wno-dev)
  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    string (REGEX REPLACE "-O0" "-Og" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG" )
  elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG" )
  endif()
endif()

if (MSVC)
  set (CMAKE_CXX_FLAGS "/wd4013 /wd4018 /wd4028 /wd4047 /wd4068 /wd4090 /wd4101 /wd4113 /wd4133 /wd4190 /wd4244 /wd4267 /wd4305 /wd4477 /wd4996 /wd4819 /fp:fast ${CMAKE_CXX_FLAGS}")
  string (REGEX REPLACE "/O2" "/Ox" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
  add_definitions (-D_CRT_RAND_S)
  add_definitions (-DNOMINMAX)
  add_definitions (-D_USE_MATH_DEFINES)
  add_definitions (-D_CRT_SECURE_NO_DEPRECATE -D_SCL_SECURE_NO_WARNINGS)
  set (CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif ()

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
if (COMPILER_SUPPORTS_MARCH_NATIVE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
endif()

# -------------------------------------------------------------------------
# Install layout
# -------------------------------------------------------------------------
set(CMAKE_DEBUG_POSTFIX d)

set(ParseArgs_INSTALL_BIN_DIR     "${CMAKE_INSTALL_BINDIR}" CACHE PATH "Install dir for executables")
set(ParseArgs_INSTALL_LIB_DIR     "${CMAKE_INSTALL_LIBDIR}" CACHE PATH "Install dir for libraries")
set(ParseArgs_INSTALL_INCLUDE_DIR "${CMAKE_INSTALL_INCLUDEDIR}/parse_args" CACHE PATH "Install dir for headers")
set(ParseArgs_INSTALL_CMAKE_DIR   "${CMAKE_INSTALL_LIBDIR}/cmake/parse_args" CACHE PATH "Install dir for CMake package files")

# Variables used by .pc.in templates (keep compatibility)
set(INSTALL_BIN_DIR     "${ParseArgs_INSTALL_BIN_DIR}")
set(INSTALL_LIB_DIR     "${ParseArgs_INSTALL_LIB_DIR}")
set(INSTALL_INCLUDE_DIR "${ParseArgs_INSTALL_INCLUDE_DIR}")
set(INSTALL_CMAKE_DIR   "${ParseArgs_INSTALL_CMAKE_DIR}")

set (ParseArgslib ParseArgs)

# generate pkg-config + version header
configure_file(
  "${CMAKE_CURRENT_LIST_DIR}/parseargs.pc.in"
  "${CMAKE_CURRENT_BINARY_DIR}/parseargs.pc" @ONLY
)
message(STATUS "Pkg-config generated")

file(GLOB ParseArgs_SRC    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" )
file(GLOB ParseArgs_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h" )
file(GLOB ParseArgs_HPP    "${CMAKE_CURRENT_SOURCE_DIR}/hpp/*.hpp" )

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/hpp")

# Make relative paths absolute (needed later on)
foreach (p LIB BIN INCLUDE CMAKE)
  set (var INSTALL_${p}_DIR)
  if (NOT IS_ABSOLUTE "${${var}}")
    set (FULLPATH_${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

# -------------------------------------------------------------------------
# SUMMARY
# -------------------------------------------------------------------------
message(STATUS ""                                                                          )
message(STATUS "=================== ParseArgs configuration Summary =================="      )
message(STATUS "   ParseArgs version: ${ParseArgs_VERSION}"                                    )
message(STATUS ""                                                                          )
message(STATUS "   Build type      : ${CMAKE_BUILD_TYPE}"                                  )
message(STATUS "   Install prefix  : ${CMAKE_INSTALL_PREFIX}"                              )
message(STATUS "   Install bin dir : ${ParseArgs_INSTALL_BIN_DIR}"                           )
message(STATUS "   Install lib dir : ${ParseArgs_INSTALL_LIB_DIR}"                           )
message(STATUS "   Install inc dir : ${ParseArgs_INSTALL_INCLUDE_DIR}"                       )
message(STATUS "   Install cmake dir: ${ParseArgs_INSTALL_CMAKE_DIR}"                        )
message(STATUS ""                                                                          )
message(STATUS "   Documentation support : ${BUILD_DOCS}"                                  )
message(STATUS ""                                                                          )

# print compiler flags
string(REPLACE " " ";" CMAKE_CXX_FLAGS_LIST "${CMAKE_CXX_FLAGS}")
message(STATUS "      C++ Compiler : ${CMAKE_CXX_COMPILER}"                                )
message(STATUS "      C++ flags    :"                                                      )
foreach(FLAG ${CMAKE_CXX_FLAGS_LIST})
  message(STATUS "                    * ${FLAG}"                                           )
endforeach()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  message(STATUS "      C++ Debug flags:"                                                  )
  string(REPLACE " " ";" _DBG "${CMAKE_CXX_FLAGS_DEBUG}")
  foreach(FLAG ${_DBG})
    message(STATUS "                    * ${FLAG}"                                         )
  endforeach()
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
  message(STATUS "      C++ Release flags  :"                                              )
  string(REPLACE " " ";" _REL "${CMAKE_CXX_FLAGS_RELEASE}")
  foreach(FLAG ${_REL})
    message(STATUS "                    * ${FLAG}"                                         )
  endforeach()
endif()

message(STATUS "      Linker flags : "                                                     )
foreach(FLAG ${linked_libs})
  message(STATUS "                    * ${FLAG}"                                           )
endforeach(FLAG)
message(STATUS ""                                                                          )


#################################################################
#                         MAIN RULES                            #
#################################################################

set(_ParseArgs_LIBTYPE SHARED)

add_library(
  ${ParseArgslib}
  ${_ParseArgs_LIBTYPE}
  ${ParseArgs_SRC} ${ParseArgs_HEADER} ${ParseArgs_HPP}
)
set_target_properties(${ParseArgslib} PROPERTIES POSITION_INDEPENDENT_CODE ON)
# Ensure the actual library name is always libParseArgs (so pkg-config uses -lParseArgs)
set_target_properties(${ParseArgslib} PROPERTIES OUTPUT_NAME ParseArgs)

target_include_directories(${ParseArgslib}
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/hpp>
  $<INSTALL_INTERFACE:${ParseArgs_INSTALL_INCLUDE_DIR}>
)
target_link_libraries(${ParseArgslib} ${linked_libs})

set_target_properties(${ParseArgslib} PROPERTIES PUBLIC_HEADER "${ParseArgs_HEADER}")

install(TARGETS ${ParseArgslib} EXPORT ParseArgsTargets
  RUNTIME DESTINATION "${ParseArgs_INSTALL_BIN_DIR}"
  LIBRARY DESTINATION "${ParseArgs_INSTALL_LIB_DIR}"
  ARCHIVE DESTINATION "${ParseArgs_INSTALL_LIB_DIR}"
  PUBLIC_HEADER DESTINATION "${ParseArgs_INSTALL_INCLUDE_DIR}"
  COMPONENT dev
)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/parseargs.pc"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
)

add_custom_target(example)
add_executable(runtest "${CMAKE_CURRENT_LIST_DIR}/example/runtest.cpp")
add_dependencies(example runtest)
target_link_libraries(runtest ${linked_libs} ${ParseArgslib})
install(TARGETS runtest DESTINATION "${ParseArgs_INSTALL_BIN_DIR}")

# -------------------------------------------------------------------------
# EXPORT ParseArgs
# -------------------------------------------------------------------------

install(EXPORT ParseArgsTargets
  FILE ParseArgsTargets.cmake
  NAMESPACE ParseArgs::
  DESTINATION "${ParseArgs_INSTALL_CMAKE_DIR}"
)

include(CMakePackageConfigHelpers)

# Generate Config + Version files
configure_package_config_file(
  "${CMAKE_CURRENT_LIST_DIR}/ParseArgsConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/ParseArgsConfig.cmake"
  INSTALL_DESTINATION "${ParseArgs_INSTALL_CMAKE_DIR}"
  PATH_VARS ParseArgs_INSTALL_INCLUDE_DIR ParseArgs_INSTALL_LIB_DIR
)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/ParseArgsConfigVersion.cmake"
  VERSION "${PROJECT_VERSION}"
  COMPATIBILITY SameMajorVersion
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/ParseArgsConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/ParseArgsConfigVersion.cmake"
  DESTINATION "${ParseArgs_INSTALL_CMAKE_DIR}"
)

# optional: register build-tree package
export(PACKAGE ParseArgs)

# This must be the latest subdirectory included!!
if (BUILD_DOCS)
  add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/docs/")
endif ()
