include_directories(${OPENSSL_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/external/asio/asio/include)
include_directories(${CMAKE_SOURCE_DIR}/external/json/single_include)
include_directories(${CMAKE_SOURCE_DIR}/external/Hash/src)

set(test_FILES
        Client.cpp Client.h
        ../../server/ConnectionObject.cpp ../../server/ConnectionObject.h
        ConsoleLog.h
        main.cpp)

add_executable(testClient
        ${test_FILES})

target_link_libraries(testClient
        Threads::Threads
		${OPENSSL_LIBRARIES})