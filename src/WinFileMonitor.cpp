#include "WinFileMonitor.h"


namespace fs = std::filesystem;


WinFileMonitor::WinFileMonitor(QObject *parent)
    : QObject{parent}
{
    m_worker = new MultiDirMonitor();
    m_thread = new QThread(this);
    m_worker->moveToThread(m_thread);

    connect(m_thread, &QThread::started, m_worker, &MultiDirMonitor::start);

    connect(m_worker, &MultiDirMonitor::changeFound, this, &WinFileMonitor::onChangeFound);

    connect(qApp, &QCoreApplication::aboutToQuit, m_thread, &QThread::quit, Qt::DirectConnection);
    connect(m_thread, &QThread::finished, m_thread, &QThread::deleteLater);

    m_thread->start();
}

void WinFileMonitor::addFile(const QString &path)
{
    QMetaObject::invokeMethod(m_worker, "addFile", Qt::QueuedConnection,
                              Q_ARG(QString, path));
}

void WinFileMonitor::onChangeFound(const QString &filePath, const Change type)
{
    switch(type) {
        case Change::Added:
            qDebug() << "Added: " << filePath;
            break;

        case Change::Removed:
            qDebug() << "Removed: " << filePath;
            break;
    }
}



