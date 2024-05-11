#include "FileChangeWorker.h"

FILETIME FileData::timeAux;

FileData::FileData(const QString path)
    : filePath{path},
      hFile{FileChangeWorker::getHandle(path)}
{
    GetFileTime(hFile, NULL, NULL, &lastWriteTime);
    startFile();
}

FileData::~FileData()
{
    file.close();
}

bool FileData::isFileTimeDiff()
{
    if (!GetFileTime(hFile, NULL, NULL, &timeAux)) {
        Utils::showInfo("Couldn't check the following file: " + filePath.toStdString());
        emit checkFailed(filePath);
    }

    return CompareFileTime(&lastWriteTime, &timeAux) != 0;
}

bool FileData::saveCurTime()
{
    if (!GetFileTime(hFile, NULL, NULL, &timeAux)) {
        Utils::showInfo("Couldn't check the following file: " + filePath.toStdString());
        emit checkFailed(filePath);
    }

    if (CompareFileTime(&lastWriteTime, &timeAux) != 0) {
        lastWriteTime = timeAux;
        return true;
    }

    return false;
}

void FileData::startFile()
{
    if (file.is_open())
        file.close();

    file = std::ifstream(filePath.toStdString());

    if (!file.is_open())
        Utils::throwError("Error opening file: " + filePath.toStdString());

    file.seekg(0, std::ios::end);
}

FileChangeWorker::FileChangeWorker(QObject *parent)
    : QObject{parent}
{
    m_qWatcher = new QFileSystemWatcher();
    connect(m_qWatcher, &QFileSystemWatcher::fileChanged, this, &FileChangeWorker::pathToSignal);
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
    for (FileData* fileData: m_files) {
        if (fileData->isFileTimeDiff())
            emit fileChanged(fileData);
    }
}

void FileChangeWorker::pathToSignal(const QString& path)
{
    for (FileData* fileData: m_files) {
        if (fileData->filePath == path) {
            emit fileChanged(fileData);
            return;
        }
    }
}

void FileChangeWorker::addPath(const QString &filePath)
{
    m_qWatcher->addPath(filePath);
    auto* fileData = new FileData(filePath);
    m_files.append(fileData);

    connect(fileData, &FileData::checkFailed, this, &FileChangeWorker::checkFailed);
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
