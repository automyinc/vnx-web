cmake_minimum_required(VERSION 2.6)
project(vnx-web)

find_package(Boost 1.55.0 REQUIRED COMPONENTS filesystem)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

# Always use multi-threaded Boost libraries.
set(Boost_USE_MULTI_THREADED ON)

file(GLOB GENERATED_SOURCE_FILES generated/src/*.cpp)

add_library(vnx_web SHARED
	${GENERATED_SOURCE_FILES}
	src/Path.cpp
	src/Cache.cpp
	src/FileSystem.cpp
	src/ValueFile.cpp
	src/ObjectFile.cpp
	src/ErrorCode.cpp
	src/Content.cpp
	src/File.cpp
	src/TextFile.cpp
)

target_include_directories(vnx_web
	PUBLIC include generated/include
)

target_link_libraries(vnx_web
	vnx_base
	${Boost_LIBRARIES}
	magic
)

add_executable(filesystem_server src/filesystem_server.cpp)
target_link_libraries(filesystem_server vnx_web)

add_subdirectory(test)
