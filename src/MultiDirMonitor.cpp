#include "MultiDirMonitor.h"

namespace fs = std::filesystem;


MultiDirMonitor::MultiDirMonitor(QObject *parent)
    : QObject{parent}
{}

SingleDirMonitor* MultiDirMonitor::addDir(const QString& path)
{
    qDebug() << "Added dir: " << path;
    auto* newDir = new SingleDirMonitor(this, path);
    QObject::connect(newDir, &SingleDirMonitor::changeFound, this, &MultiDirMonitor::changeFound);
    QObject::connect(newDir, &SingleDirMonitor::cdUpped, this, &MultiDirMonitor::onCdUpped);

    m_dirs.append(newDir);
    newDir->startWatching();

    return newDir;
}

void MultiDirMonitor::removeDir(const QString& path)
{
    for (int i = 0; i < m_dirs.size(); i++) {
        if (m_dirs[i]->getPath() == path) {
            delete m_dirs.takeAt(i);
            return;
        }
    }
}

void MultiDirMonitor::addFile(const QString &path)
{
    for (auto* monitor: m_dirs) {
        if (path.startsWith(monitor->getPath())) {
                monitor->addFile(path);
                qDebug() << "Added file to existent dir: " << path;
                return;
        }

        auto pathDir = QFileInfo(path).absolutePath();
        if (monitor->getPath().startsWith(pathDir)) {
            bool success = monitor->cdUpTo(pathDir);

            if (!success)
                throw std::runtime_error(("Failed to cdUp from " + monitor->getPath() + "to " + pathDir).toStdString());

            qDebug() << "CdUpped and added file to existent dir: " << pathDir;
            monitor->addFile(path);
            return;
        }
    }

    auto dirPath = QFileInfo(path).absolutePath();
    auto dir = addDir(dirPath);

    dir->addFile(path);
    qDebug() << "Added file: " << path;
}

void MultiDirMonitor::start()
{
    for (auto* dir: m_dirs)
        dir->startWatching();
}

void MultiDirMonitor::onCdUpped(SingleDirMonitor *instance)
{
    for (auto* monitor: m_dirs) {
        if (instance == monitor)
            continue;

        if (monitor->getPath().startsWith(instance->getPath())) {
            transferFiles(monitor, instance);
            qDebug() << "Transferred files from: " << monitor->getPath() << "to " << instance->getPath();
        }
    }
}

void MultiDirMonitor::transferFiles(SingleDirMonitor* src, SingleDirMonitor* dst)
{
    for (const auto& file: src->getFiles())
        dst->addFile(file);

    m_dirs.removeAll(src);
    delete src;
}
