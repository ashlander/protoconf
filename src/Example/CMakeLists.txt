cmake_minimum_required(VERSION 2.6)
project (example)

# global include
include_directories("${PROJECT_INCLUDEX_DIR}") # end user include

# local includes
set (PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(TESTS_SRCS
    ${PROJECT_SOURCE_DIR}/ProtoconfTests.cpp
    ${PROJECT_SOURCE_DIR}/Tests.cpp
)

include_directories("${PROJECT_INCLUDE_DIR}")

# tags generation
if (WITH_TAGS)
    set_source_files_properties( ${PROJECT_TAG_FILE} PROPERTIES GENERATED true)
    add_custom_command (
        OUTPUT ${PROJECT_TAG_FILE}
        COMMAND ${PROJECT_TAG_COMMAND}
        DEPENDS ${TESTS_SRCS}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} )
endif()

# with conan
if (WITH_CONAN)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set (EXTRA_LIBS ${EXTRA_LIBS} ${CONAN_LIB_DIRS_GTEST}/libgtestd.a)
    else()
        set (EXTRA_LIBS ${EXTRA_LIBS} ${CONAN_LIB_DIRS_GTEST}/libgtest.a)
    endif()
    set (EXTRA_LIBS ${EXTRA_LIBS} ${CMAKE_BINARY_DIR}/lib/libSchema.a)
    set (EXTRA_LIBS ${EXTRA_LIBS} ${CMAKE_BINARY_DIR}/lib/libProtoconf.a)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
       set (EXTRA_LIBS ${EXTRA_LIBS} ${CONAN_LIB_DIRS_PROTOBUF}/libprotobufd.a)
    else()
       set (EXTRA_LIBS ${EXTRA_LIBS} ${CONAN_LIB_DIRS_PROTOBUF}/libprotobuf.a)
    endif()
    set (EXTRA_LIBS ${EXTRA_LIBS} pthread)
else()
    set (EXTRA_LIBS ${EXTRA_LIBS} pthread)
    find_library (LIBGTEST_PATH NAMES libgtest.a)
    message(STATUS "PATH to libgtest is ${LIBGTEST_PATH}")
    set (EXTRA_LIBS ${EXTRA_LIBS} ${LIBGTEST_PATH})
    set (EXTRA_LIBS ${EXTRA_LIBS} ${LIBRARY_OUTPUT_PATH}/libSchema.a)
    find_library (LIBPROTOBUF_PATH NAMES libprotobuf.a)
    message(STATUS "PATH to libprotobuf is ${LIBPROTOBUF_PATH}")
    set (EXTRA_LIBS ${EXTRA_LIBS} ${LIBPROTOBUF_PATH})
    set (EXTRA_LIBS ${EXTRA_LIBS} ${LIBRARY_OUTPUT_PATH}/libProtoconf.a)
endif()

add_executable(${PROJECT_NAME} ${PROJECT_TAG_FILE} ${TESTS_SRCS})
target_link_libraries(${PROJECT_NAME} Protoconf ProtoconfStatic Schema ${EXTRA_LIBS})
