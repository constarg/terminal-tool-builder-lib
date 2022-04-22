cmake_minimum_required(VERSION 3.21)
project(terminal-tool-builder C)

set(CMAKE_C_STANDARD 17)

set(HEADER_FILES include/tool_builder.h src/ds/queue.h)
set(SOURCE_FILES src/tool_builder.c src/ds/queue.c)
set(TO_INCLUDE include/ src/ds)


ADD_LIBRARY(	
		toolbuilder STATIC
		${HEADER_FILES}
		${SOURCE_FILES}
	   )

include_directories(${TO_INCLUDE})

