CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(routing)

FILE(GLOB SOURCE_FILES *.cpp)
ADD_LIBRARY(${PROJECT_NAME} SHARED ${SOURCE_FILES})
