#ifndef FILECHANGEWORKER_H
#define FILECHANGEWORKER_H

#include "Utils.h"

#include <QObject>
#include <Windows.h>
#include <QFileSystemWatcher>
#include <QTimerEvent>
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


class FileChangeWorker: public QObject
{
    Q_OBJECT

    Q_PROPERTY(int pollingRate READ pollingRate WRITE setPollingRate NOTIFY pollingRateChanged FINAL)

public:
    explicit FileChangeWorker(QObject* parent = nullptr);
    ~FileChangeWorker();

    static HANDLE getHandle(const QString& filePath);

    int pollingRate() const;
    void setPollingRate(int newPollingRate);

    Q_INVOKABLE void addPath(const QString& filePath);
    Q_INVOKABLE void removePath(const QString& filePath);
    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();

public slots:
    void onCheckFailed(const QString& filePath);
    void finish();

signals:
    void fileChanged(FileData* fileData);
    void checkFailed(const QString& filePath);
    void finished();

    void pollingRateChanged();

private:
    QList<FileData*> m_files;
    QFileSystemWatcher* m_qWatcher{};

    int m_timer{};
    int m_pollingRate = 200;
    bool m_running = false;

    void timerEvent(QTimerEvent* event) override;

    void checkAll();
    void pathToSignal(const QString& path);
    void removeFromList(const QString& filePath);
};

#endif // FILECHANGEWORKER_H
