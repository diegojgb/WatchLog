#ifndef MANAGER_H
#define MANAGER_H

#include "FileWatcher.h"
#include "Monitor.h"
#include "MonitorCollection.h"
#include "TrayIcon.h"
#include "Settings.h"

#include <QObject>
#include <QHash>
#include <QStringList>

using json = nlohmann::json;


class Manager: public QObject
{
    Q_OBJECT

    Q_PROPERTY(MonitorCollection* monitors READ monitors CONSTANT)
    Q_PROPERTY(Settings* settings READ settings CONSTANT)

public:
    explicit Manager(QObject* parent, const json& monitorsData);

    void initTrayIcon(QObject* parent, QObject* root, HWND& hwnd);

    bool hadInitErrors() const;
    json toJSON() const;

    MonitorCollection* monitors();
    Settings* settings();

    Q_INVOKABLE bool addMonitor(const QString& name, const QString& filePath);
    Q_INVOKABLE void checkFilesNow();

public slots:
    void disableMonitor(const Monitor* monitor);
    void enableMonitor(const Monitor* monitor);
    void updateJSON() const;
    void onCheckFailed(const QString& filePath);

private:
    FileWatcher m_fileWatcher;
    MonitorCollection m_monitors;
    WinFileManager m_winFileManager;
    TrayIcon* m_trayIcon;
    Settings m_settings;
    bool m_error;
    bool m_trayIconInitialized = false;

    Mode toWinFileMode(const std::string mode);
};

#endif // MANAGER_H
