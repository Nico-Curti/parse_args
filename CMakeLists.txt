cmake_minimum_required (VERSION 3.12.0)

# histology Version
set (PARSE_ARGS_MAJOR    1)
set (PARSE_ARGS_MINOR    0)
set (PARSE_ARGS_PATCH    0)
set (PARSE_ARGS_VERSION ${PARSE_ARGS_MAJOR}.${PARSE_ARGS_MINOR}.${PARSE_ARGS_PATCH})

project (parse_args LANGUAGES CXX VERSION ${PARSE_ARGS_VERSION} DESCRIPTION "Encoding models")


if ( ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" AND ${CMAKE_CXX_COMPILER_VERSION} LESS 7) OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND ${CMAKE_CXX_COMPILER_VERSION} LESS 4) )

  set (CMAKE_CXX_STANDARD 14)
  add_definitions(-D__old_cpp_std__)
  message (STATUS "The provided compiler version does not support the c++17 standard. Switch to old gcc compatibility.")

else ()

  set (CMAKE_CXX_STANDARD 17)

endif()

enable_language(CXX)

#################################################################
#                         COMPILE OPTIONS                       #
#################################################################

option (PKGCONF     "Enable pkg-config generation        " ON  )
option (BUILD_DOCS  "Enable Documentaion builid   support" OFF )

#################################################################
#                         SETTING VARIABLES                     #
#################################################################

set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/" ${CMAKE_MODULE_PATH})

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
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
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
  #list (APPEND linked_libs stdc++fs)
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
  set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} /DCMAKE_BUILD /DG_DISABLE_SINGLE_INCLUDES")
  set(ADDITIONAL_CXX_FLAGS "/wd4013 /wd4018 /wd4028 /wd4047 /wd4068 /wd4090 /wd4101 /wd4113 /wd4133 /wd4190 /wd4244 /wd4267 /wd4305 /wd4477 /wd4996 /wd4819 /fp:fast")
  set(ADDITIONAL_C_FLAGS "/wd4013 /wd4018 /wd4028 /wd4047 /wd4068 /wd4090 /wd4101 /wd4113 /wd4133 /wd4190 /wd4244 /wd4267 /wd4305 /wd4477 /wd4996 /wd4819 /fp:fast")
  set(CMAKE_CXX_FLAGS "${ADDITIONAL_CXX_FLAGS} ${CMAKE_CXX_FLAGS}")
  set(CMAKE_C_FLAGS "${ADDITIONAL_C_FLAGS} ${CMAKE_C_FLAGS}")
  string(REGEX REPLACE "/O2" "/Ox" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
  string(REGEX REPLACE "/O2" "/Ox" CMAKE_C_FLAGS_RELEASE ${CMAKE_C_FLAGS_RELEASE})
else ()
  set(ADDITIONAL_CXX_FLAGS "-Wall -Wno-unused-result -Wno-unknown-pragmas -Wfatal-errors -Wno-deprecated-declarations -Wno-write-strings")
  set(ADDITIONAL_C_FLAGS "-Wall -Wno-unused-result -Wno-unknown-pragmas -Wfatal-errors -Wno-deprecated-declarations -Wno-write-strings")
  set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -DCMAKE_BUILD -DG_DISABLE_SINGLE_INCLUDES")
  set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -DCMAKE_BUILD -DG_DISABLE_SINGLE_INCLUDES")
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

set(PARSE_ARGS_INSTALL_INCLUDE_DIR ${INSTALL_INCLUDE_DIR})

set(SRC_DIR  ${CMAKE_SOURCE_DIR}/src                            CACHE PATH "Path where find cpp files"                 )
set(INC_DIR  ${CMAKE_SOURCE_DIR}/include                        CACHE PATH "Path where find header files"              )
set(HPP_DIR  ${CMAKE_SOURCE_DIR}/hpp                            CACHE PATH "Path where find hpp files"                 )
set(OUT_DIR  ${CMAKE_SOURCE_DIR}/bin                            CACHE PATH "Path where outputs will be installed" FORCE)
set(LIB_DIR  ${CMAKE_SOURCE_DIR}/lib                            CACHE PATH "Path where lib will be installed"     FORCE)
set(EXAMPLE  ${CMAKE_SOURCE_DIR}/example                        CACHE PATH "Path where find examples files"                   )
set(OUT_DIR  ${CMAKE_SOURCE_DIR}/bin                            CACHE PATH "Path where outputs will be installed"        FORCE)

set (INSTALL_INCLUDE_DIR  "${CMAKE_CURRENT_LIST_DIR}/share/include/parseargs"  CACHE PATH "Path where headers will be installed")
set (INSTALL_CMAKE_DIR    "${CMAKE_CURRENT_LIST_DIR}/share/parseargs"          CACHE PATH "Path where cmake configs will be installed")

set(CMAKE_DEBUG_POSTFIX d)

file(GLOB SRC           "${SRC_DIR}/*.cpp" )
file(GLOB HEADER        "${INC_DIR}/*.h" )
file(GLOB HPP           "${HPP_DIR}/*.hpp" )


set (parseargslib parseargs)

include_directories(${INC_DIR})
include_directories(${HPP_DIR})

if (BUILD_DOCS)
  add_subdirectory(${CMAKE_SOURCE_DIR}/docs/)
endif ()


if (PKGCONF)
  configure_file(${CMAKE_SOURCE_DIR}/parseargs.pc.in ${CMAKE_SOURCE_DIR}/parseargs.pc @ONLY)
  message(STATUS "Pkg-config generated")
endif ()

set(PARSE_ARGS_INSTALL_INCLUDE_DIR ${INSTALL_INCLUDE_DIR})

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
message(STATUS "      Linker flags : "                                               )
foreach(FLAG ${linked_libs})
  message(STATUS "                    * ${FLAG}"                                     )
endforeach(FLAG)
message(STATUS ""                                                                    )
message(STATUS "   Documentation support : ${BUILD_DOCS}"                            )
message(STATUS ""                                                                    )


#################################################################
#                         MAIN RULES                            #
#################################################################


add_library(${parseargslib} ${SRC} ${HEADER} ${HPP})
set_target_properties(${parseargslib} PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_include_directories(${parseargslib} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include> $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/hpp> $<INSTALL_INTERFACE:${PARSE_ARGS_INSTALL_INCLUDE_DIR}>)
target_link_libraries(${parseargslib} PUBLIC ${linked_libs})

set_target_properties(${parseargslib} PROPERTIES PUBLIC_HEADER "${HEADER}")
set_target_properties(${parseargslib} PROPERTIES CXX_VISIBILITY_PRESET hidden)

install(TARGETS ${parseargslib} EXPORT ParseArgsTargets
                                RUNTIME DESTINATION {OUT_DIR}
                                LIBRARY DESTINATION ${LIB_DIR}
                                ARCHIVE DESTINATION ${LIB_DIR}
                                PUBLIC_HEADER DESTINATION ${INSTALL_INCLUDE_DIR}
                                COMPONENT dev
                                )


# runtest
add_executable(runtest ${EXAMPLE}/runtest.cpp)
target_link_libraries(runtest ${linked_libs} ${parseargslib})
install(TARGETS runtest  DESTINATION ${OUT_DIR})


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
file(RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}" "${INSTALL_INCLUDE_DIR}")
set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")
configure_file(ParseArgsConfig.cmake.in "${PROJECT_BINARY_DIR}/ParseArgsConfig.cmake" @ONLY)
set(CONF_INCLUDE_DIRS "\${PARSE_ARGS_CMAKE_DIR}/${REL_INCLUDE_DIR}")
configure_file(ParseArgsConfig.cmake.in "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/ParseArgsConfig.cmake" @ONLY)

# Create the ParseArgsConfigVersion.cmake
include(CMakePackageConfigHelpers)
write_basic_package_version_file("${PROJECT_BINARY_DIR}/ParseArgsConfigVersion.cmake"
                                 COMPATIBILITY SameMajorVersion)

install(FILES
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/ParseArgsConfig.cmake"
  "${PROJECT_BINARY_DIR}/ParseArgsConfigVersion.cmake"
  DESTINATION "${INSTALL_CMAKE_DIR}"
)