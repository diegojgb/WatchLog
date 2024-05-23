#ifndef WINFILEMONITOR_H
#define WINFILEMONITOR_H

#include "MultiDirMonitor.h"

#include <QCoreApplication>
#include <QObject>
#include <QThread>
#include <QFileInfo>
#include <QDir>


class WinFileMonitor : public QObject
{
    Q_OBJECT
public:
    explicit WinFileMonitor(QObject *parent = nullptr);

    void addFile(const QString &filePath);
    void removeFile(const QString &filePath); // Por implementar

signals:

public slots:
    void onChangeFound(const QString& filePath, const Change type);

private:
    QThread* m_thread;
    MultiDirMonitor* m_worker;
};

#endif // WINFILEMONITOR_H
