#include "MonitorCollection.h"


MonitorData::MonitorData(const QString path, Monitor* monitor)
    : filePath{path}, monitor{monitor}
{}

MonitorCollection::MonitorCollection(QObject *parent)
    : QAbstractListModel{parent}
{}

int MonitorCollection::rowCount(const QModelIndex& parent) const
{
    return m_list.size();
}

QVariant MonitorCollection::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= m_list.size())
        return QVariant();

    auto* monitor = getAt(index.row());

    if (role == Qt::DisplayRole)
        return QVariant::fromValue(monitor->name());

    if (role == Qt::EditRole)
        return QVariant::fromValue(monitor);

    return QVariant();
}

Monitor* MonitorCollection::get(const QString &filePath) const
{
    auto* monitorData = getMonitorData(filePath);

    if (monitorData == nullptr)
        return nullptr;

    return monitorData->monitor;
}

Monitor *MonitorCollection::getAt(const int i) const
{
    return m_list[i]->monitor;
}

bool MonitorCollection::contains(const QString &filePath) const
{
    return getMonitorData(filePath) != nullptr;
}

bool MonitorCollection::insert(const QString &filePath, Monitor* monitor)
{
    if (contains(filePath))
        return false;

    int newIndex = m_list.size();

    beginInsertRows(QModelIndex(), newIndex, newIndex);
    m_list.append(new MonitorData(filePath, monitor));
    endInsertRows();

    return true;
}

void MonitorCollection::remove(const QString &filePath)
{
    for (int i = 0; i < m_list.size(); i++) {
        if (m_list[i]->filePath != filePath)
            continue;

        beginRemoveRows(QModelIndex(), i, i);

        auto monitor = m_list[i]->monitor;
        monitor->setEnabled(false);
        auto toDelete = m_list.takeAt(i);

        endRemoveRows();

        delete monitor;
        delete toDelete;
        break;
    }
}

void MonitorCollection::removeAt(int i)
{
    delete m_list.takeAt(i);
}

MonitorData *MonitorCollection::getMonitorData(const QString &filePath) const
{
    for (auto* monitorData: m_list) {
        if (monitorData->filePath == filePath) {
            return monitorData;
        }
    }

    return nullptr;
}

const QList<MonitorData*>& MonitorCollection::getList() const
{
    return m_list;
}

void MonitorCollection::changeFilePath(const QString& oldKey, const QString& newKey)
{
    if (!contains(oldKey))
        return;

    auto* monitorData = getMonitorData(oldKey);
    monitorData->filePath = newKey;
}
