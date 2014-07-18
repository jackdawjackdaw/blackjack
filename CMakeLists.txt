cmake_minimum_required(VERSION 2.8)
project (blackjack)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -Wall -std=c99 -m64")


## don't need this yet, may want it for rng
find_package(GSL REQUIRED)
message(STATUS "GSL_INCLUDE_DIRS: ${GSL_INCLUDE_DIR}")
message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
message(STATUS "CMAKE_INSTALL_PATH: ${CMAKE_INSTALL_PREFIX}")

INCLUDE_DIRECTORIES(${GSL_INCLUDE_DIR})
MESSAGE(STATUS "include dirs: ${INCLUDE_DIRECTORIES}")
SET(libs ${libs} ${GSL_LIBRARIES} ${GSLCBLAS_LIBRARIES})
MESSAGE(STATUS "libs: ${libs}")

add_subdirectory(src)

if(BUILD_TESTING)
enable_testing()
add_subdirectory(tests)
endif(BUILD_TESTING)