cmake_minimum_required(VERSION 3.0)
project(FaceMerger)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(face_merger opencv.cpp)

target_link_libraries(face_merger ${OpenCV_LIBS})

