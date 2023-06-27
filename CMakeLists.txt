cmake_minimum_required(VERSION 3.16)
project(medicalPredictor)

set(CMAKE_CXX_STANDARD 17)

include_directories(Code)

add_executable(medicalPredictor
        Code/CovidModeler.cpp
        Code/CovidModeler.h
        Code/Graph.cpp
        Code/Graph.h
        MP/SDA.h
        MP/SDA.cpp
        MP/Evolver.cpp
        MP/Evolver.h)

add_executable(SDATest
        MP/SDA.cpp
        MP/SDA.h
        )

add_executable(MetricsTest
        Code/Metrics.cpp
        )