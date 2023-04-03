cmake_minimum_required(VERSION 3.16)
project(medicalPredictor)

set(CMAKE_CXX_STANDARD 14)

include_directories(Code)

add_executable(medicalPredictor
        Code/CovidModeler.cpp
        Code/Graph.cpp
        Code/Graph.h
        Code/CovidModeler.h)

add_executable(BitsprayerTest
        Code/Bitsprayer.cpp
        Code/Bitsprayer.h
        )

add_executable(MetricsTest
        Code/Metrics.cpp
        )