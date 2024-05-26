#include "FileChecker.h"


FileChecker::FileChecker(QObject *parent, const QList<FileStatus *> &fileList)
{
    m_worker = new FileCheckWorker(nullptr, fileList);
    m_thread = new QThread(this);
    m_worker->moveToThread(m_thread);

    connect(m_thread, &QThread::started, m_worker, &FileCheckWorker::start);
    connect(qApp, &QCoreApplication::aboutToQuit, m_worker, &FileCheckWorker::finish);

    connect(m_thread, &QThread::finished, m_thread, &QThread::deleteLater);
    connect(m_worker, &FileCheckWorker::finished, m_worker, &FileCheckWorker::deleteLater);
    connect(m_worker, &FileCheckWorker::finished, m_thread, &QThread::quit, Qt::DirectConnection);

    m_thread->start();
}

void FileChecker::checkNow()
{
    QMetaObject::invokeMethod(m_worker, "checkNow", Qt::QueuedConnection);
}
