#ifndef MANAGER_H
#define MANAGER_H

#include "FileWatcher.h"
#include "Monitor.h"
#include "MonitorCollection.h"

#include <QObject>
#include <QHash>
#include <QStringList>

using json = nlohmann::json;


class Manager: public QObject
{
    Q_OBJECT

    Q_PROPERTY(MonitorCollection* monitors READ monitors CONSTANT)

public:
    FileWatcher m_fileWatcher;

    explicit Manager(QObject *parent, const json &monitorsData);

    bool hadInitErrors() const;
    json toJSON() const;

    MonitorCollection* monitors();

public slots:
    void changeFilePath(const QString& oldKey, const QString& newKey);
    void disableMonitor(const Monitor* monitor);
    void enableMonitor(const Monitor* monitor);
    void updateJSON() const;
    void addMonitor(const QString& name, const QString& filePath);

private:
    MonitorCollection m_monitors;
    bool m_error;
};

#endif // MANAGER_H
