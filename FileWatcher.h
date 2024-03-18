#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include "Monitor.h"

#include <QObject>
#include <QFileSystemWatcher>
#include <QHash>
#include <windows.h>
#include <mmsystem.h>
#include <regex>


class FileWatcher: public QObject
{
    Q_OBJECT

public:
    explicit FileWatcher(QObject* parent, const QHash<QString, Monitor*> &monitorsHash);

public slots:
    void onFileChanged(const QString &path);
    void addFilePath(const QString &filePath);
    void removeFilePath(const QString &filePath);
    void addAllMonitors();
    void removeAllMonitors();

signals:
    void matchFound(const WinToastTemplate& templ);
    void fileReset();

private:
    QFileSystemWatcher m_watcher;
    const QHash<QString, Monitor*> &m_monitorsHash;
    TCHAR m_defaultSoundFilePath[MAX_PATH];
};

#endif // FILEWATCHER_H
