cmake_minimum_required(VERSION 3.0)

# Ensure libcmoka.so (and similar files) are saved to /usr/lib

include_directories(
    ${CMAKE_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ../modules 
    /usr/local/shared
)

# All unit tests.
add_executable(unittests 
	unit/unittests.c
	unit/serialBufferTest.c 
    unit/motorMixerTest.c
    ../modules/common/serialBuffer.c
    ../modules/common/mixer.c)

target_link_libraries(unittests cmocka)

# Manual Single Pole LPF Test
add_executable(singlePoleLPFTest
	manual/filters/singlePoleLPFTest.c
	../modules/filters/recursive/singlePoleLPF.c)
target_link_libraries(singlePoleLPFTest cmocka m)

# Manual Biquad (2-pole) LPF Test
add_executable(biquadLPFTest
	manual/filters/biquadLPFTest.c
	../modules/filters/recursive/biquadLPF.c)
target_link_libraries(biquadLPFTest cmocka m)