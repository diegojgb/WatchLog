#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include "Monitor.h"

#include <QObject>
#include <QFileSystemWatcher>
#include <QHash>


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

private:
    QFileSystemWatcher m_watcher;
    const QHash<QString, Monitor*> &m_monitorsHash;
};

#endif // FILEWATCHER_H
