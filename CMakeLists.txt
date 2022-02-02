cmake_minimum_required(VERSION 3.10)
project(mbprint)

include(FindPkgConfig)

#set(CMAKE_BUILD_TYPE Release)

set (CMAKE_CXX_STANDARD 14)
set (CMAKE_INTERPROCEDURAL_OPTIMIZATION true)

add_compile_options(-Wall -Wextra)

pkg_search_module(MODBUS REQUIRED libmodbus)

# add the executable
add_executable(mbprint 
	mbprint.cpp
	connect.cpp
	poll.cpp
	config.cpp
)

#target_link_libraries(mbprint -s -static-libgcc -static-libstdc++ -static)
target_link_libraries(mbprint optimized -s)

target_include_directories(mbprint SYSTEM PUBLIC ${MODBUS_INCLUDE_DIRS})
target_link_libraries(mbprint ${MODBUS_LIBRARIES})
