#include "NotifierList.h"

NotifierList::NotifierList(QObject *parent)
    : QAbstractListModel{parent}
{}

int NotifierList::rowCount(const QModelIndex& parent) const
{
    return m_notifiers.size();
}

QVariant NotifierList::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= m_notifiers.size())
        return QVariant();

    if(role == Qt::DisplayRole) {
        return QVariant::fromValue(m_notifiers[index.row()]);
    }

    return QVariant();
}

Notifier* NotifierList::at(int i) const
{
    if (i < 0 || i >= m_notifiers.size())
        return nullptr;

    return m_notifiers[i];
}

void NotifierList::append(Notifier* notifier)
{
    int newIndex = m_notifiers.size();

    beginInsertRows(QModelIndex(), newIndex, newIndex);
    m_notifiers.append(notifier);
    endInsertRows();
}

void NotifierList::remove(int i)
{
    if (i < 0 || i >= m_notifiers.size())
        return;

    beginRemoveRows(QModelIndex(), i, i);
    m_notifiers.removeAt(i);
    endRemoveRows();
}
