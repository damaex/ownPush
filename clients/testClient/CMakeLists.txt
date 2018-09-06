find_package(Threads REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/external/asio/asio/include)

set(test_FILES
        main.cpp Client.cpp Client.h)

add_executable(testClient
        ${test_FILES})

target_link_libraries(testClient
        Threads::Threads)