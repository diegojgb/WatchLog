#include "FileCheckWorker.h"

FileCheckWorker::FileCheckWorker(QObject* parent, const QList<FileStatus*>& fileList)
    : QObject{parent}, m_fileList{fileList}
{}

void FileCheckWorker::start()
{
    if (!m_running) {
        m_timer = startTimer(m_pollingRate);
        m_running = true;
    }
}

void FileCheckWorker::stop()
{
    if (m_running) {
        killTimer(m_timer);
        m_running = false;
    }
}

void FileCheckWorker::finish()
{
    stop();
    emit finished();
}

void FileCheckWorker::checkNow()
{
    checkAll();
    stop();
    start();
}

void FileCheckWorker::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == m_timer) {
        checkAll();
    }
}

void FileCheckWorker::checkAll()
{
    for (auto* fileStatus: m_fileList) {
        fileStatus->updateExists();
    }
}
