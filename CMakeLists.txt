cmake_minimum_required(VERSION 3.10)

# Set project name and version
project(Zrub VERSION 1.0)

# Set compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic -O3 -s")

# Include directories
include_directories(${CMAKE_SOURCE_DIR})

# Create the build directories
set(BUILD_DIR ${CMAKE_BINARY_DIR}/build)
set(TESTS_DIR ${BUILD_DIR}/tests)
set(TEST_BIN_DIR ${BUILD_DIR}/bin/tests)

# Create directories for the library build
file(MAKE_DIRECTORY ${BUILD_DIR}/types ${BUILD_DIR}/os ${BUILD_DIR}/core ${BUILD_DIR}/cipher)

# Add source files for the library
set(SOURCE_FILES
    zrub/types/string.c
    zrub/types/linked_list.c
    zrub/types/hashmap.c
    zrub/types/vector.c
    zrub/types/queue.c
    zrub/types/iterator.c
    zrub/os/time.c
    zrub/os/file.c
    zrub/core/logger.c
    zrub/cipher/classic.c
)

# Compile the sources into object files
foreach(SRC ${SOURCE_FILES})
    get_filename_component(FILE_NAME ${SRC} NAME_WE)
    add_library(${FILE_NAME} OBJECT ${SRC})
endforeach()

# Create the final static library from object files
add_library(zrub STATIC $<TARGET_OBJECTS:string> $<TARGET_OBJECTS:linked_list> $<TARGET_OBJECTS:hashmap> 
    $<TARGET_OBJECTS:vector> $<TARGET_OBJECTS:queue> $<TARGET_OBJECTS:iterator> 
    $<TARGET_OBJECTS:time> $<TARGET_OBJECTS:file> $<TARGET_OBJECTS:logger> 
    $<TARGET_OBJECTS:classic>)

# Test directories
file(MAKE_DIRECTORY ${TESTS_DIR}/os/file ${TESTS_DIR}/os/time)
file(MAKE_DIRECTORY ${TESTS_DIR}/types/hashmap ${TESTS_DIR}/types/iterator 
    ${TESTS_DIR}/types/linked_list ${TESTS_DIR}/types/queue 
    ${TESTS_DIR}/types/string ${TESTS_DIR}/types/vector)

# Add the tests
add_subdirectory(tests/os/file)
add_subdirectory(tests/os/time)
add_subdirectory(tests/types/hashmap)
add_subdirectory(tests/types/iterator)
add_subdirectory(tests/types/linked_list)
add_subdirectory(tests/types/queue)
add_subdirectory(tests/types/string)
add_subdirectory(tests/types/vector)

# Install the library
install(TARGETS zrub DESTINATION lib)
install(FILES zrub/zrub.h DESTINATION include)
