#include "FileWatcher.h"


FileWatcher::FileWatcher(QObject* parent, const QHash<QString, Monitor*> &monitorsHash)
    : QObject{parent}, m_monitorsHash{monitorsHash}
{
    connect(&m_watcher, &QFileSystemWatcher::fileChanged, this, &FileWatcher::onFileChanged);
}

void FileWatcher::addAllMonitors()
{
    for (auto i = m_monitorsHash.cbegin(), end = m_monitorsHash.cend(); i != end; ++i) {
        if (i.value()->enabled())
            addFilePath(i.key());
    }
}

void FileWatcher::addFilePath(const QString &filePath)
{
    m_watcher.addPath(filePath);
}

void FileWatcher::removeFilePath(const QString &filePath)
{
    m_watcher.removePath(filePath);
}

void FileWatcher::removeAllMonitors() {

}

void FileWatcher::onFileChanged(const QString &path)
{
    // In case the file was removed and added back again immediately.
    if (!m_watcher.files().contains(path)) {
        if (std::filesystem::exists(path.toStdString())) {
            addFilePath(path);
            emit fileReset(); // So the monitor reopens the file, to prevent any unexpected behavior.
            return;
        }
    }

    Monitor* monitor = m_monitorsHash[path];

    if (monitor->m_file.is_open()) {
        std::string line;

        if (!std::getline(monitor->m_file, line)) { // In case weird modifications were made, and the cursor is broken.
            emit fileReset();
            return;
        }

        do {
            for (const Notifier* notifier: monitor->m_enabledNotifiers) {
                if (std::regex_search(line, notifier->regex)) {
                    if (notifier->toastEnabled())
                        emit matchFound(notifier->templ);
                    else {
                        std::wstring playCommand;

                        if (m_soundsHash.contains(notifier->soundPath())) {
                            playCommand = m_soundsHash[notifier->soundPath()];
                        } else {
                            std::wstring soundAlias = L"NotificationSound" + std::to_wstring(m_soundsHash.size());
                            std::wstring command = L"open \"" + notifier->soundPath().toStdWString() + L"\" type waveaudio alias " + soundAlias;

                            if (mciSendString(command.c_str(), NULL, 0, NULL) != 0)
                                throw std::runtime_error("FileWatcher: Error opening sound file.");

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
            }
        } while (std::getline(monitor->m_file, line));

        if (!monitor->m_file.eof())
             throw std::runtime_error("Error reading file.");

        monitor->m_file.clear();
    } else {
        throw std::runtime_error("File's not open.");
    }
}
