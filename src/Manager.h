#ifndef MANAGER_H
#define MANAGER_H

#include "FileWatcher.h"
#include "Monitor.h"
#include "MonitorCollection.h"
#include "TrayIcon.h"

#include <QObject>
#include <QHash>
#include <QStringList>

using json = nlohmann::json;


class Manager: public QObject
{
    Q_OBJECT

    Q_PROPERTY(MonitorCollection* monitors READ monitors CONSTANT)

public:
    explicit Manager(QObject *parent, const json &monitorsData);

    void initTrayIcon(QObject* parent, QObject* root, HWND &hwnd);

    bool hadInitErrors() const;
    json toJSON() const;

    MonitorCollection* monitors();

public slots:
    void disableMonitor(const Monitor* monitor);
    void enableMonitor(const Monitor* monitor);
    void updateJSON() const;
    bool addMonitor(const QString& name, const QString& filePath);
    void onCheckFailed(const QString& filePath);

private:
    FileWatcher m_fileWatcher;
    MonitorCollection m_monitors;
    WinFileManager m_winFileManager;
    TrayIcon* m_trayIcon;
    bool m_error;
    bool m_trayIconInitialized = false;
};

#endif // MANAGER_H
