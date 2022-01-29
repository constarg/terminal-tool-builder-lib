cmake_minimum_required(VERSION 3.21)
project(Test-command-tool-builder C)

set(CMAKE_C_STANDARD 17)



add_executable(builder-test main.c)

target_link_libraries(builder-test $ENV{HOME}/Documents/GitHub/terminal-tool-builder-lib/build/libtoolbuilder.a)

include_directories(../include/)

