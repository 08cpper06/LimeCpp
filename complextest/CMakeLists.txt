cmake_minimum_required(VERSION 3.8)


project(complextest)

if (MSVC)
	set(CMAKE_CXX_FLAGS "/std:c++20 /EHsc")
else()
	set(CMAKE_CXX_FLAGS "-std=c++20 -Wall")
endif()

add_executable(${PROJECT_NAME}
	src/Main.cpp
	src/ComplexTestFramework.cpp
	src/ComplexTestFileManager.cpp
	src/TokenizeTest.cpp
	src/ParseTest.cpp
	../src/Tokenize/Tokenizer.cpp
	../src/Parse/Parser.cpp
	../src/Parse/ASTNodeLog.cpp
	../src/Parse/VariableTypeTable.cpp
	../src/Parse/VariableTable.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ./include)
target_include_directories(${PROJECT_NAME} PUBLIC ../include)