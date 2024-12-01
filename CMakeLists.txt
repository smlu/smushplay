cmake_minimum_required(VERSION 3.14)

# Project name and version
project(smushplay LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Locate all .cpp and .h files in the root folder
file(GLOB SOURCE_FILES "${CMAKE_SOURCE_DIR}/*.cpp" "${CMAKE_SOURCE_DIR}/*.h")

# Add the executable target
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Find SDL2
find_package(SDL2 REQUIRED)

# Find ZLib
find_package(ZLIB REQUIRED)

# Include SDL2 headers and link libraries
target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS} ${ZLIB_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES} ZLIB::ZLIB)

message(STATUS "SDL2 include directories: ${SDL2_INCLUDE_DIRS}")
message(STATUS "SDL2 libraries: ${SDL2_LIBRARIES}")
message(STATUS "zlib include directories: ${ZLIB_INCLUDE_DIRS}")
message(STATUS "zlib libraries: ${ZLIB_LIBRARIES}")