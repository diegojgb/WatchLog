#include "FileWatcher.h"


FileWatcher::FileWatcher(QObject* parent, const MonitorCollection& monitors)
    : QObject{parent}, m_monitors{monitors}
{
    m_watcher = new FileChangeWorker();
    m_thread = new QThread(this);
    m_watcher->moveToThread(m_thread);

    connect(m_thread, &QThread::started, m_watcher, &FileChangeWorker::start);
    connect(qApp, &QCoreApplication::aboutToQuit, m_watcher, &FileChangeWorker::finish);

    connect(m_thread, &QThread::finished, m_thread, &QThread::deleteLater);
    connect(m_watcher, &FileChangeWorker::finished, m_watcher, &FileChangeWorker::deleteLater);
    connect(m_watcher, &FileChangeWorker::finished, m_thread, &QThread::quit, Qt::DirectConnection);

    connect(m_watcher, &FileChangeWorker::fileChanged, this, &FileWatcher::onFileChanged);

    m_thread->start();
}

FileWatcher::~FileWatcher()
{
    m_thread->wait();
}

void FileWatcher::addAllMonitors()
{
    for (const MonitorData* monitorData: m_monitors.getList()) {
        auto* monitor = monitorData->monitor;
        auto filePath = monitorData->filePath;

        if (monitor->enabled())
            addFilePath(filePath);
    }
}

void FileWatcher::addFilePath(const QString &filePath)
{
    QMetaObject::invokeMethod(m_watcher, "addPath", Qt::QueuedConnection,
                              Q_ARG(QString, filePath));
}

void FileWatcher::removeFilePath(const QString &filePath)
{
    QMetaObject::invokeMethod(m_watcher, "removePath", Qt::QueuedConnection,
                              Q_ARG(QString, filePath));
}

void FileWatcher::onFileChanged(const QString &path)
{
    // In case the file was removed and added back again immediately.
    // if (!m_watcher.files().contains(path)) {
    //     if (std::filesystem::exists(path.toStdString())) {
    //         addFilePath(path);
    //         emit fileReset(); // So the monitor reopens the file, to prevent any unexpected behavior.
    //         return;
    //     }
    // }

    Monitor* monitor = m_monitors.get(path);

    if (!monitor->m_file.is_open())
        Utils::throwError("FileWatcher: file's not open");

    std::string line;

    if (!std::getline(monitor->m_file, line)) { // In case weird modifications were made, and the cursor is broken.
        emit fileReset();
        return;
    }

    do {
        for (const Notifier* notifier: monitor->m_enabledNotifiers) {
            if (!std::regex_search(line, notifier->regex))
                continue;

            if (notifier->toastEnabled()) {
                emit matchFound(notifier->templ);
            } else {
                std::wstring playCommand;

                if (m_soundsHash.contains(notifier->soundPath())) {
                    playCommand = m_soundsHash[notifier->soundPath()];
                } else {
                    std::wstring soundAlias = L"NotificationSound" + std::to_wstring(m_soundsHash.size());
                    std::wstring command = L"open \"" + notifier->soundPath().toStdWString() + L"\" type waveaudio alias " + soundAlias;

                    if (mciSendString(command.c_str(), NULL, 0, NULL) != 0)
                        Utils::throwError("FileWatcher: Error opening sound file");

                    playCommand = L"play " + soundAlias + L" from 0";

                    m_soundsHash.insert(notifier->soundPath(), playCommand);
                }

                mciSendString(playCommand.c_str(), NULL, 0, NULL);
            }
            if (!monitor->manyPerUpdate) {
                monitor->m_file.seekg(0, std::ios::end);
                return;
            }
        }
    } while (std::getline(monitor->m_file, line));

    if (!monitor->m_file.eof())
        Utils::throwError("FileWatcher: error reading file");

    monitor->m_file.clear();
}
