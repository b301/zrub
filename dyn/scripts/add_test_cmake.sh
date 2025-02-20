#!/bin/bash

# Define the root directory of the tests
TESTS_DIR="tests"
BUILD_DIR="build"
TEST_BIN_DIR="${BUILD_DIR}/bin/tests"

# Create the necessary CMakeLists.txt files in the subdirectories
create_cmake_for_test() {
    local dir=$1
    # Remove the leading 'tests/' part from the directory to create a proper subdirectory structure
    local subdir="${dir#${TESTS_DIR}/}"
    cat <<EOL > "$dir/CMakeLists.txt"
# $dir/CMakeLists.txt
get_filename_component(TARGET_NAME \${CMAKE_CURRENT_SOURCE_DIR} NAME)
add_executable(\${TARGET_NAME}_test main.c)
target_link_libraries(\${TARGET_NAME}_test zrub)

# Define the output location for the tests
set_target_properties(\${TARGET_NAME}_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY \${TEST_BIN_DIR}/$subdir)
EOL
}

# Iterate through each directory in tests/ and create a CMakeLists.txt
for test_type in $(find "$TESTS_DIR" -type d); do
    if [ -f "$test_type/main.c" ]; then
        echo "Creating CMakeLists.txt for $test_type"
        create_cmake_for_test "$test_type"
    fi
done

echo "CMakeLists.txt files created for all test directories."
