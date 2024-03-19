#include "FileWatcher.h"
#include "nlohmann/json.hpp"

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

#include <QDir>
#include <fstream>
#include <QMessageBox>

using json = nlohmann::json;


int main(int argc, char *argv[])
{
    qputenv("QT_QPA_PLATFORM", "windows:darkmode=0");
    QApplication app(argc, argv);

    // Instantiation of functional parts.
    std::ifstream file("data.json");
    json data = json::parse(file);

    Manager manager(&app, data);

    if (manager.hadInitErrors())
    {
        QApplication::quit();
        return 0;
    }

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, QObject::tr("Systray"),
                                 QObject::tr("I couldn't detect any system tray "
                                             "on this system."));
        return 1;
    }
    // QApplication::setQuitOnLastWindowClosed(false);

    QQmlApplicationEngine engine;

    // Expose objects to QML
    engine.addImportPath(TaoQuickImportPath);
    engine.rootContext()->setContextProperty("taoQuickImportPath", TaoQuickImportPath);
    engine.rootContext()->setContextProperty("Manager", &manager);

    const QUrl url(u"qrc:/FallGuysNotifier/Main.qml"_qs);
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
    TrayIcon trayIcon(&app, root);

    QObject::connect(&manager.m_fileWatcher, &FileWatcher::matchFound, &trayIcon, &TrayIcon::sendNotification);

    return app.exec();
}
