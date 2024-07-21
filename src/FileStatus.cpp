#include "FileStatus.h"


FileStatus::FileStatus(const QString path, QObject* parent)
    : QObject{parent},
      m_filePath{path},
      m_exists{std::filesystem::exists(path.toStdWString())}
{}

const QString& FileStatus::getFilePath() const
{
    return m_filePath;
}

const bool FileStatus::getExists() const
{
    return m_exists;
}

void FileStatus::updateExists()
{
    setExists(std::filesystem::exists(m_filePath.toStdWString()));
}

void FileStatus::setExists(bool newExists)
{
    if (m_exists == newExists)
        return;

    m_exists = newExists;

    emit statusChanged(!newExists);
}

void FileStatus::connectNotify(const QMetaMethod& signal)
{
    if (signal == QMetaMethod::fromSignal(&FileStatus::statusChanged))
        m_connectedSlots++;
}

void FileStatus::disconnectNotify(const QMetaMethod& signal)
{
    if (signal == QMetaMethod::fromSignal(&FileStatus::statusChanged))
        m_connectedSlots--;

    if (m_connectedSlots <= 0)
        emit allSlotsDisconnected(this);
}
