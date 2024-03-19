# To-Do
# • Add a char limit to the name of a Notifier.
# • Handle case with invalid JSON syntax.
# • Figure out a way to just take the CusSwitch from TaoQuick.
# • Re-check if there is a way to make the regex checks more efficient.
# • Check the behavior of broken notifications after spamming them. Do they ever come back to working normally or are they broken until app restart?

# BUGS:
# • After multiple hours, it stops working. If I disable and re-enable, it fixes.
#   When I disable and re-enable this are the actions that take place:
#      1. It removes the filePath from the QFileSystemWatcher and adds it back again.
#      2. It executes: "m_file.seekg(0, std::ios::end);" on the Monitor.
# • Whenever I run the application in Debug mode, everytime a notification is shown, despite it working fine, it shows this error:
#   onecoreuap\base\diagnosis\platform\notifications\developer\basenotification.cpp(35)\wpnapps.dll!00007FF92DD64B52: (caller: 00007FF92DD6123E) LogHr(1) tid(1334) 80070490 Element not found.
#    I tonly happens while Debugging.

cmake_minimum_required(VERSION 3.16)

project(FallGuysNotifier VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick Widgets)

qt_standard_project_setup()

add_subdirectory(WinToast)

set(app_icon_resource "${CMAKE_CURRENT_SOURCE_DIR}/assets/watchlog.rc")

if (CMAKE_BUILD_TYPE MATCHES "Release")
qt_add_executable(appFallGuysNotifier main.cpp ${TaoQuickRes} ${app_icon_resource})
else()
qt_add_executable(appFallGuysNotifier main.cpp ${app_icon_resource})
endif()

qt_add_qml_module(appFallGuysNotifier
    URI FallGuysNotifier
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
)

#-- JSON ----------------

# set(JSON_INC_PATH C:/dev/vcpkg/packages/nlohmann-json_x64-windows/include)
# target_include_directories(appFallGuysNotifier PUBLIC ${JSON_INC_PATH})

# set(JSON_LIB_PATH C:/dev/vcpkg/packages/nlohmann-json_x64-windows/lib)
# target_link_directories(appFallGuysNotifier PUBLIC ${JSON_LIB_PATH})

#----------------------------------------

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appFallGuysNotifier PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appFallGuysNotifier
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    OUTPUT_NAME "FallGuysNotify"
)

target_link_libraries(appFallGuysNotifier
    PRIVATE Qt6::Quick
    PRIVATE Qt6::Widgets
    PRIVATE dwmapi
    PRIVATE WinToast
    PRIVATE Winmm
)

include(GNUInstallDirs)
include(taoQuick.cmake)
install(TARGETS appFallGuysNotifier
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
