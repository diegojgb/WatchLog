#ifndef FILESTATUS_H
#define FILESTATUS_H

#include <QObject>
#include <QDebug>
#include <filesystem>


class FileStatus: public QObject
{
    Q_OBJECT

public:
    explicit FileStatus(const QString path, QObject *parent = nullptr);

    const QString& getFilePath() const;
    const bool getExists() const;
    void updateExists();
    void setExists(bool newExists);

signals:
    void statusChanged(bool removed);
    void allSlotsDisconnected(FileStatus* instance);

protected:
    void disconnectNotify(const QMetaMethod& signal) override;

private:
    QString m_filePath;
    bool m_exists;
};

#endif // FILESTATUS_H
