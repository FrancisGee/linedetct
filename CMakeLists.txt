cmake_minimum_required(VERSION 3.5)
project(linedetect)
find_package(OpenCV REQUIRED)
set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp linefinder.h equation.h utils.h basic.h img_transform.h laneDetection.h laneDetection.cpp img_transform.cpp)
add_executable(linedetect ${SOURCE_FILES} linefinder.h img_transform.h)
target_link_libraries(linedetect ${OpenCV_LIBS})