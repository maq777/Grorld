cmake_minimum_required(VERSION 2.8)

project(Grorld)
add_executable(grorld main.cpp match.cpp mouse.c screen.c)
target_link_libraries(grorld X11)
target_link_libraries(grorld Xext)
target_link_libraries(grorld cv)
target_link_libraries(grorld highgui)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "-std=c++0x")
set(CMAKE_EXE_LINKER_FLAGS "-s")
