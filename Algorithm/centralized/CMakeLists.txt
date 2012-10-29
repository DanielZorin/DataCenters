CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(centralized)

include_directories(./)
add_subdirectory(test)

FILE(GLOB SOURCE_FILES *.cpp)
add_library(${PROJECT_NAME} ${SOURCE_FILES})
