#include "FileWatcher.h"


FileWatcher::FileWatcher(QObject* parent, const QHash<QString, Monitor*> &monitorsHash)
    : QObject{parent}, m_monitorsHash{monitorsHash}
{
    connect(&m_watcher, &QFileSystemWatcher::fileChanged, this, &FileWatcher::onFileChanged);

    TCHAR windir[MAX_PATH];

    if (GetWindowsDirectory(windir, MAX_PATH) == 0)
        throw std::runtime_error("FileWatcher: Error getting Windows directory.");

    if (PathCombine(m_defaultSoundFilePath, windir, TEXT("Media\\Windows Notify System Generic.wav")) == NULL)
        throw std::runtime_error("FileWatcher: Error getting Windows Notification Sound path.");

    std::wstring command = L"open \"" + std::wstring(m_defaultSoundFilePath) + L"\" type waveaudio alias NotificationSound";
    LPCWSTR lpcwCommand = command.c_str();

    if (mciSendString(lpcwCommand, NULL, 0, NULL) != 0)
        throw std::runtime_error("FileWatcher: Error opening sound file.");
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
        std::smatch match;
        bool enabled = true;

        if (!std::getline(monitor->m_file, line)) { // In case weird modifications were made, and the cursor is broken.
            emit fileReset();
            return;
        }

        do {
            for (const Notifier* notifier: monitor->m_enabledNotifiers) {
                if (enabled && std::regex_search(line, match, notifier->regex)) {
                    if (notifier->toastEnabled())
                        emit matchFound(notifier->templ);
                    else
                        // PlaySound(TEXT("Notification.Default"), NULL, SND_ASYNC);
                        // PlaySound(m_defaultSoundFilePath, NULL, SND_FILENAME | SND_ASYNC);
                        mciSendString(L"play NotificationSound from 0", NULL, 0, NULL);

                    qDebug() << "Found: " << match.str();
                    enabled = false;
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
