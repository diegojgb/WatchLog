#include "nlohmann/json.hpp"
#include "FileWatcher.h"
#include "Manager.h"
#include "TrayIcon.h"

#include <QApplication>
#include <QQmlApplicationEngine>
#include <dwmapi.h>
#include <QQuickWindow>
#include <QQmlContext>
#include <QVariant>
#include <Qstring>
#include <QGuiApplication>
#include <QObject>

#include <QDir>
#include <fstream>
#include <QMessageBox>

using json = nlohmann::json;


int main(int argc, char *argv[])
{
    qputenv("QT_QPA_PLATFORM", "windows:darkmode=0");
    qputenv("QSG_NO_VSYNC", "1");

    QApplication app(argc, argv);

    std::ifstream file("data.json");

    if (!file.good()) {
        QMessageBox::critical(nullptr, QObject::tr("WatchLog"), QObject::tr("Error opening data.json (does it exist?)"));
        return -1;
    }

    json data;

    try {
        data = json::parse(file);
    } catch (const json::parse_error& e) {
        std::cerr << "JSON parsing failed: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Exception caught during JSON parsing: " << e.what() << std::endl;
        return 1;
    }

    file.close();

    Manager manager(&app, data);

    if (manager.hadInitErrors())
    {
        QApplication::quit();
        return 1;
    }

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, QObject::tr("Systray"),
                                 QObject::tr("I couldn't detect any system tray "
                                             "on this system."));
        return 1;
    }

    QQmlApplicationEngine engine;

    // Expose objects to QML
    engine.rootContext()->setContextProperty("Manager", &manager);

    const QUrl url(u"qrc:/qml/Main.qml"_qs);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    // Set window to dark mode.
    QWindow* window = QGuiApplication::allWindows().at(0);
    HWND windowHandle = (HWND)window->winId();

    BOOL enableDark = true;
    if (!SUCCEEDED(DwmSetWindowAttribute(windowHandle, 20, &enableDark, sizeof(enableDark)))) // Windows 10 20H1+
        DwmSetWindowAttribute(windowHandle, 19, &enableDark, sizeof(enableDark)); // Windows 10 before 20H1

    // Tray icon (using Qt Widgets).
    QObject *root = engine.rootObjects().at(0);
    TrayIcon trayIcon(&app, root, windowHandle);

    QObject::connect(&manager.m_fileWatcher, &FileWatcher::matchFound, &trayIcon, &TrayIcon::sendNotification);

    return app.exec();
}
