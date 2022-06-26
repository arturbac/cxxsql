#header-only library
set(ninja_can_be_used ON) 

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO boost-ext/ut
    REF v1.1.9
    SHA512 81a6b80948d3a203534244f62f5f3ac57593083cc0c32484498a7d01d29455f7dcb33e2ec0587609b8dff33a81a5551796d7681d48fd93e817d6d0c31697234e
    HEAD_REF master
)

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        -DBOOST_UT_BUILD_BENCHMARKS:BOOL=OFF
        -DBOOST_UT_BUILD_EXAMPLES:BOOL=OFF
        -DBOOST_UT_BUILD_TESTS:BOOL=OFF
        -DCMAKE_INSTALL_DIR:STRING=cmake
)
vcpkg_cmake_install()

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")
#file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/lib" "${CURRENT_PACKAGES_DIR}/debug/lib")

#vcpkg_cmake_config_fixup(CONFIG_PATH cmake)
#vcpkg_cmake_config_fixup()
vcpkg_cmake_config_fixup(CONFIG_PATH lib/cmake/ut-1.1.8)

#The /lib/cmake folder should be merged with /debug/lib/cmake and moved to /share/boost-ext-ut/cmake.
#Please use the helper function `vcpkg_cmake_config_fixup()` from the port vcpkg-cmake-config.`

#file(INSTALL "${SOURCE_PATH}/COPYING" DESTINATION "${CURRENT_PACKAGES_DIR}/boost-ext/ut" RENAME copyright)
