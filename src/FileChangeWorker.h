#ifndef FILECHANGEWORKER_H
#define FILECHANGEWORKER_H

#include "Utils.h"

#include <QObject>
#include <Windows.h>
#include <QFileSystemWatcher>
#include <QTimerEvent>
#include <QDebug>
#include <fstream>


struct FileData: public QObject
{
    Q_OBJECT

public:
    static FILETIME timeAux; // To prevent the declaration of a new time variable in each polling iteration.

    QString filePath;
    HANDLE hFile;
    FILETIME lastWriteTime;
    std::ifstream file;

    explicit FileData(const QString path);
    ~FileData();

    bool isFileTimeDiff();
    bool saveCurTime(); // Returns false if it is same as current.
    void startFile();

signals:
    void checkFailed(const QString& filePath);
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
    void removeFromPolling(const QString &filePath);
    void onCheckFailed(const QString &filePath);
    void start();
    void stop();
    void finish();

signals:
    void fileChanged(FileData* fileData);
    void finished();
    void checkFailed(const QString& filePath);

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
