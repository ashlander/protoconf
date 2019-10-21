cmake_minimum_required(VERSION 2.6)
project (Protoconf)

# global include
include_directories("${PROJECT_INCLUDE_DIR}")

# local includes
set (PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(PROTOCONF_SRCS
    ${PROJECT_SOURCE_DIR}/module.cpp
)

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_INCLUDE_DIR}")

# tags generation
if (WITH_TAGS)
    set_source_files_properties( ${PROJECT_TAG_FILE} PROPERTIES GENERATED true)
    add_custom_command ( OUTPUT ${PROJECT_TAG_FILE}
        COMMAND ${PROJECT_TAG_COMMAND}
        DEPENDS ${PROTOCONF_SRCS}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} )
endif()

add_library(${PROJECT_NAME} SHARED ${PROJECT_TAG_FILE} ${PROTOCONF_SRCS})
set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
add_library(${PROJECT_NAME}Static STATIC ${PROJECT_TAG_FILE} ${PROTOCONF_SRCS})
set_target_properties(${PROJECT_NAME}Static PROPERTIES OUTPUT_NAME ${PROJECT_NAME}) # static library now will have the same name as dynamic
