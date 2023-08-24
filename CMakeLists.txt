cmake_minimum_required(VERSION 3.5)

project(qtws-2023 VERSION 1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets Qml)
find_package(Qt6 REQUIRED COMPONENTS Widgets Qml)

add_subdirectory(src)
