cmake_minimum_required(VERSION 3.2)
project(shm_test)

set(TARG_NAME shm_test)
 
set(CMAKE_CXX_FLAGS "-std=c++14" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS "-std=gnu99" CACHE STRING "" FORCE)
 
#==========# SOURCE FILES #==========#
file(GLOB_RECURSE H_HEADER_FILES include/*.h)
file(GLOB_RECURSE HPP_HEADER_FILES include/*.hpp)

include_directories(include)

#==========# ADDING TARGET #==========#
add_executable(reader src/reader.cpp)
add_executable(writer src/writer.cpp)

#==========# Libraries #==========#
TARGET_LINK_LIBRARIES(reader rt)
TARGET_LINK_LIBRARIES(writer rt)
