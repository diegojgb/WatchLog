#include "Monitor.h"

#include <QApplication>
#include <fstream>
#include <QMessageBox>


Monitor::Monitor(QObject *parent, const json &monitorData)
    : QObject{parent}
{
    m_name = QString::fromStdString(monitorData["name"]);
    std::string filePath = monitorData["filePath"];
    m_filePath = QString::fromStdString(filePath);

    m_file = std::ifstream(filePath);

    if (!m_file.is_open()) {
        qDebug() << "Error opening file: " << filePath << "\n";
        throw std::runtime_error("Error opening file.");
    }
    m_file.seekg(0, std::ios::end);

    readNotifiers(monitorData["notifiers"]);
}

auto Monitor::jsonFindByKey(const json &data, const std::string &key) {
    if (data.contains(key))
    {
        return data[key];
    }
    else
    {
        QMessageBox::critical(nullptr, tr("WatchLog"), tr("Error: Need a \"%1\" for every Notifier in data.json").arg(QString::fromStdString(key)));
        throw std::runtime_error("Error: Need a \"" + key + "\" for every Notifier in data.json");
    }
}

void Monitor::readNotifiers(const json &data)
{
    for (const auto& item: data) {
        QString name = QString::fromStdString(jsonFindByKey(item, "name").get<std::string>());
        QString regexStr = QString::fromStdString(jsonFindByKey(item, "pattern").get<std::string>());
        QString title = QString::fromStdString(item.value("title", "Match found!"));
        QString desc = QString::fromStdString(item.value("desc", "For regex: " + item["pattern"].get<std::string>()));
        QString imagePath = QString::fromStdString(item.value("image", "D:/Diego/Windows folder sources/Desktop/FallGuysNotifier/assets/fg-faceplate.png"));
        QString duration = QString::fromStdString(item.value("duration", "System"));
        bool toastEnabled = item.value("toast", true);
        bool soundEnabled = item.value("sound", true);

        Notifier* newNotifier = new Notifier(this, name, regexStr, title, desc, imagePath,
                                             duration, toastEnabled, soundEnabled);

        m_notifiers.append(newNotifier);

        if (toastEnabled || soundEnabled)
            m_enabledNotifiers.append(newNotifier);

        QObject::connect(newNotifier, &Notifier::enabled, this, &Monitor::notifierEnabled);
        QObject::connect(newNotifier, &Notifier::disabled, this, &Monitor::notifierDisabled);
    }
}

QString Monitor::name() const
{
    return m_name;
}

void Monitor::setName(const QString &newName)
{
    if (m_name == newName)
        return;

    m_name = newName;

    emit nameChanged();
}

int Monitor::nListLength() const
{
    return m_notifiers.length();
}

Notifier* Monitor::nListAt(int i) const
{
    return m_notifiers[i];
}

void Monitor::notifierDisabled(Notifier* notifier)
{
    m_enabledNotifiers.removeOne(notifier);
}

void Monitor::notifierEnabled(Notifier* notifier)
{
    m_enabledNotifiers.append(notifier);
}

QString Monitor::filePath() const
{
    return m_filePath;
}

void Monitor::setFilePath(const QString &newFilePath)
{
    if (m_filePath == newFilePath)
        return;

    emit filePathChangedOverload(m_filePath, newFilePath);

    m_filePath = newFilePath;

    emit filePathChanged();
}
