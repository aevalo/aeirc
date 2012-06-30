project(aeirc-gui CXX)
cmake_minimum_required(VERSION 2.8)

set(AEIRC-GUI_VERSION_MAJOR 0)
set(AEIRC-GUI_VERSION_MINOR 1)
set(AEIRC-GUI_VERSION_BUILD 1)
set(AEIRC-GUI_VERSION "${AEIRC-GUI_VERSION_MAJOR}.${AEIRC-GUI_VERSION_MINOR}.${AEIRC-GUI_VERSION_BUILD}")

find_package(Qt4 REQUIRED)

set(QT_USE_QTNETWORK TRUE)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(aeirc-gui_SOURCES main.cpp mainwindow.cpp)
set(aeirc-gui_HEADERS mainwindow.h)
set(aeirc-gui_RESOURCES images.qrc)

QT4_WRAP_CPP(aeirc-gui_HEADERS_MOC ${aeirc-gui_HEADERS})
QT4_ADD_RESOURCES(aeirc-gui_RESOURCES_RCC ${aeirc-gui_RESOURCES})

add_executable(aeirc-gui ${aeirc-gui_SOURCES} ${aeirc-gui_HEADERS_MOC} ${aeirc-gui_RESOURCES_RCC})
target_link_libraries(aeirc-gui ${QT_LIBRARIES})

set_target_properties(aeirc-gui PROPERTIES VERSION ${AEIRC-GUI_VERSION})

install(TARGETS aeirc-gui
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)
