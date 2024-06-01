#ifndef NOTIFIERLIST_H
#define NOTIFIERLIST_H

#include "Notifier.h"

#include <QObject>
#include <QAbstractListModel>

class NotifierList: public QAbstractListModel
{
    Q_OBJECT

public:
    explicit NotifierList(QObject *parent = nullptr);
    ~NotifierList();

    // QAbstractListModel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE void append(Notifier* notifier);
    Q_INVOKABLE void removeAt(int i);
    Q_INVOKABLE Notifier* at(int i) const;

private:
    QList<Notifier*> m_notifiers;
};

#endif // NOTIFIERLIST_H
