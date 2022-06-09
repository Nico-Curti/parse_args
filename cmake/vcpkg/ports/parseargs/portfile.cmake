vcpkg_from_github(
  OUT_SOURCE_PATH SOURCE_PATH
  REPO Nico-Curti/parse_args
  HEAD_REF master
  REF ...
  SHA512 ...
)

vcpkg_cmake_configure(
  SOURCE_PATH "${SOURCE_PATH}"
  OPTIONS
    -DINSTALL_BIN_DIR:STRING=bin
    -DINSTALL_LIB_DIR:STRING=lib
    -DCMAKE_VERBOSE_MAKEFILE:BOOL=OFF
    -DBUILD_DOCS:BOOL=OFF
)

vcpkg_cmake_install()

if(VCPKG_LIBRARY_LINKAGE STREQUAL static)
  file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/bin" "${CURRENT_PACKAGES_DIR}/debug/bin")
endif()

vcpkg_cmake_config_fixup()

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/share")
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")

vcpkg_fixup_pkgconfig()
file(INSTALL "${SOURCE_PATH}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)
