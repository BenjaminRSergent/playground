cmake_minimum_required(VERSION 3.2)
project(mutex_test)

set(TARG_NAME mutex_test)
 
set(CMAKE_CXX_FLAGS "-std=c++14" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS "-std=gnu99" CACHE STRING "" FORCE)
 
#==========# SOURCE FILES #==========#


file(GLOB_RECURSE C_SRC_FILES src/*.c)
file(GLOB_RECURSE CPP_SRC_FILES src/*.cpp)

include_directories(include)

set(SRC_FILES ${C_SRC_FILES} ${CPP_SRC_FILES})

#==========# ADDING TARGET #==========#
add_executable(${TARG_NAME} ${SRC_FILES})
TARGET_LINK_LIBRARIES(${TARG_NAME} pthread)
