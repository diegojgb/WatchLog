#ifndef FILECHECKER_H
#define FILECHECKER_H

#include "FileCheckWorker.h"
#include "FileStatus.h"

#include <QObject>
#include <QThread>
#include <QCoreApplication>


class FileChecker : public QObject
{
    Q_OBJECT

public:
    explicit FileChecker(QObject *parent, const QList<FileStatus*>& fileList);

    void checkNow();

private:
    QThread* m_thread;
    FileCheckWorker* m_worker;
};

#endif // FILECHECKER_H
