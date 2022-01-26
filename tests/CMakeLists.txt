cmake_minimum_required(VERSION 3.21)
project(Test-command-tool-builder C)

set(CMAKE_C_STANDARD 17)


add_executable(builder-test main.c)


TARGET_LINK_LIBRARIES(builder-test -L ../../build/toolbuilder)


include_directories(
	../include/
)
