#include "Manager.h"


Manager::Manager(QObject *parent, const json &data)
    : QObject{parent}, m_fileWatcher{this, m_monitors}, m_error{false}
{
    if (data.empty())
        return;

    json monitorsData = Monitor::jsonFindByKey(data, "monitors");

    if (!monitorsData.is_array())
        Utils::throwError("Invalid monitors array in JSON");

    if  (monitorsData.size() == 0)
        return;

    try {
        for (const json& item: monitorsData) {
            Monitor* newMonitor = new Monitor(this, item);
            QString filePath = QString::fromStdString(item["filePath"].get<std::string>());

            m_monitors.insert(filePath, newMonitor);

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

json Manager::toJSON() const
{
    json obj;
    json monitorsArray = json::array();

    for (const MonitorData* monitorData: m_monitors.getList()) {
        auto* monitor = monitorData->monitor;
        monitorsArray.push_back(monitor->toJSON());
    }

    obj["monitors"] = monitorsArray;

    return obj;
}

void Manager::changeFilePath(const QString& oldKey, const QString& newKey)
{
    m_monitors.changeFilePath(oldKey, newKey);

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
        Utils::throwError("Manager: Failed to open data.json for writing");
    }
}

bool Manager::addMonitor(const QString &name, const QString &filePath)
{
    if (m_monitors.contains(filePath))
        return false;

    Monitor* newMonitor = new Monitor(this, name, filePath);

    m_monitors.insert(filePath, newMonitor);

    QObject::connect(newMonitor, &Monitor::filePathChangedOverload, this, &Manager::changeFilePath);
    QObject::connect(newMonitor, &Monitor::monitorEnabled, this, &Manager::enableMonitor);
    QObject::connect(newMonitor, &Monitor::monitorDisabled, this, &Manager::disableMonitor);
    QObject::connect(&m_fileWatcher, &FileWatcher::fileReset, newMonitor, &Monitor::startFile);

    return true;
}

bool Manager::hadInitErrors() const
{
    return m_error;
}

MonitorCollection* Manager::monitors()
{
    return &m_monitors;
}
