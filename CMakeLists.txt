cmake_minimum_required(VERSION 3.10)
project(HospitalNetwork)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directory
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add source files
set(SOURCES
    src/main.cpp
    src/Hospital.cpp
    src/HospitalNetwork.cpp
)

# Create executable
add_executable(hospital_network ${SOURCES})

# Create data directory if it doesn't exist
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/data) 