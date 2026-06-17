cmake_minimum_required(VERSION 3.16)

project(NZFTC_EMS_UI VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Path to Qt installation
set(CMAKE_PREFIX_PATH "C:/Qt/6.9.2/msvc2022_64")

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(NZFTC_EMS_UI
    mainUI.cpp
    ../Shared_UI_Elements/Calendar_Widget.cpp
)

target_link_libraries(NZFTC_EMS_UI PRIVATE Qt6::Widgets)
