#include "FileStatus.h"


FileStatus::FileStatus(const QString path, QObject *parent)
    : QObject{parent},
      m_filePath{path},
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

void FileStatus::disconnectNotify(const QMetaMethod &signal)
{
    if (!isSignalConnected(signal))
        emit allSlotsDisconnected(this);
}
