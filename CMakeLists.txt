cmake_minimum_required(VERSION 3.5.0)
project(nowordchat VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the required packages
find_package(Boost REQUIRED COMPONENTS system)
find_package(Drogon CONFIG REQUIRED)

# Add libs to the project
add_subdirectory(libs/BidirectionalMessenger)

# Specify the include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/BidirectionalMessenger/inc)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${Drogon_INCLUDE_DIRS})

add_executable(nowordchat src/main.cpp src/ChatWebSocket.cpp)

target_link_libraries(nowordchat PRIVATE Drogon::Drogon BidirectionalMessenger)

