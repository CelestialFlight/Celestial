cmake_minimum_required(VERSION 2.6)
project (imu)

include_directories(
	../../modules
)

file(GLOB filtersSRC 
	"../../modules/filters/IMU/*.c")

add_executable(imu 
	imuInputOutput.c 
	csvParser/csvparser.c 
	${filtersSRC})

target_link_libraries(imu m)