project(CodeCraftSDK)
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

add_compile_definitions(DISABLE_PTHREAD_HOOK)
if (NOT WIN32)
    link_libraries(pthread m)
endif (NOT WIN32)

AUX_SOURCE_DIRECTORY(. src)
list(REMOVE_ITEM src "./load_pthread.cpp")
ADD_EXECUTABLE(main ${src})