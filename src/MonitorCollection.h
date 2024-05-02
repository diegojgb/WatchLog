#ifndef MONITORCOLLECTION_H
#define MONITORCOLLECTION_H

#include "Monitor.h"

#include <QObject>
#include <QAbstractListModel>


class MonitorCollection : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit MonitorCollection(QObject *parent = nullptr);

    // QAbstractListModel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void changeFilePath(const QString &oldKey, const QString &newKey);
    const QStringList& getOrder() const;
    const QHash<QString, Monitor*>& getHash() const;

public slots:
    Monitor* get(const QString filePath) const;
    bool insert(QString filePath, Monitor* monitor);
    void remove(QString filePath);

private:
    QStringList m_order;
    QHash<QString, Monitor*> m_hash;
};

#endif // MONITORCOLLECTION_H


