cmake_minimum_required(VERSION 3.8)


project(limecpp)

set(CMAKE_CXX_FLAGS "-std=c++20 -Wall")

add_executable(${PROJECT_NAME}
	src/main.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ./include)

# add_subdirectory(unittest)
