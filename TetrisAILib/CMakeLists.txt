cmake_minimum_required(VERSION 3.16)

project(TetrisAILib LANGUAGES CXX)

add_library(TetrisAILib STATIC
  Include/TetrisAILib/TetrisAI.h
  Source/TetrisAI.cpp
)

target_include_directories(TetrisAILib PUBLIC Include)

target_link_libraries(TetrisAILib PRIVATE TetrisLib)
