CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(ants)
FILE(GLOB SOURCE_FILES *.cpp ../decentralized/virtualLinkRouter.cpp ../decentralized/criteria.cpp)
add_library(${PROJECT_NAME} ${SOURCE_FILES})
