# Ensure libcmoka.so (and similar files) are saved to /usr/lib

include_directories(
    ${CMAKE_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ../modules 
    /usr/local/shared
)

add_executable(serialBufferTest 
    common/serialBufferTest.c 
    ../modules/common/serialBuffer.c)

target_link_libraries(serialBufferTest cmocka)

add_test(serialBufferTest ${CMAKE_CURRENT_BINARY_DIR}/simple_test)
