cmake_minimum_required(VERSION 3.16)

project(TetrisLib LANGUAGES CXX)

add_library(TetrisLib STATIC
  Include/TetrisLib/ITetrisBoardData.h
  Include/TetrisLib/ITetrisBoardObserver.h
  Include/TetrisLib/TetrisBoardData.h
  Source/TetrisBoardData.cpp
  Include/TetrisLib/ITetrisPiece.h
  Include/TetrisLib/TetrisPiece.h
  Source/TetrisPiece.cpp
  Include/TetrisLib/Direction.h
  Include/TetrisLib/SpotColor.h
  Include/TetrisLib/SpotInfo.h
  Include/TetrisLib/TetrisRawBoardData.h
  Source/TetrisRawBoardData.cpp
  Include/TetrisLib/ITetrisRawBoardData.h
  Include/TetrisLib/ITetrisPieceProvider.h
  Include/TetrisLib/TetrisPieceProvider.h
  Source/TetrisPieceProvider.cpp
)

target_include_directories(TetrisLib PUBLIC Include)

target_compile_definitions(TetrisLib PRIVATE TETRISLIB_LIBRARY)
