#include "MultiDirMonitor.h"

namespace fs = std::filesystem;


MultiDirMonitor::MultiDirMonitor(QObject* parent)
    : QObject{parent}
{}

SingleDirMonitor* MultiDirMonitor::addDir(const QString& path)
{
    auto* newDir = new SingleDirMonitor(this, path);
    QObject::connect(newDir, &SingleDirMonitor::changeFound, this, &MultiDirMonitor::changeFound);
    QObject::connect(newDir, &SingleDirMonitor::cdUpped, this, &MultiDirMonitor::onCdUpped);
    QObject::connect(newDir, &SingleDirMonitor::clearForDeletion, this, &MultiDirMonitor::onClearForDeletion);

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

void MultiDirMonitor::addFile(const QString& path)
{
    for (auto* monitor: m_dirs) {
        if (path.startsWith(monitor->getPath())) {
                monitor->addFile(path);
                return;
        }
    }

    for (auto* monitor: m_dirs) {
        auto pathDir = SingleDirMonitor::firstParent(path);

        if (monitor->getPath().startsWith(pathDir)) {
            bool success = monitor->cdUpTo(pathDir);

            if (!success)
                throw std::runtime_error(("Failed to cdUp from " + monitor->getPath() + "to " + pathDir).toStdString());

            onCdUpped(monitor);
            monitor->addFile(path);
            return;
        }
    }

    auto dirPath = QFileInfo(path).absolutePath();
    auto dir = addDir(dirPath);

    dir->addFile(path);
}

void MultiDirMonitor::removeFile(const QString& path)
{
    for (auto* monitor: m_dirs) {
        if (path.startsWith(monitor->getPath()))
            if (monitor->removeFile(path))
                break;
    }
}

void MultiDirMonitor::start()
{
    for (auto* dir: m_dirs)
        dir->startWatching();
}

void MultiDirMonitor::onCdUpped(SingleDirMonitor* instance)
{
    for (auto* monitor: m_dirs) {
        if (instance == monitor)
            continue;

        if (monitor->getPath().startsWith(instance->getPath())) {
            transferFiles(monitor, instance);
        }
    }
}

void MultiDirMonitor::onClearForDeletion(SingleDirMonitor* instance)
{
    delete instance;
}

void MultiDirMonitor::transferFiles(SingleDirMonitor* src, SingleDirMonitor* dst)
{
    for (const auto& file: src->getFiles())
        dst->addFile(file);

    m_dirs.removeAll(src);
    src->deleteLater();
}
