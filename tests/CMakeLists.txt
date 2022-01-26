cmake_minimum_required(VERSION 3.19)
project(Test-command-tool-builder C)

set(CMAKE_C_STANDARD 99)


add_executable(builder-test main.c)


TARGET_LINK_LIBRARIES(builder-test ../build/libtoolbuilder)


include_directories(
	../include/
)
