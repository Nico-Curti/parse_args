cmake_minimum_required (VERSION 3.12.0)

# parse args Version
set (ParseArgs_MAJOR_VERSION 1)
set (ParseArgs_MINOR_VERSION 0)
set (ParseArgs_PATCH_VERSION 0)
set (ParseArgs_VERSION ${ParseArgs_MAJOR_VERSION}.${ParseArgs_MINOR_VERSION}.${ParseArgs_PATCH_VERSION})
set (CMAK_PROJECT_HOMEPAGE_URL "https://github.com/Nico-Curti/parse_args")

message (STATUS "ParseArgs VERSION: ${ParseArgs_VERSION}")

project (parse_args LANGUAGES CXX VERSION ${ParseArgs_VERSION} DESCRIPTION "Parse Command Line Arguments in C++")
enable_language(CXX)

if ( ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" AND ${CMAKE_CXX_COMPILER_VERSION} LESS 7) OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND ${CMAKE_CXX_COMPILER_VERSION} LESS 4) )

  set (CMAKE_CXX_STANDARD 14)
  add_definitions(-D__old_cpp_std__)
  message (STATUS "The provided compiler version does not support the c++17 standard. Switch to old gcc compatibility.")

else ()

  set (CMAKE_CXX_STANDARD 17)

endif()


#################################################################
#                         COMPILE OPTIONS                       #
#################################################################

option (BUILD_DOCS  "Enable Documentaion builid   support" OFF )

#################################################################
#                         SETTING VARIABLES                     #
#################################################################

if ( NOT APPLE )
  set (CMAKE_SKIP_BUILD_RPATH             FALSE )
  set (CMAKE_BUILD_WITH_INSTALL_RPATH     FALSE )
  set (CMAKE_INSTALL_RPATH_USE_LINK_PATH  TRUE  )
endif()

# make sure that the default is a RELEASE
set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

if ( CMAKE_COMPILER_IS_GNUCXX )
  add_compile_options (-Wall -Wextra -Wno-unused-result -Wno-unknown-pragmas -Wfatal-errors -Wno-narrowing -Wpedantic)
  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    string (REGEX REPLACE "-O[0123]" "-Og" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG" )
    set (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG" )
    set (CMAKE_CXX_FLAGS_RELEASE "")
    set (CMAKE_C_FLAGS_RELEASE "")
  elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    string (REGEX REPLACE "-O3" "-Ofast" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
    set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG" )
    set (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG" )
    set (CMAKE_CXX_FLAGS_DEBUG "")
    set (CMAKE_C_FLAGS_DEBUG "")
  endif()
endif()

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  add_compile_options(-Wno-deprecated -Wno-writable-strings)
  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    string (REGEX REPLACE "-O0" "-Og" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
    string (REGEX REPLACE "-O0" "-Og" CMAKE_C_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG" )
    set (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG" )
  elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    string (REGEX REPLACE "-O3" "-Ofast" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
    string (REGEX REPLACE "-O3" "-Ofast" CMAKE_C_FLAGS_RELEASE ${CMAKE_C_FLAGS_RELEASE})
    set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG" )
    set (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG" )
  endif()
endif()

if (MSVC)
  add_definitions (-D_CRT_RAND_S)
  add_definitions (-DNOMINMAX)
  add_definitions (-D_USE_MATH_DEFINES)
  add_definitions (-D_CRT_SECURE_NO_DEPRECATE -D_SCL_SECURE_NO_WARNINGS)
  set (CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

  set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /DCMAKE_BUILD /DG_DISABLE_SINGLE_INCLUDES")
  set(ADDITIONAL_CXX_FLAGS "/wd4013 /wd4018 /wd4028 /wd4047 /wd4068 /wd4090 /wd4101 /wd4113 /wd4133 /wd4190 /wd4244 /wd4267 /wd4305 /wd4477 /wd4996 /wd4819 /fp:fast")
  set(ADDITIONAL_C_FLAGS "/wd4013 /wd4018 /wd4028 /wd4047 /wd4068 /wd4090 /wd4101 /wd4113 /wd4133 /wd4190 /wd4244 /wd4267 /wd4305 /wd4477 /wd4996 /wd4819 /fp:fast")
  set(CMAKE_CXX_FLAGS "${ADDITIONAL_CXX_FLAGS} ${CMAKE_CXX_FLAGS}")
  string(REGEX REPLACE "/O2" "/Ox" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
else ()
  set(ADDITIONAL_CXX_FLAGS "-Wall -Wno-unused-result -Wno-unknown-pragmas -Wfatal-errors -Wno-deprecated-declarations -Wno-write-strings")
  set(ADDITIONAL_C_FLAGS "-Wall -Wno-unused-result -Wno-unknown-pragmas -Wfatal-errors -Wno-deprecated-declarations -Wno-write-strings")
  set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -DCMAKE_BUILD -DG_DISABLE_SINGLE_INCLUDES")
endif()


include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
if (COMPILER_SUPPORTS_MARCH_NATIVE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
endif()

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_LIST_DIR}" CACHE PATH "Install prefix" FORCE)
endif()


#################################################################
#                         SETTING DIRECTORIES                   #
#################################################################

set(SRC_DIR  ${CMAKE_CURRENT_LIST_DIR}/src      CACHE PATH "Path where find cpp files"                 )
set(INC_DIR  ${CMAKE_CURRENT_LIST_DIR}/include  CACHE PATH "Path where find header files"              )
set(HPP_DIR  ${CMAKE_CURRENT_LIST_DIR}/hpp      CACHE PATH "Path where find hpp files"                 )
set(EXAMPLE  ${CMAKE_CURRENT_LIST_DIR}/example  CACHE PATH "Path where find examples files"                   )

set (INSTALL_BIN_DIR  ${CMAKE_CURRENT_LIST_DIR}/bin      CACHE PATH "Path where outputs will be installed" FORCE)
set (INSTALL_LIB_DIR  ${CMAKE_CURRENT_LIST_DIR}/lib      CACHE PATH "Path where lib will be installed"     FORCE)

set (INSTALL_INCLUDE_DIR  "include/parseargs"  CACHE PATH "Path where headers will be installed")
set (INSTALL_CMAKE_DIR    "share/parseargs"    CACHE PATH "Path where cmake configs will be installed")

set(CMAKE_DEBUG_POSTFIX d)

file(GLOB SRC    "${SRC_DIR}/*.cpp" )
file(GLOB HEADER "${INC_DIR}/*.h" )
file(GLOB HPP    "${HPP_DIR}/*.hpp" )


set (parseargslib parseargs)

include_directories(${INC_DIR})
include_directories(${HPP_DIR})

if (BUILD_DOCS)
  add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/docs/")
endif ()


configure_file(
  "${CMAKE_CURRENT_LIST_DIR}/parseargs.pc.in"
  "${CMAKE_CURRENT_LIST_DIR}/parseargs.pc"
  @ONLY)
message(STATUS "Pkg-config generated")

# Make relative paths absolute (needed later on)
foreach (p LIB BIN INCLUDE CMAKE)
  set (var INSTALL_${p}_DIR)
  if (NOT IS_ABSOLUTE "${${var}}")
    set (FULLPATH_${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

#################################################################
#                          SUMMARY                              #
#################################################################

message(STATUS ""                                                                    )
message(STATUS "================ ParseArgs configuration Summary ==============="    )
message(STATUS "   ParseArgs version: ${HISTOLOGY_VERSION}"                          )
message(STATUS ""                                                                    )
message(STATUS "   Build type : ${CMAKE_BUILD_TYPE}"                                 )
message(STATUS "   C++ :"                                                            )
message(STATUS "      C++ Compiler : ${CMAKE_CXX_COMPILER}"                          )
message(STATUS "      C++ flags    :"                                                )
foreach(FLAG ${CMAKE_CXX_FLAGS_LIST})
  message(STATUS "                    * ${FLAG}"                                     )
endforeach(FLAG)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  message(STATUS "      C++ Debug flags:"                                            )
  foreach(FLAG ${CMAKE_CXX_FLAGS_DEBUG})
    message(STATUS "                    * ${FLAG}"                                   )
  endforeach(FLAG)
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
  message(STATUS "      C++ Release flags  :"                                        )
  foreach(FLAG ${CMAKE_CXX_FLAGS_RELEASE})
    message(STATUS "                    * ${FLAG}"                                   )
  endforeach(FLAG)
endif()
message(STATUS ""                                                                    )
message(STATUS "   Documentation support : ${BUILD_DOCS}"                            )
message(STATUS ""                                                                    )


#################################################################
#                         MAIN RULES                            #
#################################################################


add_library(${parseargslib} ${SRC} ${HEADER} ${HPP})
set_target_properties(${parseargslib} PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_include_directories(${parseargslib} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include> $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/hpp> $<INSTALL_INTERFACE:${INSTALL_INCLUDE_DIR}>)

set_target_properties(${parseargslib} PROPERTIES PUBLIC_HEADER "${CMAKE_CURRENT_LIST_DIR}/include/parse_args.h" "${CMAKE_CURRENT_LIST_DIR}/hpp/parse_args.hpp" "${CMAKE_CURRENT_LIST_DIR}/hpp/parse_args_old.hpp")
set_target_properties(${parseargslib} PROPERTIES CXX_VISIBILITY_PRESET hidden)

install(TARGETS ${parseargslib} EXPORT ParseArgsTargets
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
  LIBRARY DESTINATION "${INSTALL_LIB_DIR}"
  ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
  PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}"
  COMPONENT dev
)

# runtest
add_executable(runtest "${EXAMPLE}/runtest.cpp")
target_link_libraries(runtest ${parseargslib})
install(TARGETS runtest DESTINATION "${OUT_DIR}")


#################################################################
#                          EXPORT LIBRARY                       #
#################################################################


install(EXPORT ParseArgsTargets
  FILE ParseArgsTargets.cmake
  NAMESPACE ParseArgs::
  DESTINATION "${INSTALL_CMAKE_DIR}"
)

# Export the package for use from the build-tree (this registers the build-tree with a global CMake-registry)
export(PACKAGE ParseArgs)

# Create the ParseArgsConfig.cmake
# First of all we compute the relative path between the cmake config file and the include path
file(RELATIVE_PATH REL_INCLUDE_DIR "${FULLPATH_INSTALL_CMAKE_DIR}" "${FULLPATH_INSTALL_INCLUDE_DIR}")
set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")
configure_file(ParseArgsConfig.cmake.in "${PROJECT_BINARY_DIR}/ParseArgsConfig.cmake" @ONLY)
set(CONF_INCLUDE_DIRS "\${ParseArgs_CMAKE_DIR}/${REL_INCLUDE_DIR}")
configure_file(ParseArgsConfig.cmake.in "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/ParseArgsConfig.cmake" @ONLY)

# Create the ParseArgsConfigVersion.cmake
include(CMakePackageConfigHelpers)
write_basic_package_version_file("${PROJECT_BINARY_DIR}/ParseArgsConfigVersion.cmake"
  COMPATIBILITY SameMajorVersion
)

install(FILES
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/ParseArgsConfig.cmake"
  "${PROJECT_BINARY_DIR}/ParseArgsConfigVersion.cmake"
  DESTINATION "${INSTALL_CMAKE_DIR}"
)