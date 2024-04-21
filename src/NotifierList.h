#ifndef NOTIFIERLIST_H
#define NOTIFIERLIST_H

#include "Notifier.h"

#include <QObject>
#include <QAbstractListModel>

class NotifierList : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit NotifierList(QObject *parent = nullptr);

    // QAbstractListModel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

public slots:
    void append(Notifier* notifier);
    void remove(int i);
    Notifier* at(int i) const;

private:
    QList<Notifier*> m_notifiers;
};

#endif // NOTIFIERLIST_H
