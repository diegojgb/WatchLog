#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include "Monitor.h"
#include "Utils.h"

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

signals:
    void matchFound(const WinToastTemplate& templ);
    void fileReset();

private:
    QFileSystemWatcher m_watcher;
    QHash<QString, std::wstring> m_soundsHash;
    const QHash<QString, Monitor*> &m_monitorsHash;
};

#endif // FILEWATCHER_H
