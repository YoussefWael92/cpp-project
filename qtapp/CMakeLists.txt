cmake_minimum_required(VERSION 3.16)

project(StudentSystem_QtGUI VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(StudentSystem_QtGUI
    main.cpp
    mainwindow.h
    mainwindow.cpp
    student.cpp
    course.cpp
    grades.cpp
)

target_link_libraries(StudentSystem_QtGUI PRIVATE Qt6::Widgets)
