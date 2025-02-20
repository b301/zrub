# tests/types/vector/CMakeLists.txt
get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
add_executable(${TARGET_NAME}_test main.c)
target_link_libraries(${TARGET_NAME}_test zrub)

# Define the output location for the tests
set_target_properties(${TARGET_NAME}_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TEST_BIN_DIR}/types/vector)
