cmake_minimum_required(VERSION 3.5)

project(micro_brico_app VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        connect_db.cpp
        datamodel.cpp
        datamodel.h
        utilisateur.cpp
        utilisateur.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(micro_brico_app
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}

        datamodel.h
        connect_db.h connect_db.cpp
        datamodel.cpp
        utilisateur.h utilisateur.cpp


        popups/popupaddusers.h popups/popupaddusers.cpp popups/popupaddusers.ui
        popups/login_connect.h popups/login_connect.cpp popups/login_connect.ui
        popups/delete_user.h popups/delete_user.cpp popups/delete_user.ui
        logger/logbrowser.h logger/logbrowser.cpp
        logger/logbrowserdialog.h logger/logbrowserdialog.cpp
        popups/poppupaddkit.h popups/poppupaddkit.cpp popups/poppupaddkit.ui
        kit.h kit.cpp
        item.h item.cpp
        money.h money.cpp
        resa.h resa.cpp
        utils.h utils.cpp
        popups/popupsortirresa.h popups/popupsortirresa.cpp popups/popupsortirresa.ui
        sortie.h sortie.cpp
        log.h log.cpp
        logger/log_user_kit_appli.h logger/log_user_kit_appli.cpp logger/log_user_kit_appli.ui
        types.h
        popups/popuppchangeitemquantities.h popups/popuppchangeitemquantities.cpp popups/popuppchangeitemquantities.ui







    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET micro_brico_app APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(micro_brico_app SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(micro_brico_app
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(micro_brico_app PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

find_package(Qt6 REQUIRED COMPONENTS Sql)
target_link_libraries(micro_brico_app PRIVATE Qt6::Sql)
include(GNUInstallDirs)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.micro_brico_app)
endif()
set_target_properties(micro_brico_app PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
install(TARGETS micro_brico_app
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(micro_brico_app)
endif()
