cmake_minimum_required(VERSION 3.5)
project(PETools LANGUAGES CXX)


set(CMAKE_PREFIX_PATH ${Qt5_Lib_Local_Path})
MESSAGE(STATUS "Qt5_Lib_Local_Path: ${Qt5_Lib_Local_Path} ")


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
    enable_language("RC")
    set (WIN32_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/icon.rc)
    add_definitions(-D_UNICODE -DWIN32 -DUNICODE)
    set(CMAKE_WIN32_EXECUTABLE ON)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
   # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} /guard:cf")
   # add_compile_options("/GD")
    add_compile_options("/guard:cf")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    MESSAGE(STATUS "current platform: Mac OS X")
ELSE ()
    MESSAGE(STATUS "other platform: ${CMAKE_SYSTEM_NAME}")
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

MESSAGE(STATUS "CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR} ")
MESSAGE(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR} ")
MESSAGE(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
include_directories(${Qt5Widgets_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR} 
   "${CMAKE_CURRENT_SOURCE_DIR}/PEInfo" 
   "${CMAKE_CURRENT_SOURCE_DIR}/Util" 
   "${CMAKE_CURRENT_SOURCE_DIR}/GUI"
   )
file(GLOB_RECURSE PE_LIB_SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/PEInfo/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/Util/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/GUI/*.cpp")



MESSAGE(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
MESSAGE(STATUS "PE_LIB_SRC_FILES: ${PE_LIB_SRC_FILES}")

set (GUI_CPP_FILES
    GUI/common/baseWindow.cpp 
    GUI/common/baseTableView.cpp
    GUI/common/hoverWindow.cpp
    GUI/common/qbasiclabel.cpp
    GUI/common/hexlineedit.cpp
    GUI/PEMainWindow.cpp
    GUI/rvatofoawindow.cpp
    GUI/dataDirectoryWidget.cpp
    GUI/PEHeaderItemListWidget.cpp 
    GUI/sectionTable.cpp
    GUI/importTable.cpp
    GUI/exportFunctionWidget.cpp
    GUI/dllTree.cpp 
)

set (GUI_HEADER_FILES
    GUI/common/baseWindow.h
    GUI/common/baseTableView.h
    GUI/common/hoverWindow.h
    GUI/common/qbasiclabel.h
    GUI/common/hexlineedit.h
    GUI/constants/constStrings.h
    GUI/PEMainWindow.h
    GUI/rvatofoawindow.h
    GUI/dataDirectoryWidget.h
    GUI/PEHeaderItemListWidget.h
    GUI/sectionTable.h 
    GUI/importTable.h
    GUI/exportFunctionWidget.h
    GUI/dllTree.h
)

SET(PROJ_SOURCES
    main.cpp
    peloadworker.cpp
    taskMgr.cpp
    ${GUI_CPP_FILES}
    
)
 
SET(PROJ_HEADERS
    peloadworker.h
    taskMgr.h
    ${GUI_HEADER_FILES}
)

# SET(PROJ_FORMS   rvatofoawindow.ui  )

SET(PROJ_RESOURCES 
    res.qrc
) 

set (RC_ICONS = pe.ico)
  
#call precompiler moc, need to use QT5_WRAP_CPP macro
QT5_WRAP_CPP(PROJ_HEADERS_MOC ${PROJ_HEADERS})

#use uic to process .ui files
QT5_WRAP_UI(PROJ_FORMS_HEADERS ${PROJ_FORMS})

#use rcc to process .qrc files
QT5_ADD_RESOURCES(PROJ_RESOURCES_RCC ${PROJ_RESOURCES})

# Qt5LinguistTools
find_package(Qt5 COMPONENTS LinguistTools REQUIRED)


set(LANG_PATH ${CMAKE_CURRENT_BINARY_DIR}/langs)
MESSAGE(STATUS "LANG_PATH: ${LANG_PATH}")

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
    execute_process(COMMAND mkdir -p ${LANG_PATH})
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
    MESSAGE(STATUS "LANG_PATH: ${LANG_PATH} on windows")
    execute_process(COMMAND if not exist ${LANG_PATH} (mkdir ${LANG_PATH}))
ENDIF()
  
FILE(GLOB QM_FILES "${CMAKE_CURRENT_SOURCE_DIR}/langs/*.qm")
MESSAGE(STATUS "QM_FILES: ${QM_FILES}")
MESSAGE(STATUS "CMAKE_PROJECT_NAME: ${CMAKE_PROJECT_NAME}")

#Copy Qm files to langs folder
foreach(QM_FILE ${QM_FILES})
    get_filename_component(QM_FILENAME "${QM_FILE}" NAME)
    set(TARGET_QM_FILE_PATH "${CMAKE_CURRENT_BINARY_DIR}/langs/${QM_FILENAME}")
    MESSAGE(STATUS "TARGET_QM_FILE_PATH:  ${TARGET_QM_FILE_PATH}")
    configure_file(${QM_FILE} ${TARGET_QM_FILE_PATH} COPYONLY)
endforeach()
 


ADD_EXECUTABLE( ${CMAKE_PROJECT_NAME}  WIN32
    ${QM_FILES}
    ${PROJ_SOURCES} 
    ${PE_LIB_SRC_FILES}
    ${PROJ_HEADERS_MOC} 
    ${PROJ_FORMS_HEADERS}
    ${PROJ_RESOURCES_RCC}
    ${WIN32_RESOURCES}
)
 
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} Qt5::Core Qt5::Widgets Qt5::Gui)


