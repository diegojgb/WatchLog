#include "Manager.h"


Manager::Manager(QObject *parent, const json &data)
    : QObject{parent}, m_fileWatcher{this, m_monitorsHash}, m_error{false}
{
    if (data.empty()) {
        QMessageBox::critical(nullptr, tr("WatchLog"), tr("Error: The JSON file is empty"));
        throw std::runtime_error("Error: The JSON file is empty");
    }

    json monitorsData = Monitor::jsonFindByKey(data, "monitors");

    if (!monitorsData.is_array() || monitorsData.size() == 0) {
        QMessageBox::critical(nullptr, tr("WatchLog"), tr("Error: Invalid monitors array in JSON"));
        throw std::runtime_error("Error: Invalid monitors array in JSON");
    }

    try {
        for (const json& item: monitorsData) {
            Monitor* newMonitor = new Monitor(this, item);
            QString filePath = QString::fromStdString(item["filePath"].get<std::string>());

            m_monitorsHash.insert(filePath, newMonitor);
            m_monitorsOrder.append(filePath);

            QObject::connect(newMonitor, &Monitor::filePathChangedOverload, this, &Manager::changeFilePath);
            QObject::connect(newMonitor, &Monitor::monitorEnabled, this, &Manager::enableMonitor);
            QObject::connect(newMonitor, &Monitor::monitorDisabled, this, &Manager::disableMonitor);
            QObject::connect(&m_fileWatcher, &FileWatcher::fileReset, newMonitor, &Monitor::startFile);
        }

        m_fileWatcher.addAllMonitors();
    }
    catch(const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        m_error = true;
    }
}

Monitor* Manager::hashGet(const QString& key)
{
    return m_monitorsHash[key];
}

json Manager::toJSON() const
{
    json rootArray = json::array();

    for (const QString& monitorPath: m_monitorsOrder) {
        rootArray.push_back(m_monitorsHash[monitorPath]->toJSON());
    }

    return rootArray;
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

void Manager::disableMonitor(const Monitor* monitor)
{
    m_fileWatcher.removeFilePath(monitor->filePath());
}

void Manager::enableMonitor(const Monitor* monitor)
{
    m_fileWatcher.addFilePath(monitor->filePath());
}

void Manager::updateJSON() const
{
    json obj = toJSON();
    std::ofstream outFile("data.json");

    if (outFile.is_open()) {
        outFile << obj.dump(4); // dump(4) prettifies it with 4 spaces indentation.
        outFile.close();
        qDebug() << "JSON data saved to data.json";
    } else {
        std::cerr << "Failed to open output.json for writing\n";
        throw std::runtime_error("Manager: Failed to open data.json for writing");
    }
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
