#include "TrayIcon.h"

using namespace WinToastLib;


TrayIcon::TrayIcon(QObject* parent, QObject* root)
    : QObject{parent}, m_root{root}
{
    QMenu* trayIconMenu = createMenu();

    QSystemTrayIcon *trayIcon = new QSystemTrayIcon(m_root);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setIcon(QIcon(":/FallGuysNotifier/assets/watchlog-logo.ico"));
    trayIcon->show();

    initWinToast();

    m_toastHandler = new ToastHandler();
}

QMenu* TrayIcon::createMenu()
{
    QAction *minimizeAction = new QAction(QObject::tr("Mi&nimize"), m_root);
    m_root->connect(minimizeAction, SIGNAL(triggered()), m_root, SLOT(hide()));
    QAction *maximizeAction = new QAction(QObject::tr("Ma&ximize"), m_root);
    m_root->connect(maximizeAction, SIGNAL(triggered()), m_root, SLOT(showMaximized()));
    QAction *restoreAction = new QAction(QObject::tr("&Restore"), m_root);
    m_root->connect(restoreAction, SIGNAL(triggered()), m_root, SLOT(showNormal()));
    QAction *quitAction = new QAction(QObject::tr("&Quit"), m_root);
    m_root->connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    QMenu *trayIconMenu = new QMenu();
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    return trayIconMenu;
}

void TrayIcon::initWinToast()
{
    if (!WinToast::isCompatible())
        throw std::runtime_error("WinToast: Error, your system in not supported!");

    WinToast::instance()->setAppName(L"WatchLog");
    const auto aumi = WinToast::configureAUMI(L"Obin XYZ", L"WatchLog", L"MatchNotification", L"1.0");
    WinToast::instance()->setAppUserModelId(aumi);

    if (!WinToast::instance()->initialize())
        throw std::runtime_error("Error, could not initialize WinToast!");
}

void TrayIcon::sendNotification(const WinToastTemplate& templ)
{
    WinToast::WinToastError error;

    if (WinToast::instance()->showToast(templ, m_toastHandler, &error) == -1L) {
        throw std::runtime_error("Error: Could not launch your toast notification!");
    }
}
