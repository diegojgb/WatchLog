#ifndef MANAGER_H
#define MANAGER_H

#include "FileWatcher.h"
#include "Monitor.h"

#include <QObject>
#include <QHash>
#include <QStringList>

using json = nlohmann::json;


class Manager: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QStringList monitorsOrder READ monitorsOrder WRITE setMonitorsOrder NOTIFY monitorsOrderChanged FINAL)

public:
    FileWatcher m_fileWatcher;

    explicit Manager(QObject *parent, const json &monitorsData);

    QStringList monitorsOrder() const;
    void setMonitorsOrder(const QStringList &newMonitorsOrder);

    bool hadInitErrors() const;
    json toJSON() const;

public slots:
    Monitor* hashGet(const QString& key);
    void changeFilePath(const QString& oldKey, const QString& newKey);
    void disableMonitor(const Monitor* monitor);
    void enableMonitor(const Monitor* monitor);

signals:
    void monitorsOrderChanged();

private:
    QHash<QString, Monitor*> m_monitorsHash;
    QStringList m_monitorsOrder;
    bool m_error;
};

#endif // MANAGER_H
