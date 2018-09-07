cmake_minimum_required(VERSION 3.2)
project(ownPush)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DASIO_STANDALONE)
add_definitions(-DUSE_STANDALONE_ASIO)

add_subdirectory(server)
add_subdirectory(clients/testClient)