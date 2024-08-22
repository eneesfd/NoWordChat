cmake_minimum_required(VERSION 3.5.0)
project(nowordchat VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Drogon package
find_package(Drogon CONFIG REQUIRED)

# Add subdirectories for libraries
add_subdirectory(libs/BidirectionalMessenger)

# Include Drogon headers
include_directories(${Drogon_INCLUDE_DIRS})

add_executable(nowordchat main.cpp)

target_link_libraries(nowordchat PRIVATE Drogon::Drogon BidirectionalMessenger)

