#include "TrayIcon.h"

using namespace WinToastLib;


TrayIcon::TrayIcon(QObject* parent, QObject* root)
    : QObject{parent}, m_root{root}
{
    QMenu* trayIconMenu = createMenu();

    m_trayIcon = new QSystemTrayIcon(m_root);
    m_trayIcon->setContextMenu(trayIconMenu);
    m_trayIcon->setIcon(QIcon(":/FallGuysNotifier/assets/watchlog-logo.ico"));
    m_trayIcon->show();

    initWinToast();
    m_toastHandler = new ToastHandler();

    connect(this, SIGNAL(singleClick()), m_root, SLOT(showNormal()));
    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &TrayIcon::trayIconActivated);
}

void TrayIcon::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger)
       emit singleClick();
}

QMenu* TrayIcon::createMenu()
{
    QAction *restoreAction = new QAction(QObject::tr("&Restore"), m_root);
    m_root->connect(restoreAction, SIGNAL(triggered()), m_root, SLOT(showNormal()));
    QAction *quitAction = new QAction(QObject::tr("&Quit"), m_root);
    m_root->connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    QMenu *trayIconMenu = new QMenu();
    trayIconMenu->addAction(restoreAction);
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
