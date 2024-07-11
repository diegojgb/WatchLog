#ifndef MULTIDIRMONITOR_H
#define MULTIDIRMONITOR_H

#include "SingleDirMonitor.h"

#include <QObject>

// Note:
// Due to race conditions and such, I decided not to implement the removal of directories with 0 files in them.
// Specially since this class isn't being used by default.

class MultiDirMonitor: public QObject
{
    Q_OBJECT

public:
    MultiDirMonitor(QObject* parent = nullptr);

    Q_INVOKABLE SingleDirMonitor* addDir(const QString& path);
    Q_INVOKABLE void addFile(const QString& path);
    Q_INVOKABLE void removeFile(const QString& path);

    void removeDir(const QString& path);

signals:
    void changeFound(const QString& filePath, const Change type);

public slots:
    void start();
    void onCdUpped(SingleDirMonitor* instance);
    void onClearForDeletion(SingleDirMonitor* instance);

private:
    QList<SingleDirMonitor*> m_dirs;

    void transferFiles(SingleDirMonitor* src, SingleDirMonitor* dst);
};

#endif // MULTIDIRMONITOR_H
