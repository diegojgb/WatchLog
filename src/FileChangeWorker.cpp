#include "FileChangeWorker.h"

FileData::FileData(const QString path)
    : filePath{path},
      hFile{FileChangeWorker::getHandle(path)}
{
    GetFileTime(hFile, NULL, NULL, &lastWriteTime);
}

FileChangeWorker::FileChangeWorker(QObject *parent)
    : QObject{parent}
{
    m_qWatcher = new QFileSystemWatcher();
    connect(m_qWatcher, &QFileSystemWatcher::fileChanged, this, &FileChangeWorker::fileChanged);
}

FileChangeWorker::~FileChangeWorker()
{
    for (FileData* fileData: m_files)
        delete fileData;

    m_qWatcher->deleteLater();
}

HANDLE FileChangeWorker::getHandle(const QString &filePath)
{
    HANDLE hFile = CreateFile(filePath.toStdWString().c_str(), GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL, NULL);

    return hFile;
}

void FileChangeWorker::checkAll()
{
    for (FileData* file: m_files) {
        checkOne(file);
    }
}

bool FileChangeWorker::checkOne(FileData* file)
{
    FILETIME newTime;

    if (!GetFileTime(file->hFile, NULL, NULL, &newTime))
        return false;

    if (CompareFileTime(&file->lastWriteTime, &newTime) != 0) {
        file->lastWriteTime = newTime;
        emit fileChanged(file->filePath);
    }

    return true;
}

void FileChangeWorker::addPath(const QString &filePath)
{
    m_qWatcher->addPath(filePath);
    m_files.append(new FileData(filePath));
}

void FileChangeWorker::removePath(const QString &filePath)
{
    m_qWatcher->removePath(filePath);

    // Remove from m_files list
    for (int i = 0; i < m_files.size(); i++) {
        if (m_files[i]->filePath == filePath) {
            delete m_files.takeAt(i);
            break;
        }
    }
}

void FileChangeWorker::start()
{
    if (!m_running) {
        m_timer = startTimer(m_pollingRate);
        m_running = true;
    }
}

void FileChangeWorker::stop()
{
    if (m_running) {
        killTimer(m_timer);
        m_running = false;
    }
}

void FileChangeWorker::finish()
{
    stop();
    emit finished();
}

void FileChangeWorker::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == m_timer) {
        checkAll();
    }
}

int FileChangeWorker::pollingRate() const
{
    return m_pollingRate;
}

void FileChangeWorker::setPollingRate(int newPollingRate)
{
    if (m_pollingRate == newPollingRate)
        return;

    m_pollingRate = newPollingRate;

    emit pollingRateChanged();
}
