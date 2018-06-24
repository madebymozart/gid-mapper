cmake_minimum_required(VERSION 3.8)
project(gid-mapper)

set(CMAKE_CXX_STANDARD 11)

include_directories("source")

set(SOURCE_FILES source/main.cpp
        source/xml/tinyxml2.cxx
        source/xml/tinyxml2.hxx
        source/json/json.hxx
        source/mapper/mapper.cxx
        source/mapper/mapper.hxx
        source/cmd/cmd.hxx)

add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -gdwarf-3")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -gdwarf-3")

add_executable(gid-mapper ${SOURCE_FILES})