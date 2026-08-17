cmake_minimum_required(VERSION 3.16)

project(NZFTC_EMS_UI VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Open-source Qt install root (can be overridden at configure time)
set(QT_ROOT "D:/Tools/QT/6.11.1/mingw_64" CACHE PATH "Path to Qt installation root")
list(PREPEND CMAKE_PREFIX_PATH "${QT_ROOT}")

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(NZFTC_EMS_UI
    main.cpp
    mainUI.cpp
    ../Shared_UI_Elements/Admin_Dash_Home_Page.cpp
    ../Shared_UI_Elements/Employee_Dash_Home_Page.cpp
    ../Shared_UI_Elements/Header_Bar.cpp
    ../Shared_UI_Elements/Left_Side_Bar_Navigation.cpp
    ../Shared_UI_Elements/Calendar.cpp
    ../Shared_UI_Elements/Calendar_Widget.cpp
)

target_link_libraries(NZFTC_EMS_UI PRIVATE Qt6::Widgets)
