#ifndef MONITORCOLLECTION_H
#define MONITORCOLLECTION_H

#include "Monitor.h"

#include <QObject>
#include <QAbstractListModel>


struct MonitorData {
    QString filePath;
    Monitor* monitor;

    explicit MonitorData(const QString path, Monitor* monitor);
};

class MonitorCollection : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit MonitorCollection(QObject *parent = nullptr);

    // QAbstractListModel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    const QList<MonitorData*> &getList() const;
    void changeFilePath(const QString &oldKey, const QString &newKey);
    Monitor* get(const QString &filePath) const;
    bool contains(const QString &filePath) const;
    bool insert(const QString &filePath, Monitor* monitor);

    Q_INVOKABLE Monitor* getAt(const int i) const;
    Q_INVOKABLE void remove(const QString &filePath);
    Q_INVOKABLE void removeAt(int i);

private:
    QList<MonitorData*> m_list;

    MonitorData* getMonitorData(const QString &filePath) const;
};

#endif // MONITORCOLLECTION_H
