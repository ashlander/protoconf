cmake_minimum_required(VERSION 2.6)
project (Schema)

# local includes
set (PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(SCHEMA_SRCS
    ${PROJECT_SOURCE_DIR}/schema.pb.cc
)

set(SCHEMA_PROTO
    ${PROJECT_SOURCE_DIR}/schema.proto
)

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_INCLUDE_DIR}")

# protobuf
set_source_files_properties( ${SCHEMA_SRCS} PROPERTIES GENERATED true)
add_custom_command (    OUTPUT              ${SCHEMA_SRCS}
                        COMMAND             ${PROJECT_PROTOBUF_COMMAND}
                        ARGS                -I${PROJECT_SOURCE_DIR} --cpp_out=${PROJECT_SOURCE_DIR} ${SCHEMA_PROTO}
                        DEPENDS             ${SCHEMA_PROTO}
                        WORKING_DIRECTORY   ${CMAKE_SOURCE_DIR} )

# tags generation
if (WITH_TAGS)
    set_source_files_properties( ${PROJECT_TAG_FILE} PROPERTIES GENERATED true)
    add_custom_command ( OUTPUT ${PROJECT_TAG_FILE}
        COMMAND ${PROJECT_TAG_COMMAND}
        DEPENDS ${NETWORK_SRCS}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} )
endif()

add_library(${PROJECT_NAME} STATIC ${PROJECT_TAG_FILE} ${SCHEMA_SRCS})
target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBS})




