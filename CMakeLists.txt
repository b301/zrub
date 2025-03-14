cmake_minimum_required(VERSION 3.10)

# Set project name and version
project(Zrub VERSION 1.0)

# Set compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wextra -pedantic -O3 -s -DZRUBLIB -DZRUBLIB_DEBUG")

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add source files for the library
set(SOURCE_FILES
    zrub/core/time.c
    zrub/core/logger.c
    zrub/core/bytes.c
    zrub/io/file.c
    zrub/utils/str.c
    zrub/auth/oauth2.c
    zrub/cfg/kv.c
    zrub/3p/uuid/uuid4.c
)

# Create the final static library directly from source files
add_library(zrub STATIC ${SOURCE_FILES})

# Install the library
install(TARGETS zrub
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)

# Install the include directory recursively
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/
        DESTINATION include
        FILES_MATCHING PATTERN "*.h")
