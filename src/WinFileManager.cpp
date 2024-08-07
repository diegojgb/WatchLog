#include "WinFileManager.h"


WinFileManager::WinFileManager(QObject* parent, const Mode mode)
    : QObject{parent},
      m_mode{mode}
{
    if (mode != Mode::Manual) {
        m_winFileMonitor = new WinFileMonitor(this);
        connect(m_winFileMonitor, &WinFileMonitor::changeFound, this, &WinFileManager::onChangeFound);
    }

    if (mode != Mode::WinApi) {
        m_fileChecker = new FileChecker(this, m_fileList);
    }
}

FileStatus* WinFileManager::findOrCreate(const QString& path)
{
    for (auto* file: m_fileList) {
        if (file->getFilePath() == path)
            return file;
    }

    auto* fs = new FileStatus(path);
    m_fileList.append(fs);

    connect(fs, &FileStatus::allSlotsDisconnected, this, &WinFileManager::onAllSlotsDisconnected);

    if (m_winFileMonitor != nullptr)
        m_winFileMonitor->addFile(path);

    return fs;
}

FileStatus* WinFileManager::find(const QString& path)
{
    for (auto* file: m_fileList) {
        if (file->getFilePath() == path)
            return file;
    }

    return nullptr;
}

void WinFileManager::manualCheckNow()
{
    if (m_fileChecker == nullptr) {
        Utils::showInfo("Error: manual mode is disabled. Can't perform a manual check.");
        return;
    }

    m_fileChecker->checkNow();
}

void WinFileManager::onChangeFound(const QString& filePath, const Change type)
{
    bool newExists = Change::Added == type;

    for (auto* file: m_fileList) {
        if (file->getFilePath() == filePath) {
            file->setExists(newExists);
            return;
        }
    }

    Utils::throwError("Received change notification from untracked file: " + filePath);
}

void WinFileManager::onAllSlotsDisconnected(FileStatus* instance)
{
    m_fileList.removeAll(instance);

    if (m_winFileMonitor != nullptr)
        m_winFileMonitor->removeFile(instance->getFilePath());

    instance->deleteLater();
}
