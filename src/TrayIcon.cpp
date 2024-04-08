#include "TrayIcon.h"

using namespace WinToastLib;


TrayIcon::TrayIcon(QObject* parent, QObject* root, HWND hwnd)
    : QObject{parent}, m_root{root}, m_hwnd{hwnd}
{
    QMenu* trayIconMenu = createMenu();

    m_trayIcon = new QSystemTrayIcon(m_root);
    m_trayIcon->setContextMenu(trayIconMenu);
    m_trayIcon->setIcon(QIcon(":/assets/watchlog-logo.ico"));
    m_trayIcon->show();

    initWinToast();

    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &TrayIcon::trayIconActivated);
    connect(this, SIGNAL(singleClick()), m_root, SLOT(showNormal()));
    connect(this, SIGNAL(singleClick()), this, SLOT(bringToTop()));
}

void TrayIcon::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger)
        emit singleClick();
}

void TrayIcon::bringToTop()
{
    SetForegroundWindow(m_hwnd);
}

QMenu* TrayIcon::createMenu()
{
    QAction *restoreAction = new QAction(QObject::tr("&Restore"), m_root);
    m_root->connect(restoreAction, SIGNAL(triggered()), m_root, SLOT(showNormal()));
    m_root->connect(restoreAction, SIGNAL(triggered()), this, SLOT(bringToTop()));
    QAction *quitAction = new QAction(QObject::tr("&Quit"), m_root);
    m_root->connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    QMenu *trayIconMenu = new QMenu();
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addAction(quitAction);
    trayIconMenu->setStyleSheet("QMenu { background-color: white; padding: 3px; }\
                                QMenu::item { color: black; background-color: transparent; padding: 4px 40px 4px 12px; }\
                                QMenu::item:selected { color: white; background-color: #0078d4; border: 0px; }");

    return trayIconMenu;
}

void TrayIcon::initWinToast()
{
    if (!WinToast::isCompatible())
        Utils::throwError("WinToast: Error, your system in not supported!");

    WinToast::instance()->setAppName(L"WatchLog");
    const auto aumi = WinToast::configureAUMI(L"ObinXYZ", L"WatchLog", L"MatchNotification", L"1.0");
    WinToast::instance()->setAppUserModelId(aumi);
    WinToast::instance()->setShortcutPolicy(WinToast::ShortcutPolicy::SHORTCUT_POLICY_IGNORE);

    // Add icon and name to registry, so Windows can display them in toast notifications.
    if (!Registry::createRegistryKey(HKEY_CURRENT_USER, L"Software\\Classes\\AppUserModelId\\ObinXYZ.WatchLog.MatchNotification.1.0")) {
        Utils::throwError("Error opening or creating new Registry key");
    }

    if (!Registry::writeStringInRegistry(
                HKEY_CURRENT_USER,
                L"Software\\Classes\\AppUserModelId\\ObinXYZ.WatchLog.MatchNotification.1.0",
                L"DisplayName",
                L"WatchLog")) {
        Utils::throwError("Error saving toast DisplayName Regitry value");
    }

    QString defaultIcon = QCoreApplication::applicationDirPath() + "/assets/watchlog-logo.ico";
    defaultIcon.replace("/", "\\");
    if (!Registry::writeStringInRegistry(
                HKEY_CURRENT_USER,
                L"Software\\Classes\\AppUserModelId\\ObinXYZ.WatchLog.MatchNotification.1.0",
                L"IconUri",
                defaultIcon.toStdWString().c_str())) {
        Utils::throwError("Error saving toast IconUri Regitry value");
    }

    if (!WinToast::instance()->initialize())
        Utils::throwError("Error, could not initialize WinToast!");
}

void TrayIcon::sendNotification(const WinToastTemplate& templ)
{
    if (WinToast::instance()->showToast(templ, new ToastHandler()) < 0) {
        Utils::throwError("Could not launch your toast notification!");
    }
}
