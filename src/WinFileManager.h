#ifndef WINFILEMANAGER_H
#define WINFILEMANAGER_H

#include "WinFileMonitor.h"
#include "Utils.h"

#include <QObject>

enum class Mode {
    WinApi,
    Manual,
    Mixed
};

class FileStatus: public QObject
{
    Q_OBJECT

public:
    explicit FileStatus(const QString path);

    const QString& getFilePath() const;
    const bool getExists() const;
    void updateExists();
    void setExists(bool newExists);

signals:
    void statusChanged(bool removed);

private:
    QString m_filePath;
    bool m_exists;
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

private:
    WinFileMonitor* m_winFileMonitor;
    QList<FileStatus*> m_fileList;
    const Mode m_mode;
};

#endif // WINFILEMANAGER_H
