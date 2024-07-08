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

// Executes just before the qApp exits.
void cleanup()
{
    WinToast::instance()->clear();
}

bool readArgs(int argc, char* argv[], Manager &manager)
{
    for (int i = 1; i < argc; i++) {
        if (std::strcmp(argv[i], "--minimized") == 0)
            manager.settings()->setStartMinimized(true);
        else
            return false;
    }

    return true;
}

int main(int argc, char *argv[])
{
    qputenv("QT_QPA_PLATFORM", "windows:darkmode=0");
    qputenv("QSG_NO_VSYNC", "1");

    QApplication app(argc, argv);
    app.setStyle("windowsvista");

    QObject::connect(&app, &QCoreApplication::aboutToQuit, cleanup);

    std::ifstream file("data.json");
    json data;

    if (file.good()) {
        try {
            data = json::parse(file);
        } catch (const json::parse_error& e) {
            std::cerr << "JSON parsing failed: " << e.what() << std::endl;
            return 1;
        } catch (const std::exception& e) {
            std::cerr << "Exception caught during JSON parsing: " << e.what() << std::endl;
            return 1;
        }
    }

    file.close();

    Manager manager(&app, data);

    if (manager.hadInitErrors()) {
        QApplication::quit();
        return 1;
    }

    bool validArgs = readArgs(argc, argv, manager); // Read and process command line arguments.

    if (!validArgs) {
        Utils::showCritical("Invalid command line argument(s).");
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

    // Redraw the Window. For Windows 10, which doesn't automatically change the title bar color to dark upon change.
    // It's a hacky solution, forces a redraw by resizing the window.
    RECT rect;
    int width;
    int height;

    if(GetWindowRect(windowHandle, &rect)) {
        width = rect.right - rect.left;
        height = rect.bottom - rect.top;
    }

    SetWindowPos(windowHandle, 0, 0, 0, width-1, height, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

    // Tray icon (using Qt Widgets).
    QObject *root = engine.rootObjects().at(0);

    manager.initTrayIcon(&app, root, windowHandle);

    return app.exec();
}
