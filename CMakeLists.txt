cmake_minimum_required(VERSION 3.5)

project(PETools LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
    MESSAGE(STATUS "current platform: Linux ")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
    MESSAGE(STATUS "current platform: Windows")
    add_definitions(-D_UNICODE -DWIN32 -DUNICODE)
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    MESSAGE(STATUS "current platform: Mac OS X")
ELSE ()
    MESSAGE(STATUS "other platform: ${CMAKE_SYSTEM_NAME}")
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")


find_package(Qt5 COMPONENTS Widgets REQUIRED)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/PEInfo" "${CMAKE_CURRENT_SOURCE_DIR}/Util")
file(GLOB_RECURSE pe_files "${CMAKE_CURRENT_SOURCE_DIR}/PEInfo/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/Util/*.cpp")



add_executable(PETools
  main.cpp
  pewindow.cpp
  pewindow.h
  pewindow.ui
  qbasiclabel.cpp
    qbasiclabel.h
    mythread.cpp
    mythread.h
    peloadworker.cpp
    peloadworker.h
    globaldata.cpp
    globaldata.h
    res.qrc
    ${pe_files}
)

target_link_libraries(PETools PRIVATE Qt5::Widgets)
