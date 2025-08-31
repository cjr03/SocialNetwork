cmake_minimum_required(VERSION 3.16)
project(SocialNetwork LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

include_directories(include)

add_executable(SocialNetwork
    src/main.cpp
    src/mainwindow.cpp
    src/network.cpp
    src/user.cpp
    src/post.cpp
    src/mainwindow.ui
)

target_link_libraries(SocialNetwork
    Qt6::Widgets
)

