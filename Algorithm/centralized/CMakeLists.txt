CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(centralized)

include_directories(../)
include_directories(../common)

FILE(GLOB SOURCE_FILES *.cpp)
add_library(${PROJECT_NAME} ${SOURCE_FILES})
