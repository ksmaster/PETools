cmake_minimum_required(VERSION 3.5)
project(PETools LANGUAGES CXX)

set(Qt5_DIR "D:/Qt/Qt5.14.2/5.14.2/msvc2017/lib/cmake/Qt5")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
#FIND_PACKAGE(Qt5 COMPONENTS Core Gui Qml Quick Widgets REQUIRED)
FIND_PACKAGE(Qt5 COMPONENTS Widgets REQUIRED)

SET(PROJ_SOURCES 
    main.cpp 
    pewindow.cpp
    qbasiclabel.cpp
    mythread.cpp
    peloadworker.cpp
    globaldata.cpp
)

SET(PROJ_HEADERS 
    pewindow.h
    qbasiclabel.h
    mythread.h
    peloadworker.h
    globaldata.h
)

SET(PROJ_FORMS 
    pewindow.ui
)

SET(PROJ_RESOURCES 
    res.qrc
)

#call precompiler moc, need to use QT5_WRAP_CPP macro
QT5_WRAP_CPP(PROJ_HEADERS_MOC ${PROJ_HEADERS})

#use uic to process .ui files
QT5_WRAP_UI(PROJ_FORMS_HEADERS ${PROJ_FORMS})

#use rcc to process .qrc files
#QT5_ADD_RESOURCES(PROJ_RESOURCES_RCC ${PROJ_RESOURCES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/PEInfo" "${CMAKE_CURRENT_SOURCE_DIR}/Util")
file(GLOB_RECURSE PE_LIB_SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/PEInfo/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/Util/*.cpp")


ADD_EXECUTABLE(PETools 
    ${PROJ_SOURCES} 
    ${PE_LIB_SRC_FILES}
    ${PROJ_HEADERS_MOC} 
    ${PROJ_FORMS_HEADERS}
    ${PROJ_RESOURCES_RCC}
)

#TARGET_LINK_LIBRARIES(PETools Qt5::Core Qt5::Gui Qt5::Qml Qt5::Quick ${Qt5Widgets_LIBRARIES})
TARGET_LINK_LIBRARIES(PETools ${Qt5Widgets_LIBRARIES})
INSTALL(TARGETS PETools
    RUNTIME DESTINATION lib
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
