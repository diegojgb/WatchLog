#include "WinFileManager.h"

FileStatus::FileStatus(const QString path)
    : m_filePath{path},
      m_exists{std::filesystem::exists(path.toStdString())}
{}

const QString &FileStatus::getFilePath() const
{
    return m_filePath;
}

const bool FileStatus::getExists() const
{
    return m_exists;
}

void FileStatus::updateExists()
{
    setExists(std::filesystem::exists(m_filePath.toStdString()));
}

void FileStatus::setExists(bool newExists)
{
    if (m_exists == newExists)
        return;

    m_exists = newExists;

    emit statusChanged(!newExists);
}

WinFileManager::WinFileManager(QObject *parent, const Mode mode)
    : QObject{parent},
      m_mode{mode}
{
    if (mode != Mode::Manual) {
        m_winFileMonitor = new WinFileMonitor(this);
        connect(m_winFileMonitor, &WinFileMonitor::changeFound, this, &WinFileManager::onChangeFound);
    }
}

FileStatus *WinFileManager::findOrCreate(const QString &path)
{
    for (auto* file: m_fileList) {
        if (file->getFilePath() == path)
            return file;
    }

    auto* fs = new FileStatus(path);
    m_fileList.append(fs);

    if (m_mode != Mode::Manual)
        m_winFileMonitor->addFile(path);

    return fs;
}

FileStatus* WinFileManager::find(const QString &path)
{
    for (auto* file: m_fileList) {
        if (file->getFilePath() == path)
            return file;
    }

    return nullptr;
}

void WinFileManager::onChangeFound(const QString &filePath, const Change type)
{
    bool newExists = Change::Added == type;

    for (auto* file: m_fileList) {
        if (file->getFilePath() == filePath) {
            file->setExists(newExists);
            return;
        }
    }

    Utils::throwError("Received change notification from untracked file: " + filePath.toStdString());
}
