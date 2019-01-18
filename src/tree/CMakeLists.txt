# Set output library.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/test/tree)

# Build static library
add_library(tree STATIC dtree.cc)

# Build unittests.
set(LIBS base tree gtest)

add_executable(dtree_test dtree_test.cc)
target_link_libraries(dtree_test gtest_main ${LIBS})

# Install library and header files
install(TARGETS tree DESTINATION lib/tree)
FILE(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
install(FILES ${HEADER_FILES} DESTINATION include/tree)