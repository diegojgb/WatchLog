#include "SingleDirMonitor.h"

namespace fs = std::filesystem;


SingleDirMonitor::SingleDirMonitor(QObject* parent, const QString& path)
    : QObject{parent},
      m_qPath{firstAvailable(path)},
      m_qParentPath{firstParent(m_qPath)},
      m_wParentPath{m_qParentPath.toStdWString()}
{
    m_changeBuff.resize(BUFFER_SIZE);
    startFile();
    m_overlapped.hEvent = this;
}

SingleDirMonitor::~SingleDirMonitor()
{
    if (!m_pendingDelete)
        CancelIoEx(m_dir, NULL);

    CloseHandle(m_dir);
}

void SingleDirMonitor::startWatching()
{
     // May need more bits.
    BOOL success = ReadDirectoryChangesW(
                m_dir, &m_changeBuff[0],
                m_changeBuff.size(), TRUE,
                FILE_NOTIFY_CHANGE_FILE_NAME  |
                FILE_NOTIFY_CHANGE_DIR_NAME   |
                FILE_NOTIFY_CHANGE_LAST_WRITE,
                NULL, &m_overlapped, &onDirectoryChange);

    if (!success) {
        DWORD error = GetLastError();
        std::cerr << "ReadDirectoryChangesW failed with error: " << std::to_string(error) << std::endl;
    }
}

// static
VOID CALLBACK SingleDirMonitor::onDirectoryChange(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped)
{
    SingleDirMonitor* instance = (SingleDirMonitor*)lpOverlapped->hEvent;

    if (instance->hasPendingDelete()) {
        emit instance->clearForDeletion(instance);
        return;
    }

    if (instance->clearPending())
        return;

    if (dwErrorCode == ERROR_OPERATION_ABORTED) {
        std::cerr <<  "Directory I/O operation aborted." << std::endl;
        return;
    }

    instance->processNotification();
    instance->startWatching();
}

void SingleDirMonitor::processNotification()
{
    FILE_NOTIFY_INFORMATION* event = (FILE_NOTIFY_INFORMATION*)m_changeBuff.data();
    bool dirRemoved = false;

    for (;;) {
        DWORD name_len = event->FileNameLength / sizeof(wchar_t);
        std::wstring fileName(event->FileName, name_len);

        switch(event->Action) {
            case FILE_ACTION_ADDED:
            case FILE_ACTION_RENAMED_NEW_NAME:
                onAdded(parseFileName(fileName));
                break;

            case FILE_ACTION_REMOVED:
            case FILE_ACTION_RENAMED_OLD_NAME:
                dirRemoved = onRemoved(parseFileName(fileName));
                break;
        }

        // Are there more events to handle?
        if (event->NextEntryOffset) {
            *((uint8_t**)&event) += event->NextEntryOffset;
        } else {
            if (dirRemoved) {
                cdUp();
            }

            break;
        }
    }
}

bool SingleDirMonitor::onRemoved(const QString& filePath)
{
    if (isMonitored(filePath))
        emit changeFound(filePath, Change::Removed);

    return wasDirRemoved(filePath);
}

void SingleDirMonitor::onAdded(const QString& filePath)
{
    if (isMonitored(filePath))
        emit changeFound(filePath, Change::Added);
}

bool SingleDirMonitor::wasDirRemoved(const QString& filePath)
{
    return filePath == m_qPath;
}

bool SingleDirMonitor::clearPending()
{
    if (m_pending) {
        m_pending = false;
        CloseHandle(m_dir);
        startFile();
        startWatching();

        return true;
    }

    return false;
}

void SingleDirMonitor::startFile()
{
    m_dir = CreateFile(m_wParentPath.c_str(),
                             FILE_LIST_DIRECTORY,
                             FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                             NULL,
                             OPEN_EXISTING,
                             FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
                             NULL);

    if (m_dir == INVALID_HANDLE_VALUE) {
        DWORD error = GetLastError();
        std::cerr << "Failed to open directory: " << std::to_string(error) << std::endl;
    }
}

void SingleDirMonitor::resetFile()
{
    m_pending = true;
    CancelIoEx(m_dir, NULL);
}

void SingleDirMonitor::cdUp()
{
    m_qPath = m_qParentPath;
    m_qParentPath = firstParent(m_qParentPath);
    m_wParentPath = m_qParentPath.toStdWString();
    resetFile();

    emit cdUpped(this);
}

bool SingleDirMonitor::cdUpTo(const QString& path)
{
    if (!m_qPath.startsWith(path))
        return false;

    m_qPath = path;
    m_qParentPath = firstParent(path);
    m_wParentPath = m_qParentPath.toStdWString();
    resetFile();

    return true;
}

void SingleDirMonitor::deleteLater()
{
    m_pendingDelete = true;
    CancelIoEx(m_dir, NULL);
}

bool SingleDirMonitor::hasPendingDelete()
{
    return m_pendingDelete;
}

QString SingleDirMonitor::parseFileName(const std::wstring& fileName)
{
    auto filePath = QString::fromStdWString(fileName);
    filePath = QDir(m_qParentPath).filePath(filePath);

    return QDir::cleanPath(filePath);
}

const QString& SingleDirMonitor::getPath() const
{
    return m_qPath;
}

const QList<QString>& SingleDirMonitor::getFiles() const
{
    return m_files;
}

void SingleDirMonitor::addFile(const QString& filePath)
{
    m_files.append(filePath);
}

bool SingleDirMonitor::removeFile(const QString& filePath)
{
    return m_files.removeAll(filePath) > 0;
}

bool SingleDirMonitor::isMonitored(const QString& path)
{
    return m_files.contains(path);
}

// static
QString SingleDirMonitor::firstAvailable(const QString& filePath)
{
    QFileInfo dir(filePath);

    while (!dir.exists() && !dir.isRoot()) {
        dir.setFile(dir.absolutePath());
    }

    // If dir is root and doesn't exist.
    if (!dir.exists()) {
        std::cerr <<  "Root directory doesn't exist" << std::endl;
    }

    return dir.absoluteFilePath();
}

// static
QString SingleDirMonitor::firstParent(const QString& filePath)
{
    QFileInfo dir(filePath);

    return firstAvailable(dir.absolutePath());
}
