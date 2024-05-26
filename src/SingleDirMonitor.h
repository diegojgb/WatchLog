#ifndef SINGLEDIRMONITOR_H
#define SINGLEDIRMONITOR_H

#include <Windows.h>
#include <iostream>
#include <QObject>
#include <QDebug>
#include <QDir>

namespace fs = std::filesystem;

enum class Change {
    Added,
    Removed,
    RenamedTo,
    RenamedFrom
};

class SingleDirMonitor: public QObject
{
    Q_OBJECT

public:
    explicit SingleDirMonitor(QObject* parent, const QString& path);
    ~SingleDirMonitor();

    static QString firstParent(const QString& filePath);
    static QString firstAvailable(const QString& filePath);
    static VOID CALLBACK onDirectoryChange(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped);

    void startWatching();

    const QString& getPath() const;
    const QList<QString>& getFiles() const;
    void addFile(const QString& filePath);
    bool removeFile(const QString& filePath);
    bool cdUpTo(const QString& path);
    void deleteLater();
    bool hasPendingDelete();

signals:
    void changeFound(const QString& filePath, const Change type);
    void error(SingleDirMonitor* instance);
    void cdUpped(SingleDirMonitor* instance);
    void clearForDeletion(SingleDirMonitor* instance);

private:
    QString m_qPath;
    QString m_qParentPath;
    std::wstring m_wParentPath;
    QList<QString> m_files;
    uint8_t m_changeBuff[1024];
    OVERLAPPED m_overlapped;
    HANDLE m_dir;
    bool m_pending = false;
    bool m_pendingDelete = false;

    bool isMonitored(const QString& path);
    void processNotification();
    bool onRemoved(const QString& filePath);
    void onAdded(const QString& filePath);
    bool wasDirRemoved(const QString& filePath);
    bool clearPending();
    void startFile();
    void resetFile();
    void cdUp();
    QString parseFileName(const std::wstring& fileName);
};

#endif // SINGLEDIRMONITOR_H
