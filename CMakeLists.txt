cmake_minimum_required(VERSION 3.10)

# Set project name and version
project(Zrub VERSION 1.0)

# Set compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wextra -pedantic -O3 -s -DZRUBLIB -DZRUBLIB_DEBUG")

# Include directories
include_directories(${CMAKE_SOURCE_DIR})

# Create the build directories
set(BUILD_DIR ${CMAKE_BINARY_DIR}/build)

# Add source files for the library
set(SOURCE_FILES
    zrub/core/time.c
    zrub/core/logger.c
    zrub/io/file.c
    zrub/utils/str.c
    zrub/utils/iterators.c
    zrub/cfg/kv.c
    zrub/3p/uuid/uuid4.c
)

# Compile the sources into object files
foreach(SRC ${SOURCE_FILES})
    get_filename_component(FILE_NAME ${SRC} NAME_WE)
    add_library(${FILE_NAME} OBJECT ${SRC})
endforeach()

# Create the final static library from object files
add_library(zrub STATIC $<TARGET_OBJECTS:time> 
                        $<TARGET_OBJECTS:logger> 
                        $<TARGET_OBJECTS:file> 
                        $<TARGET_OBJECTS:str> 
                        $<TARGET_OBJECTS:iterators>
                        $<TARGET_OBJECTS:kv> 
                        $<TARGET_OBJECTS:uuid4>)

# Install the library
install(TARGETS zrub DESTINATION lib)
install(FILES zrub/zrub.h DESTINATION include)
