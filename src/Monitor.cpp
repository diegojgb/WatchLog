#include "Monitor.h"
#include "SystemMedia.h"

#include <fstream>
#include <QMessageBox>


Monitor::Monitor(QObject *parent, const json &monitorData)
    : QObject{parent},
      m_name{QString::fromStdString(jsonGetValue<std::string>(monitorData, "name"))},
      m_filePath{QString::fromStdString(jsonGetValue<std::string>(monitorData, "filePath"))},
      m_enabled{jsonGetValue<bool>(monitorData, "enabled", true)},
      m_defaultImage{jsonGetValue<std::string>(monitorData, "defaultImage", Notifier::getDefaultImg().toStdString())},
      manyPerUpdate{jsonGetValue<bool>(monitorData, "manyPerUpdate", false)}
{
    startFile();
    readNotifiers(jsonFindByKey(monitorData, "notifiers"));
    addEmptyNotifier();
}

void Monitor::startFile()
{
    if (m_file.is_open())
        m_file.close();

    m_file = std::ifstream(m_filePath.toStdString());

    if (!m_file.is_open())
        Utils::throwError("Error opening file: " + m_filePath.toStdString());

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

    for (int i = 0; i < m_notifiers.rowCount(); i++)
        obj["notifiers"].push_back(m_notifiers.at(i)->toJSON());

    return obj;
}

json Monitor::jsonFindByKey(const json &data, const std::string &key) {
    if (!data.contains(key))
        Utils::throwError("Need a \""+key+"\" for every Notifier in data.json");

    return data[key];
}

void Monitor::showTypeError(json::type_error e, const std::string& key)
{
    std::string errorStr = std::string(e.what());
    std::regex toRemove("\\[.*\\] *");
    std::string newErrorStr = std::regex_replace(errorStr, toRemove, " ");

    Utils::throwError("JSON: \""+key+"\"" + newErrorStr);
}

template <typename T>
T Monitor::jsonGetValue(const json& data, const std::string& key)
{
    T property;

    json item = jsonFindByKey(data, key);

    try
    {
        property = item.get<T>();
    }
    catch (const json::type_error& e)
    {
        showTypeError(e, key);
    }

    return property;
}

template <typename T>
T Monitor::jsonGetValue(const json& data, const std::string& key, const T defaultValue)
{
    T property;

    try
    {
        property = data.value(key, defaultValue);
    }
    catch (const json::type_error& e)
    {
        showTypeError(e, key);
    }

    return property;
}

void Monitor::readNotifiers(const json &data)
{
    if (!data.is_array() || data.size() == 0)
       Utils::throwError("Invalid notifiers array in JSON");

    for (const json& item: data) {
        QString name = QString::fromStdString(jsonGetValue<std::string>(item, "name"));
        QString regexStr = QString::fromStdString(jsonGetValue<std::string>(item, "pattern"));
        QString title = QString::fromStdString(jsonGetValue<std::string>(item, "title", Notifier::getDefaultTitle().toStdString()));
        QString desc = QString::fromStdString(jsonGetValue<std::string>(item, "desc", Notifier::getDefaultDesc().toStdString()));
        QString soundPath = QString::fromStdString(jsonGetValue<std::string>(item, "soundFile", SystemMedia::getDefaultSound()));
        QString imagePath = QString::fromStdString(jsonGetValue<std::string>(item, "image", m_defaultImage));
        QString duration = QString::fromStdString(jsonGetValue<std::string>(item, "duration", "System"));
        bool toastEnabled = jsonGetValue<bool>(item, "toast", true);
        bool soundEnabled = jsonGetValue<bool>(item, "sound", true);
        bool sticky = jsonGetValue<bool>(item, "sticky", false);

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

void Monitor::notifierDisabled(Notifier* notifier)
{
    m_enabledNotifiers.removeOne(notifier);
}

void Monitor::notifierEnabled(Notifier* notifier)
{
    m_enabledNotifiers.append(notifier);
}

// Add a notifier that represents a "new notifier" object.
void Monitor::addEmptyNotifier()
{
    Notifier* newNotifier = new Notifier(this);
    m_notifiers.append(newNotifier);
    QObject::connect(newNotifier, &Notifier::enabled, this, &Monitor::notifierEnabled);
    QObject::connect(newNotifier, &Notifier::disabled, this, &Monitor::notifierDisabled);
}

void Monitor::removeNotifier(int i)
{
    Notifier* notifier = m_notifiers.at(i);

    if (notifier->soundEnabled() || notifier->toastEnabled())
        m_enabledNotifiers.removeOne(notifier);

    m_notifiers.removeAt(i);
    delete notifier;
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

NotifierList* Monitor::notifiers()
{
    return &m_notifiers;
}
