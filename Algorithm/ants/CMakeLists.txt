CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(ants)

include_directories(./)
add_subdirectory(anttest)

FILE(GLOB SOURCE_FILES *.cpp ../decentralized/virtualLinkRouter.cpp)
add_library(${PROJECT_NAME} ${SOURCE_FILES})
