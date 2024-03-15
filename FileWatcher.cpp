#include "FileWatcher.h"

#include <regex>


FileWatcher::FileWatcher(QObject* parent, const QHash<QString, Monitor*> &monitorsHash)
    : QObject{parent}, m_monitorsHash{monitorsHash}
{
    connect(&m_watcher, &QFileSystemWatcher::fileChanged, this, &FileWatcher::onFileChanged);
}

void FileWatcher::addFilePath(const QString &filePath) {
    m_watcher.addPath(filePath);
}

void FileWatcher::addAllMonitors() {
    for (auto i = m_monitorsHash.cbegin(), end = m_monitorsHash.cend(); i != end; ++i) {
        if (i.value()->enabled())
            addFilePath(i.key());
    }
}

void FileWatcher::removeFilePath(const QString &filePath) {
    m_watcher.removePath(filePath);
}

void FileWatcher::removeAllMonitors() {

}

void FileWatcher::onFileChanged(const QString &path)
{
    Monitor* monitor = m_monitorsHash[path];

    if (monitor->m_file.is_open())
    {
        std::string line;
        std::smatch match;
        bool enabled = true;

        while (std::getline(monitor->m_file, line)) {

            for(const Notifier* notifier: monitor->m_enabledNotifiers) {
                if (enabled && std::regex_search(line, match, notifier->regex)) {
                    emit matchFound(notifier->templ);
                    qDebug() << "Found: " << match.str();
                    enabled = false;
                }
            }
        }

        if (!monitor->m_file.eof())
             throw std::runtime_error("Error reading file.");

        monitor->m_file.clear();
    } else {
        throw std::runtime_error("File's not open.");
    }
}
