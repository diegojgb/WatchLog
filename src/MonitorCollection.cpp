#include "MonitorCollection.h"


MonitorCollection::MonitorCollection(QObject *parent)
    : QAbstractListModel{parent}
{}

int MonitorCollection::rowCount(const QModelIndex& parent) const
{
    return m_order.size();
}

QVariant MonitorCollection::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= m_order.size())
        return QVariant();

    auto key = m_order[index.row()];
    auto monitor = m_hash[key];

    if (role == Qt::DisplayRole)
        return QVariant::fromValue(monitor->name());

    if (role == Qt::EditRole)
        return QVariant::fromValue(monitor);

    return QVariant();
}

Monitor* MonitorCollection::get(const QString &filePath) const
{
    return m_hash[filePath];
}

void MonitorCollection::insert(QString filePath, Monitor* monitor)
{
    int newIndex = m_order.size();

    beginInsertRows(QModelIndex(), newIndex, newIndex);
    m_hash.insert(filePath, monitor);
    m_order.append(filePath);
    endInsertRows();
}

void MonitorCollection::remove(QString &filePath)
{
    Monitor* monitor = m_hash[filePath];
    int index = m_order.indexOf(filePath);

    beginRemoveRows(QModelIndex(), index, index);
    monitor->setEnabled(false);
    m_hash.remove(filePath);
    m_order.removeAt(index);
    endRemoveRows();

    delete monitor;
}

const QStringList& MonitorCollection::getOrder() const
{
    return m_order;
}

const QHash<QString, Monitor*>& MonitorCollection::getHash() const
{
    return m_hash;
}

void MonitorCollection::changeFilePath(const QString& oldKey, const QString& newKey)
{
    if (!m_hash.contains(oldKey))
        return;

    Monitor* value = m_hash.value(oldKey);
    m_hash.remove(oldKey);
    m_hash.insert(newKey, value);
}
