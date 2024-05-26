#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include "MonitorCollection.h"
#include "FileChangeWorker.h"

#include <QObject>
#include <QFileSystemWatcher>
#include <QCoreApplication>
#include <QHash>
#include <QThread>
#include <windows.h>
#include <mmsystem.h>


class FileWatcher: public QObject
{
    Q_OBJECT

public:
    explicit FileWatcher(QObject* parent, const MonitorCollection& monitors);
    ~FileWatcher();

public slots:
    void onFileChanged(FileData* fileData);
    void addFilePath(const QString &filePath);
    void removeFilePath(const QString &filePath);
    void changeFilePath(const QString &oldKey, const QString &newKey);
    void addAllMonitors();

signals:
    void matchFound(const WinToastTemplate& templ);
    void fileReset();
    void checkFailed(const QString& filePath);

private:
    QThread* m_thread;
    FileChangeWorker* m_watcher;
    QHash<QString, std::wstring> m_soundsHash;
    const MonitorCollection& m_monitors;
};

#endif // FILEWATCHER_H
