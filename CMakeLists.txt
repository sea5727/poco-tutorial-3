cmake_minimum_required(VERSION 3.0.0)
project(poco-tutorial-1 VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include(CTest)
enable_testing()

# list(APPEND CMAKE_MODULE_PATH "/usr/local/lib/cmake")
list(APPEND CMAKE_MODULE_PATH "/usr/local/lib/cmake")
message(CMAKE_MODULE_PATH:${CMAKE_MODULE_PATH})
# list(APPEND CMAKE_MODULE_PATH "/usr/local/poco-poco-1.10.1-release/cmake")

# find_package(Poco REQUIRED COMPONENTS Foundation Net Zip Util)
find_package(Poco REQUIRED COMPONENTS Net)
if(Poco_FOUND)
    message(Poco_LIBRARIES: ${Poco_LIBRARIES})
    message(Poco_INCLUDE_DIR: ${Poco_INCLUDE_DIR})
    message(Poco_NET_INCLUDE_DIR: ${Poco_NET_INCLUDE_DIR})

    
    # include_directories(/usr/local/include)
    # link_libraries(/usr/local/lib)
    # include_directories(/usr/local/include)
    link_directories(/usr/local/gcc/lib64 /usr/local/lib )
endif()

add_executable(poco-tutorial-3 main.cpp)
#Poco::Foundation Poco::Util Poco::Net
target_link_libraries(poco-tutorial-3 Poco::Net)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

