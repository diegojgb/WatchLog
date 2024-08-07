#include "Manager.h"


Manager::Manager(QObject* parent, const json& data)
    : QObject{parent}, m_fileWatcher{this, m_monitors}, m_error{false},
      m_winFileManager{this, data.empty() ? Mode::Manual : toWinFileMode(Monitor::jsonGetValue<std::string>(data, "winFileMode", "Manual"))}
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
            Monitor* newMonitor = new Monitor(this, item, m_winFileManager);
            QString filePath = QString::fromStdString(item["filePath"].get<std::string>());

            m_monitors.insert(filePath, newMonitor);

            QObject::connect(newMonitor, &Monitor::filePathChangedOverload, &m_fileWatcher, &FileWatcher::changeFilePath);
            QObject::connect(newMonitor, &Monitor::monitorEnabled, this, &Manager::enableMonitor);
            QObject::connect(newMonitor, &Monitor::monitorDisabled, this, &Manager::disableMonitor);
        }

        m_fileWatcher.addAllMonitors();
    } catch(const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        m_error = true;
    }

    connect(&m_fileWatcher, &FileWatcher::checkFailed, this, &Manager::onCheckFailed);
}

void Manager::initTrayIcon(QObject* parent, QObject* root, HWND& hwnd)
{
    if (m_trayIconInitialized)
        Utils::throwError("Tray icon can only be initialized once.");

    m_trayIcon = new TrayIcon(parent, root, hwnd);

    connect(&m_fileWatcher, &FileWatcher::matchFound, m_trayIcon, &TrayIcon::sendNotification);
    connect(m_trayIcon, &TrayIcon::saveRequested, this, &Manager::updateJSON);
    connect(m_trayIcon, &TrayIcon::aboutClicked, this, &Manager::aboutClicked);

    m_trayIconInitialized = true;
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
    } else {
        Utils::throwError("Manager: Failed to open data.json for writing");
    }
}

bool Manager::addMonitor(const QString& name, const QString& filePath)
{
    if (m_monitors.contains(filePath))
        return false;

    Monitor* newMonitor = new Monitor(this, name, filePath, m_winFileManager);

    m_monitors.insert(filePath, newMonitor);

    QObject::connect(newMonitor, &Monitor::filePathChangedOverload, &m_fileWatcher, &FileWatcher::changeFilePath);
    QObject::connect(newMonitor, &Monitor::monitorEnabled, this, &Manager::enableMonitor);
    QObject::connect(newMonitor, &Monitor::monitorDisabled, this, &Manager::disableMonitor);

    return true;
}

void Manager::onCheckFailed(const QString& filePath)
{
    Utils::showInfo("Couldn't check the following file: " + filePath);

    auto* monitor = m_monitors.get(filePath);
    monitor->setFileError(true);
}

void Manager::checkFilesNow()
{
    m_winFileManager.manualCheckNow();
}

Mode Manager::toWinFileMode(const std::string mode)
{
    if (mode == "WinApi")
        return Mode::WinApi;
    if (mode == "Manual")
        return Mode::Manual;
    if (mode == "Mixed")
        return Mode::Mixed;

    Utils::throwError("Invalid WinFile mode value.");
    return Mode::Manual; // To prevent compiler warnings.
}

bool Manager::hadInitErrors() const
{
    return m_error;
}

MonitorCollection* Manager::monitors()
{
    return &m_monitors;
}

Settings* Manager::settings()
{
    return &m_settings;
}
