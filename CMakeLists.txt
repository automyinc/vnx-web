cmake_minimum_required(VERSION 2.6)
project(vnx-web)

find_package(Boost 1.55.0 REQUIRED COMPONENTS filesystem)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

file(GLOB GENERATED_SOURCE_FILES generated/src/*.cpp)
file(GLOB GENERATED_QUERY_SOURCE_FILES query/generated/src/*.cpp)

add_library(vnx_web SHARED
	${GENERATED_SOURCE_FILES}
	${GENERATED_QUERY_SOURCE_FILES}
	lib/http-parser/http_parser.c
	src/Path.cpp
	src/Cache.cpp
	src/FileSystem.cpp
	src/ErrorCode.cpp
	src/Content.cpp
	src/File.cpp
	src/Response.cpp
	src/HttpParser.cpp
	src/HttpProcessor.cpp
	src/BinaryData.cpp
	src/stream_event_t.cpp
	src/HistoryCache.cpp
	src/Frontend.cpp
	src/HttpRenderer.cpp
	src/View.cpp
	src/ViewProcessor.cpp
	src/Request.cpp
	src/DefaultView.cpp
	src/PageView.cpp
	src/DynamicFile.cpp
	src/DynamicView.cpp
	src/Util.cpp
	src/SetCookie.cpp
	src/Query.cpp
	src/Database.cpp
	src/DatabaseView.cpp
)

target_include_directories(vnx_web
	PUBLIC include generated/include query/generated/include lib/
)

target_link_libraries(vnx_web vnx_base ${Boost_LIBRARIES} magic)

add_executable(vnxwebserver src/vnxwebserver.cpp)
target_link_libraries(vnxwebserver vnx_web)

add_executable(vnxfilesystem src/vnxfilesystem.cpp)
target_link_libraries(vnxfilesystem vnx_web)

add_executable(vnxdatabase src/vnxdatabase.cpp)
target_link_libraries(vnxdatabase vnx_web)

add_executable(http_bench_tool tools/http_bench_tool.cpp)
target_link_libraries(http_bench_tool vnx_web)

install(DIRECTORY include/ DESTINATION include)
install(DIRECTORY generated/include/ DESTINATION include)
install(DIRECTORY query/generated/include/ DESTINATION include)
install(DIRECTORY query/interface/ DESTINATION interface/vnx/query)

install(TARGETS vnxwebserver DESTINATION bin)
install(TARGETS vnxfilesystem DESTINATION bin)
install(TARGETS vnxdatabase DESTINATION bin)

add_subdirectory(test)
