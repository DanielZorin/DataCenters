CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(router)

INCLUDE_DIRECTORIES(
      ../common 
      ../decentralized 
      ../misc
      )

ADD_EXECUTABLE(${PROJECT_NAME} router.cpp)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${LIBRARY_SET})
