cmake_minimum_required(VERSION 3.31)
project(cloud_builder CXX)
set(CMAKE_CXX_STANDARD 20)

add_executable(cloud_builder main.cpp
        Deck.cpp
        Deck.h
        Card.h
        Game.cpp
        Game.h
        Data.cpp)
