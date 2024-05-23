cmake_minimum_required(VERSION 3.5)

project(new_imgproc VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(new_imgproc
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        res.qrc

        openfilepage.h openfilepage.cpp openfilepage.ui
        confirmpage.h confirmpage.cpp confirmpage.ui
        wizard.h wizard.cpp wizard.ui
        filetree.h filetree.cpp filetree.ui
        filetreewidget.h filetreewidget.cpp
        filetreeitem.h filetreeitem.cpp
        const.h
        filetreethread.h filetreethread.cpp
        removefiledialog.h removefiledialog.cpp removefiledialog.ui
        openfilethread.h openfilethread.cpp
        imgshow.h imgshow.cpp imgshow.ui
        toolwidget.h toolwidget.cpp toolwidget.ui
        imgprocess.h imgprocess.cpp


    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET new_imgproc APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(new_imgproc SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(new_imgproc
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(new_imgproc PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.new_imgproc)
endif()
set_target_properties(new_imgproc PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS new_imgproc
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(new_imgproc)
endif()
