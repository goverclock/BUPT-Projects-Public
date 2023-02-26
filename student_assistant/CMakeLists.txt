cmake_minimum_required(VERSION 3.5)

project(data-structure-curriculum-design VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(PROJECT_SOURCES
    resource/res.qrc

    code/main.cpp
    code/primary.cpp
    code/primary.h
    code/tool_lib.cpp
    code/tool_lib.h

    code/gui/gui.cpp
    code/gui/gui.h
    code/gui/button.cpp
    code/gui/button.h
    code/gui/text.cpp
    code/gui/text.h
    code/gui/menu.cpp
    code/gui/menu.h
    code/gui/map_gui/map_gui.cpp
    code/gui/map_gui/map_gui.h
    code/gui/activity_gui/activity_gui.cpp
    code/gui/activity_gui/activity_gui.h
    code/gui/table_gui/table_gui.cpp
    code/gui/table_gui/table_gui.h
    code/gui/residant/bird.cpp
    code/gui/residant/bird.h
    code/gui/residant/digit_clock.cpp
    code/gui/residant/digit_clock.h
    code/gui/residant/time_controller.cpp
    code/gui/residant/time_controller.h
    code/gui/residant/shell/shell.cpp
    code/gui/residant/shell/shell.h
    code/gui/residant/shell/msg_queue.cpp
    code/gui/residant/shell/msg_queue.h

    code/map/map.cpp
    code/map/map.h
    code/map/path.cpp
    code/map/path.h
    code/map/building/building.cpp
    code/map/building/building.h

    code/activity/activity.cpp
    code/activity/activity.h
    code/activity/event/event.cpp
    code/activity/event/event.h
    code/activity/event/alarm.cpp
    code/activity/event/alarm.h
    code/activity/event/move.cpp
    code/activity/event/move.h
    code/activity/event/lesson.cpp
    code/activity/event/lesson.h
    code/activity/event/exam.cpp
    code/activity/event/exam.h
    code/activity/event/outdoors.cpp
    code/activity/event/outdoors.h

    code/timetable/timetable.cpp
    code/timetable/timetable.h

    code/view/view.cpp
    code/view/view.h
        
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(data-structure-curriculum-design
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET data-structure-curriculum-design APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(data-structure-curriculum-design SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(data-structure-curriculum-design
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(data-structure-curriculum-design PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(data-structure-curriculum-design PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(data-structure-curriculum-design)
endif()

# I added the followding
 set(LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")