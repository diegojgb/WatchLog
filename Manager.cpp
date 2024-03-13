#include "Manager.h"


Manager::Manager(QObject *parent, const json &monitorsData)
    : QObject{parent}, m_fileWatcher{this, m_monitorsHash}, m_error{false}
{
    try
    {
        for (const auto& item: monitorsData) {
            Monitor* newMonitor = new Monitor(this, item);
            QString filePath = QString::fromStdString(item["filePath"]);

            m_monitorsHash.insert(filePath, newMonitor);
            m_monitorsOrder.append(QString::fromStdString(item["filePath"]));

            QObject::connect(newMonitor, &Monitor::filePathChangedOverload, this, &Manager::changeFilePath);
        }

        m_fileWatcher.addAllMonitors();
    }
    catch(const std::exception& e)
    {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        m_error = true;
    }
}

Monitor* Manager::hashGet(const QString& key) {
    return m_monitorsHash[key];
}

void Manager::changeFilePath(const QString& oldKey, const QString& newKey)
{
    if (m_monitorsHash.contains(oldKey)) {
        Monitor* value = m_monitorsHash.value(oldKey);
        m_monitorsHash.remove(oldKey);
        m_monitorsHash.insert(newKey, value);
    }
    m_fileWatcher.removeFilePath(oldKey);
    m_fileWatcher.addFilePath(newKey);
}

QStringList Manager::monitorsOrder() const
{
    return m_monitorsOrder;
}

void Manager::setMonitorsOrder(const QStringList &newMonitorsOrder)
{
    if (m_monitorsOrder == newMonitorsOrder)
        return;

    m_monitorsOrder = newMonitorsOrder;

    emit monitorsOrderChanged();
}

bool Manager::hadInitErrors() const
{
    return m_error;
}
