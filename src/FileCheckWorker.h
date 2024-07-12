#ifndef FILECHECKWORKER_H
#define FILECHECKWORKER_H

#include "FileStatus.h"

#include <QObject>
#include <QTimerEvent>


class FileCheckWorker: public QObject
{
    Q_OBJECT

public:
    explicit FileCheckWorker(QObject* parent, const QList<FileStatus*>& fileList);

    Q_INVOKABLE void stop();
    Q_INVOKABLE void checkNow();

public slots:
    void start();
    void finish();

signals:
    void finished();

private:
    const QList<FileStatus*>& m_fileList;

    int m_timer{};
    int m_pollingRate = 10000;
    bool m_running = false;

    void timerEvent(QTimerEvent* event) override;

    void checkAll();
};

#endif // FILECHECKWORKER_H
