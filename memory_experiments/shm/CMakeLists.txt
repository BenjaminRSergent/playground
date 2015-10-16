cmake_minimum_required(VERSION 3.2)
project(shm_test)

set(TARG_NAME shm_test)
 
set(CMAKE_CXX_FLAGS "-std=c++14" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS "-std=gnu99" CACHE STRING "" FORCE)
 
#==========# SOURCE FILES #==========#


file(GLOB_RECURSE C_SRC_FILES src/*.c)
file(GLOB_RECURSE CPP_SRC_FILES src/*.cpp)

file(GLOB_RECURSE H_HEADER_FILES include/*.h)
file(GLOB_RECURSE HPP_HEADER_FILES include/*.hpp)

set(PROJECT_FILES ${C_SRC_FILES} ${CPP_SRC_FILES} ${H_HEADER_FILES} ${HPP_HEADER_FILES})

#==========# ADDING TARGET #==========#
add_executable(${TARG_NAME} ${PROJECT_FILES})

#==========# Libraries #==========#
TARGET_LINK_LIBRARIES(${TARG_NAME} rt)
