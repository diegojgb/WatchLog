#ifndef FILECHANGEWORKER_H
#define FILECHANGEWORKER_H

#include "Utils.h"

#include <QObject>
#include <Windows.h>
#include <QFileSystemWatcher>
#include <QTimerEvent>
#include <QDebug>
#include <fstream>

struct FileData {
    QString filePath;
    HANDLE hFile;
    FILETIME lastWriteTime;
    std::ifstream file;

    explicit FileData(const QString path);
    ~FileData();

    bool hasTimeChanged();
    bool updateWriteTime();
    void startFile();
};

class FileChangeWorker : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int pollingRate READ pollingRate WRITE setPollingRate NOTIFY pollingRateChanged FINAL)

public:
    explicit FileChangeWorker(QObject *parent = nullptr);
    ~FileChangeWorker();

    static HANDLE getHandle(const QString &filePath);

    int pollingRate() const;
    void setPollingRate(int newPollingRate);

public slots:
    void addPath(const QString &filePath);
    void removePath(const QString &filePath);
    void start();
    void stop();
    void finish();

signals:
    void fileChanged(FileData* fileData);
    void finished();
    void pollingRateChanged();

private:
    QList<FileData*> m_files;
    QFileSystemWatcher* m_qWatcher;

    int m_timer;
    int m_pollingRate = 200;
    bool m_running = false;

    void timerEvent(QTimerEvent* event) override;

    void checkAll();
    void pathToSignal(const QString& path);
};

#endif // FILECHANGEWORKER_H
