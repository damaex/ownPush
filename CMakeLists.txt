cmake_minimum_required(VERSION 3.2)
project(ownPush)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DASIO_STANDALONE)
add_definitions(-DUSE_STANDALONE_ASIO)

IF (WIN32)
  add_definitions(-D_WIN32_WINNT=0x0501)
ENDIF()

add_subdirectory(server)
add_subdirectory(clients/testClient)