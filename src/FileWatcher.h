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

    void addAllMonitors();

    Q_INVOKABLE void addFilePath(const QString& filePath);
    Q_INVOKABLE void removeFilePath(const QString& filePath);

public slots:
    void onFileChanged(FileData* fileData);
    void changeFilePath(const QString& oldKey, const QString& newKey);

signals:
    void matchFound(const WinToastTemplate& templ);
    void checkFailed(const QString& filePath);

private:
    QThread* m_thread{};
    FileChangeWorker* m_watcher{};
    QHash<QString, std::wstring> m_soundsHash;
    const MonitorCollection& m_monitors;
};

#endif // FILEWATCHER_H
