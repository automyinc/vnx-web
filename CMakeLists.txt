cmake_minimum_required(VERSION 2.6)
project(vnx-web)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

file(GLOB GENERATED_SOURCE_FILES generated/src/*.cpp)

add_library(vnx_web SHARED
	${GENERATED_SOURCE_FILES}
	src/Path.cpp
	src/Cache.cpp
)

target_include_directories(vnx_web
	PUBLIC include generated/include
)

target_link_libraries(vnx_web
	vnx_base
)
