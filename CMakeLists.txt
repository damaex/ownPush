cmake_minimum_required(VERSION 3.2)
project(ownPush)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DASIO_STANDALONE)
add_definitions(-DUSE_STANDALONE_ASIO)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)

IF (${OPENSSL_FOUND})
	message(STATUS "OpenSSL_VERSION: ${OPENSSL_VERSION}")
ENDIF (${OPENSSL_FOUND})

if(WIN32)
	set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
	
    # Visual leak detector
    find_package(VLD)
	add_definitions(-D_WIN32_WINNT=0x0600)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

add_subdirectory(server)
add_subdirectory(clients/testClient)