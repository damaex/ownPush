find_package(Threads REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/external/asio/asio/include)
include_directories(${CMAKE_SOURCE_DIR}/external/Simple-Web-Server)
include_directories(${CMAKE_SOURCE_DIR}/external/json/single_include)
include_directories(${CMAKE_SOURCE_DIR}/external/inih)

set(server_FILES
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
        ${server_FILES})

target_link_libraries(server
        Threads::Threads)
