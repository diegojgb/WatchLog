#ifndef MULTIDIRMONITOR_H
#define MULTIDIRMONITOR_H

#include "SingleDirMonitor.h"

#include <QObject>

class MultiDirMonitor: public QObject
{
    Q_OBJECT

public:
    MultiDirMonitor(QObject *parent = nullptr);

signals:
    void changeFound(const QString& filePath, const Change type);

public slots:
    SingleDirMonitor* addDir(const QString& path);
    void removeDir(const QString& path);
    void addFile(const QString& path);
    void removeFile(const QString& path);
    void start();
    void onCdUpped(SingleDirMonitor* instance);
    void onClearForDeletion(SingleDirMonitor* instance);

private:
    QList<SingleDirMonitor*> m_dirs;

    void transferFiles(SingleDirMonitor* src, SingleDirMonitor* dst);
};

#endif // MULTIDIRMONITOR_H
