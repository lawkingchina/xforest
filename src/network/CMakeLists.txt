# Set output library.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/test/network)

# Build static library
add_library(network STATIC tcp_socket.cc communicator.cc)

# Build unittests.
set(LIBS base network gtest)

add_executable(tcp_socket_test tcp_socket_test.cc)
target_link_libraries(tcp_socket_test gtest_main ${LIBS})

# Install library and header files
install(TARGETS network DESTINATION lib/network)
FILE(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
install(FILES ${HEADER_FILES} DESTINATION include/network)