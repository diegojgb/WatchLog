#include "WinFileManager.h"

FileStatus::FileStatus(const QString path)
    : filePath{path},
      exists{std::filesystem::exists(path.toStdString())}
{}

WinFileManager::WinFileManager(QObject *parent)
    : QObject{parent}
{
    connect(&m_winFileMonitor, &WinFileMonitor::changeFound, this, &WinFileManager::onChangeFound);
}

FileStatus *WinFileManager::findOrCreate(const QString &path)
{
    for (auto* file: m_fileList) {
        if (file->filePath == path)
            return file;
    }

    auto* fs = new FileStatus(path);
    m_fileList.append(fs);
    m_winFileMonitor.addFile(path);

    return fs;
}

FileStatus* WinFileManager::find(const QString &path)
{
    for (auto* file: m_fileList) {
        if (file->filePath == path)
            return file;
    }

    return nullptr;
}

void WinFileManager::onChangeFound(const QString &filePath, const Change type)
{
    bool removed = Change::Removed == type;

    for (auto* file: m_fileList) {
        if (file->filePath == filePath) {
            emit file->existsChanged(removed);
            return;
        }
    }

    Utils::throwError("Received change notification from untracked file: " + filePath.toStdString());
}
