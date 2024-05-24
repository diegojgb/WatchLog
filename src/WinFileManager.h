#ifndef WINFILEMANAGER_H
#define WINFILEMANAGER_H

#include "WinFileMonitor.h"
#include "Utils.h"

#include <QObject>


struct FileStatus: public QObject
{
    Q_OBJECT

public:
    QString filePath;
    bool exists;

    explicit FileStatus(const QString path);

signals:
    void existsChanged(bool removed);
};


class WinFileManager: public QObject
{
    Q_OBJECT

public:
    explicit WinFileManager(QObject *parent);

    FileStatus* findOrCreate(const QString &path);

public slots:
    void onChangeFound(const QString &filePath, const Change type);

private:
    WinFileMonitor m_winFileMonitor;
    QList<FileStatus*> m_fileList;
};

#endif // WINFILEMANAGER_H
