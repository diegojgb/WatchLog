#include "Monitor.h"
#include "SystemMedia.h"

#include <fstream>
#include <QMessageBox>


Monitor::Monitor(QObject *parent, const json &monitorData)
    : QObject{parent}
{
    m_name = QString::fromStdString(jsonFindByKey(monitorData, "name").get<std::string>());
    m_filePath = QString::fromStdString(jsonFindByKey(monitorData, "filePath").get<std::string>());
    m_enabled = monitorData.value("enabled", true);
    manyPerUpdate = monitorData.value("manyPerUpdate", true);

    startFile();

    readNotifiers(jsonFindByKey(monitorData, "notifiers"));
}

void Monitor::startFile()
{
    if (m_file.is_open())
        m_file.close();

    m_file = std::ifstream(m_filePath.toStdString());

    if (!m_file.is_open()) {
        qDebug() << "Error opening file: " << m_filePath << "\n";
        throw std::runtime_error("Error opening file.");
    }

    m_file.seekg(0, std::ios::end);
}

json Monitor::toJSON() const
{
    json obj;

    obj["name"] = m_name.toStdString();
    obj["filePath"] = m_filePath.toStdString();
    obj["enabled"] = m_enabled;
    obj["notifiers"] = json::array();
    obj["manyPerUpdate"] = manyPerUpdate;

    for (const Notifier* notifier: m_notifiers) {
        obj["notifiers"].push_back(notifier->toJSON());
    }

    return obj;
}

json Monitor::jsonFindByKey(const json &data, const std::string &key) {
    if (data.contains(key)) {
        return data[key];
    }
    else {
        QMessageBox::critical(nullptr, tr("WatchLog"), tr("Error: Need a \"%1\" for every Notifier in data.json").arg(QString::fromStdString(key)));
        throw std::runtime_error("Error: Missing a \"" + key + "\" in JSON object");
    }
}

void Monitor::readNotifiers(const json &data)
{
    if (!data.is_array() || data.size() == 0) {
        QMessageBox::critical(nullptr, tr("WatchLog"), tr("Error: Invalid notifiers array in JSON"));
        throw std::runtime_error("Error: Invalid notifiers array in JSON");
    }

    for (const json& item: data) {
        QString name = QString::fromStdString(jsonFindByKey(item, "name").get<std::string>());
        QString regexStr = QString::fromStdString(jsonFindByKey(item, "pattern").get<std::string>());
        QString title = QString::fromStdString(item.value("title", Notifier::getDefaultTitle().toStdString()));
        QString desc = QString::fromStdString(item.value("desc", Notifier::getDefaultDesc().toStdString()));
        QString soundPath = QString::fromStdString(item.value("soundFile", SystemMedia::getDefaultSound()));
        QString imagePath = QString::fromStdString(item.value("image", Notifier::getDefaultImg().toStdString()));
        QString duration = QString::fromStdString(item.value("duration", "System"));
        bool toastEnabled = item.value("toast", true);
        bool soundEnabled = item.value("sound", true);
        bool sticky = item.value("sticky", false);

        Notifier* newNotifier = new Notifier(this, name, regexStr, title, desc, imagePath,
                                             soundPath, duration, toastEnabled, soundEnabled, sticky);

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

    startFile();
    emit filePathChanged();
}

bool Monitor::enabled() const
{
    return m_enabled;
}

void Monitor::setEnabled(bool newEnabled)
{
    if (m_enabled == newEnabled)
        return;

    m_enabled = newEnabled;

    if (newEnabled)
    {
        startFile();
        emit monitorEnabled(this);
    }
    else
        emit monitorDisabled(this);

    emit enabledChanged();
}
