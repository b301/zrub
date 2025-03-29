cmake_minimum_required(VERSION 3.10)

# Set project name and version
project(Zrub VERSION 1.0)

# Set compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wextra -pedantic -O3 -s -DZRUBLIB -DZRUBLIB_DEBUG")

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add source files for the library
set(SOURCE_FILES
    src/zrub/core/time.c
    src/zrub/core/logger.c
    src/zrub/core/bytes.c
    src/zrub/io/file.c
    src/zrub/utils/str.c
    src/zrub/auth/oauth2.c
    src/zrub/serializer/int32.c
    src/zrub/serializer/uint32.c
    src/zrub/serializer/float.c
    src/zrub/serializer/string.c
    src/zrub/serializer/bytes.c

    src/3p/uuid/uuid4.c
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
