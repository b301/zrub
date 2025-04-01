cmake_minimum_required(VERSION 3.10)

# Set project name and version
project(Zrub VERSION 1.0)

# Set GNU standard installation directories
include(GNUInstallDirs)

# Include submodules
include(ExternalProject)

# Set libsodium installation prefix to match main project
set(LIBSODIUM_PREFIX ${CMAKE_INSTALL_PREFIX})

# Add libsodium as an external project
ExternalProject_Add(
    libsodium
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/dep/libsodium
    CONFIGURE_COMMAND ${CMAKE_SOURCE_DIR}/dep/libsodium/autogen.sh COMMAND
                     ${CMAKE_SOURCE_DIR}/dep/libsodium/configure --prefix=${LIBSODIUM_PREFIX}
    BUILD_COMMAND make
    INSTALL_COMMAND make install
    BUILD_IN_SOURCE TRUE
    INSTALL_DIR ${LIBSODIUM_PREFIX}
)

# Set compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wextra -pedantic -O3 -s -DZRUBLIB -DZRUBLIB_DEBUG -DZRUBLIB_DEBUG_PERF")

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/dep/uuid4/src/
    ${LIBSODIUM_PREFIX}/include
)

# Add source files for the library
set(SOURCE_FILES
    src/zrub/core/time.c
    src/zrub/core/logger.c
    src/zrub/core/bytes.c
    src/zrub/io/file.c
    src/zrub/utils/str.c
    src/zrub/auth/oauth2.c
    src/zrub/serializer/serializer.c
    src/zrub/serializer/int32.c
    src/zrub/serializer/uint32.c
    src/zrub/serializer/float.c
    src/zrub/serializer/string.c
    src/zrub/serializer/bytes.c
    src/zrub/net/epacket/epacket.c
    ${CMAKE_SOURCE_DIR}/dep/uuid4/src/uuid4.c
)

# Create the final static library directly from source files
add_library(zrub STATIC ${SOURCE_FILES})

# Link against libsodium
add_dependencies(zrub libsodium)
target_link_directories(zrub PRIVATE ${LIBSODIUM_PREFIX}/lib)
target_link_libraries(zrub PRIVATE sodium)

# Install the library
install(TARGETS zrub
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# Install the include directory recursively
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/
        DESTINATION include
        FILES_MATCHING PATTERN "*.h")

# Create pkg-config file
configure_file(
    "${CMAKE_SOURCE_DIR}/zrub.pc.in"
    "${CMAKE_BINARY_DIR}/zrub.pc"
    @ONLY
)

# Install the pkg-config file
install(
    FILES "${CMAKE_BINARY_DIR}/zrub.pc"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
)
