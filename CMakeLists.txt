cmake_minimum_required(VERSION 3.21)
project(terminal-tool-builder C)

set(CMAKE_C_STANDARD 17)



ADD_LIBRARY(	
		toolbuilder STATIC
		lib/tool_builder.c
		include/tool_builder.h
		tools/parser.c
		tools/include/parser.h		
	   )

include_directories(
        	include/
		misc/include/
		tools/include/
)

