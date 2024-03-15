# To-Do
# Add a char limit to the name of a Notifier.
# Handle case with invalid JSON syntax.
# Figure out a way to just take the CusSwitch from TaoQuick.

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

    SOURCES
    FileWatcher.h FileWatcher.cpp
    Monitor.h Monitor.cpp
    Manager.h Manager.cpp
    Notifier.h Notifier.cpp
    TrayIcon.h TrayIcon.cpp
    ToastHandler.h

    RESOURCES
    assets/notification.png
    assets/crown-logo.ico
    assets/watchlog-logo.png
    assets/watchlog-logo.ico
    QML_FILES Expandable.qml
    QML_FILES NotifierPreview.qml
    RESOURCES  assets/right-arrow.png
)

#-- JSON ------------------------------

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
)

include(GNUInstallDirs)
include(taoQuick.cmake)
install(TARGETS appFallGuysNotifier
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
