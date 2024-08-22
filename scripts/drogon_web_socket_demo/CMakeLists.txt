cmake_minimum_required(VERSION 3.10)
project(WebSocketApp)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the Drogon package
find_package(Drogon CONFIG REQUIRED)

# Include Drogon headers
include_directories(${DROGON_INCLUDE_DIRS})

# Add executable for the server
add_executable(server server.cpp)

# Link Drogon library to the server executable
target_link_libraries(server PRIVATE Drogon::Drogon)

# Add executable for the client
add_executable(client client.cpp)

# Link Drogon library to the client executable
target_link_libraries(client PRIVATE Drogon::Drogon)
