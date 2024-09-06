cmake_minimum_required(VERSION 3.29)
project(simulations_cpp)

set(CMAKE_CXX_STANDARD 14)

add_executable(simulations_cpp
    fluids/fluidSim.cpp
        rigidBody/rigidSim.cpp
        cloths/clothSim.cpp
        softBody/softSim.cpp)
