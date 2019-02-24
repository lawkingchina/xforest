# Set output library.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/test/network)

# Build static library
add_library(network STATIC tcp_socket.cc msg_queue.cc)

# Build unittests.
set(LIBS base network gtest)

add_executable(tcp_socket_test tcp_socket_test.cc)
target_link_libraries(tcp_socket_test gtest_main ${LIBS})

add_executable(msg_queue_test msg_queue_test.cc)
target_link_libraries(msg_queue_test gtest_main ${LIBS})

#add_executable(socket_communicator_master_test socket_communicator_master_test.cc)
#target_link_libraries(socket_communicator_master_test gtest_main ${LIBS})

#add_executable(socket_communicator_worker_test socket_communicator_worker_test.cc)
#target_link_libraries(socket_communicator_worker_test gtest_main ${LIBS})

FILE(COPY "${CMAKE_CURRENT_SOURCE_DIR}/communicator_test.sh" 
DESTINATION ${PROJECT_BINARY_DIR}/test/network)

# Install library and header files
install(TARGETS network DESTINATION lib/network)
FILE(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
install(FILES ${HEADER_FILES} DESTINATION include/network)