cmake_minimum_required(VERSION 4.4)

project(password_generator
        VERSION 0.1
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(password_generator
        main.cpp
)

target_link_libraries(password_generator
        PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Widgets)
