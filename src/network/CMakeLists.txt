# build service
set(SOURCE_FILES
    uv/ServerImpl.cpp
    uv/Worker.cpp

    blocking/ServerImpl.cpp

    nonblocking/ServerImpl.cpp
    nonblocking/Worker.cpp
    nonblocking/Utils.cpp
    nonblocking/EpollManager.cpp)

add_library(Network ${SOURCE_FILES})
target_link_libraries(Network pthread uv Protocol Execute Executor ${CMAKE_THREAD_LIBS_INIT})
