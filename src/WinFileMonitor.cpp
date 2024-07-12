#include "WinFileMonitor.h"


namespace fs = std::filesystem;


WinFileMonitor::WinFileMonitor(QObject* parent)
    : QObject{parent},
      m_worker{new MultiDirMonitor()},
      m_thread{new QThread(this)}
{
    m_worker->moveToThread(m_thread);

    connect(m_thread, &QThread::started, m_worker, &MultiDirMonitor::start);

    connect(m_worker, &MultiDirMonitor::changeFound, this, &WinFileMonitor::changeFound);

    connect(qApp, &QCoreApplication::aboutToQuit, m_thread, &QThread::quit, Qt::DirectConnection);
    connect(m_thread, &QThread::finished, m_thread, &QThread::deleteLater);

    m_thread->start();
}

void WinFileMonitor::addFile(const QString& filePath)
{
    QMetaObject::invokeMethod(m_worker, "addFile", Qt::QueuedConnection,
                              Q_ARG(QString, filePath));
}

void WinFileMonitor::removeFile(const QString& filePath)
{
    QMetaObject::invokeMethod(m_worker, "removeFile", Qt::QueuedConnection,
                              Q_ARG(QString, filePath));
}
