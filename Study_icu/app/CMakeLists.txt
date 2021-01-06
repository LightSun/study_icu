cmake_minimum_required(VERSION 3.4.1)

SET(SRC_BASE ${CMAKE_SOURCE_DIR}/src/main/cpp)
SET(SRC_ICU_DIR ${CMAKE_SOURCE_DIR}/../../icu4c_android/icu4c/source)

include_directories(${SRC_ICU_DIR}/common)
set(LIB_DIR ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI})

add_library(icuuc SHARED IMPORTED)
set_target_properties(icuuc PROPERTIES IMPORTED_LOCATION ${LIB_DIR}/libicuuc51.so)
add_library(icudt SHARED IMPORTED)
set_target_properties(icudt PROPERTIES IMPORTED_LOCATION ${LIB_DIR}/libicudt51.so)

aux_source_directory(src/main/cpp/sample SRC_sample)
aux_source_directory(src/main/cpp/Unicode SRC_sample)

add_library(
        study-icu
        SHARED
        ${SRC_sample}
)

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

target_link_libraries(
        study-icu
        icuuc
        icudt
        ${log-lib}
)

