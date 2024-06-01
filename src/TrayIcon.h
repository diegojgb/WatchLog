#ifndef TRAYICON_H
#define TRAYICON_H

#include "Utils.h"
#include "ToastHandler.h"
#include "Registry.h"
#include "wintoastlib.h"
#include "ExitDialog.h"

#include <QObject>
#include <QAction>
#include <QMenu>
#include <QApplication>
#include <QSystemTrayIcon>
#include <QQuickWindow>
#include <Windows.h>


class TrayIcon: public QObject
{
    Q_OBJECT

public:
    explicit TrayIcon(QObject *parent, QObject *root, HWND hwnd);

public slots:
    void sendNotification(const WinToastTemplate& templ);
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void bringToTop();
    void tryQuitApp();

signals:
    void singleClick();
    void saveRequested();

private:
    QObject* m_root;
    HWND m_hwnd;
    QSystemTrayIcon* m_trayIcon;
    ExitDialog m_dialog;

    QMenu* createMenu();
    void initWinToast();
};

#endif // TRAYICON_H
