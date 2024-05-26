#ifndef WINFILEMANAGER_H
#define WINFILEMANAGER_H

#include "WinFileMonitor.h"
#include "FileChecker.h"
#include "FileStatus.h"
#include "Utils.h"

#include <QObject>

enum class Mode {
    WinApi,
    Manual,
    Mixed
};

class WinFileManager: public QObject
{
    Q_OBJECT

public:
    explicit WinFileManager(QObject *parent, const Mode mode);

    FileStatus* findOrCreate(const QString& path);
    FileStatus* find(const QString& path);

public slots:
    void onChangeFound(const QString& filePath, const Change type);
    void onAllSlotsDisconnected(FileStatus* instance);

private:
    WinFileMonitor* m_winFileMonitor;
    FileChecker* m_fileChecker;
    QList<FileStatus*> m_fileList;
    const Mode m_mode;
};

#endif // WINFILEMANAGER_H
