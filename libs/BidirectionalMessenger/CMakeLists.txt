cmake_minimum_required(VERSION 3.10)
project(BidirectionalMessenger)

# Set the version of C++ standard to use
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include Boost.Asio
find_package(Boost REQUIRED COMPONENTS system)
include_directories(${Boost_INCLUDE_DIRS})

# Specify the include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)

# Add the source files
add_library(BidirectionalMessenger STATIC
    src/server.cpp
    src/client.cpp
)

# Link Boost libraries
target_link_libraries(BidirectionalMessenger PUBLIC ${Boost_LIBRARIES})
