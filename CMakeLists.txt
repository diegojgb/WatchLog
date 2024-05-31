cmake_minimum_required(VERSION 3.16)

project(WatchLog VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick Widgets)

qt_standard_project_setup()

add_subdirectory(WinToast)

set(app_icon_resource "${CMAKE_CURRENT_SOURCE_DIR}/assets/watchlog.rc")

set(SOURCE_FILES
    src/FileWatcher.h
    src/FileWatcher.cpp
    src/Monitor.h
    src/Monitor.cpp
    src/Manager.h
    src/Manager.cpp
    src/Notifier.h
    src/Notifier.cpp
    src/TrayIcon.h
    src/TrayIcon.cpp
    src/ToastHandler.h
    src/SystemMedia.h
    src/SystemMedia.cpp
    src/Utils.h
    src/Utils.cpp
    src/Registry.h
    src/Registry.cpp
    src/NotifierList.h
    src/NotifierList.cpp
    src/MonitorCollection.h
    src/MonitorCollection.cpp
    src/FileChangeWorker.h
    src/FileChangeWorker.cpp
    src/MultiDirMonitor.cpp
    src/MultiDirMonitor.h
    src/SingleDirMonitor.cpp
    src/SingleDirMonitor.h
    src/WinFileMonitor.cpp
    src/WinFileMonitor.h
    src/WinFileManager.h
    src/WinFileManager.cpp
    src/FileCheckWorker.h
    src/FileCheckWorker.cpp
    src/FileChecker.h
    src/FileChecker.cpp
    src/FileStatus.h
    src/FileStatus.cpp
    src/ExitDialog.h
    src/ExitDialog.cpp
)

qt_add_executable(appWatchLog 
    src/main.cpp
    qml.qrc
    assets.qrc
    ${app_icon_resource}
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

target_sources(appWatchLog PRIVATE
    ${SOURCE_FILES}
)

target_link_libraries(appWatchLog
    PRIVATE Qt6::Quick
    PRIVATE Qt6::Widgets
    PRIVATE dwmapi
    PRIVATE WinToast
    PRIVATE Winmm
)

include(GNUInstallDirs)

install(TARGETS appWatchLog
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
