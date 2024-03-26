cmake_minimum_required(VERSION 3.16)

project(WatchLog VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick Widgets)

qt_standard_project_setup()

add_subdirectory(WinToast)

set(app_icon_resource "${CMAKE_CURRENT_SOURCE_DIR}/assets/watchlog.rc")

if (CMAKE_BUILD_TYPE MATCHES "Release")
qt_add_executable(appWatchLog main.cpp ${TaoQuickRes} ${app_icon_resource})
else()
qt_add_executable(appWatchLog main.cpp ${app_icon_resource})
endif()

qt_add_qml_module(appWatchLog
    URI WatchLog
    VERSION 1.0

    QML_FILES
    Main.qml
    Tab.qml
    MonitorPage.qml
    Expandable.qml
    NotifierPreview.qml
    FileBrowser.qml
    NotifierOptions.qml
    MaterialCheckBox.qml
    CustomButton.qml
    SaveButton.qml
    CustomComboBox.qml

    SOURCES
    FileWatcher.h FileWatcher.cpp
    Monitor.h Monitor.cpp
    Manager.h Manager.cpp
    Notifier.h Notifier.cpp
    TrayIcon.h TrayIcon.cpp
    ToastHandler.h
    SystemMedia.h SystemMedia.cpp

    RESOURCES
    assets/notification.png
    assets/crown-logo.ico
    assets/watchlog-logo.png
    assets/watchlog-logo.ico
    assets/right-arrow.png
    assets/information.png
    assets/save.png
    assets/save-disabled.png
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appWatchLog PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appWatchLog
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    OUTPUT_NAME "WatchLog"
)

target_link_libraries(appWatchLog
    PRIVATE Qt6::Quick
    PRIVATE Qt6::Widgets
    PRIVATE dwmapi
    PRIVATE WinToast
    PRIVATE Winmm
)

include(GNUInstallDirs)
include(taoQuick.cmake)
install(TARGETS appWatchLog
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
