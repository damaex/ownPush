find_package(Threads REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/external/asio/asio/include)
include_directories(${CMAKE_SOURCE_DIR}/external/Simple-Web-Server)
include_directories(${CMAKE_SOURCE_DIR}/external/json/single_include)
include_directories(${CMAKE_SOURCE_DIR}/external/inih)

set(exception_FILES
        exceptions/ClientIDAlreadySetException.h
        exceptions/UnknownCommandException.h)

set(server_FILES
        ChallengeHandler.cpp ChallengeHandler.h
        Client.cpp Client.h
        ConnectionObject.h ConnectionObject.cpp
        IClient.h
        IHandler.h
        Log.h ILog.h
        PushServer.cpp PushServer.h
        RestServer.cpp RestServer.h
        Settings.cpp Settings.h
        main.cpp)

add_executable(server
        ${exception_FILES}
        ${server_FILES})

source_group("exceptions"            FILES ${exception_FILES})

target_link_libraries(server
        Threads::Threads)
