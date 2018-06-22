cmake_minimum_required(VERSION 3.8)
project(OS_EX5)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES SimpleTest.cpp VirtualMemory.cpp VirtualMemory.h
        MemoryConstants.h
        PhysicalMemory.h PhysicalMemory.cpp)
add_executable(OS_EX5 ${SOURCE_FILES})