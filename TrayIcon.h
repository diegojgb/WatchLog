#ifndef TRAYICON_H
#define TRAYICON_H

#include <QObject>
#include <QAction>
#include <QMenu>
#include <QApplication>
#include <QSystemTrayIcon>
#include "ToastHandler.h"
#include "wintoastlib.h"

class TrayIcon : public QObject
{
    Q_OBJECT
public:
    explicit TrayIcon(QObject *parent, QObject *root);

public slots:
    void sendNotification(const WinToastTemplate& templ);

private:
    QObject* m_root;
    ToastHandler* m_toastHandler;

    QMenu* createMenu();
    void initWinToast();
};

#endif // TRAYICON_H
