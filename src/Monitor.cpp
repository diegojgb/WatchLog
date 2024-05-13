#include "Monitor.h"


Monitor::Monitor(QObject *parent, const json &monitorData)
    : QObject{parent},
      m_manyPerUpdate{jsonGetValue<bool>(monitorData, "manyPerUpdate", false)}
{
    setFilePath(QString::fromStdString(jsonGetValue<std::string>(monitorData, "filePath")));
    setName(QString::fromStdString(jsonGetValue<std::string>(monitorData, "name")));

    auto imagePath = jsonGetValue<std::string>(monitorData, "defaultImage", Notifier::getDefaultImg().toStdString());
    setDefaultImage(QString::fromStdString(imagePath));

    auto soundPath = jsonGetValue<std::string>(monitorData, "defaultSound", SystemMedia::getDefaultSound());
    setDefaultSound(QString::fromStdString(soundPath));

    readNotifiers(jsonFindByKey(monitorData, "notifiers"));
    addEmptyNotifier();

    setEnabled(jsonGetValue<bool>(monitorData, "enabled", true));
}

Monitor::Monitor(QObject *parent, const QString &name, const QString &filePath)
    : QObject{parent},
      m_manyPerUpdate{false}
{
    setName(name);
    setFilePath(filePath);
    setDefaultImage(Notifier::getDefaultImg());
    setDefaultSound(QString::fromStdString(SystemMedia::getDefaultSound()));

    addEmptyNotifier();
}

json Monitor::toJSON() const
{
    json obj;

    obj["name"] = m_name.toStdString();
    obj["filePath"] = m_filePath.toStdString();
    obj["enabled"] = m_enabled;
    obj["notifiers"] = json::array();
    obj["manyPerUpdate"] = m_manyPerUpdate;

    for (int i = 0; i < m_notifiers.rowCount() - 1; i++)
        obj["notifiers"].push_back(m_notifiers.at(i)->toJSON());

    return obj;
}

const QVarLengthArray<Notifier *> &Monitor::getEnabledNotifiers() const
{
    return m_enabledNotifiers;
}

const bool Monitor::getManyPerUpdate() const
{
    return m_manyPerUpdate;
}

json Monitor::jsonFindByKey(const json &data, const std::string &key) {
    if (!data.contains(key))
        Utils::throwError("Missing a \""+key+"\" property for some element in data.json");

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
    if (!data.is_array())
       Utils::throwError("Invalid notifiers array in JSON");
    if (data.size() == 0)
        return;

    for (const json& item: data) {
        QString name = QString::fromStdString(jsonGetValue<std::string>(item, "name"));
        QString regexStr = QString::fromStdString(jsonGetValue<std::string>(item, "pattern"));
        QString title = QString::fromStdString(jsonGetValue<std::string>(item, "title", Notifier::getDefaultTitle().toStdString()));
        QString desc = QString::fromStdString(jsonGetValue<std::string>(item, "desc", Notifier::getDefaultDesc().toStdString()));
        QString soundPath = QString::fromStdString(jsonGetValue<std::string>(item, "soundFile", m_defaultSound.toStdString()));
        QString imagePath = QString::fromStdString(jsonGetValue<std::string>(item, "image", m_defaultImage.toStdString()));
        QString duration = QString::fromStdString(jsonGetValue<std::string>(item, "duration", "System"));
        bool toastEnabled = jsonGetValue<bool>(item, "toast", true);
        bool soundEnabled = jsonGetValue<bool>(item, "sound", true);
        bool sticky = jsonGetValue<bool>(item, "sticky", false);

        Notifier* newNotifier = new Notifier(this, name, regexStr, title, desc, imagePath,
                                             soundPath, duration, toastEnabled, soundEnabled, sticky);

        m_notifiers.append(newNotifier);

        if (toastEnabled || soundEnabled) {
            m_enabledNotifiers.append(newNotifier);
            setEnabledNotifierCount(m_enabledNotifiers.size());
        }

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
    if (m_initialized && m_name == newName)
        return;

    m_name = newName;

    emit nameChanged();
}

void Monitor::notifierDisabled(Notifier* notifier)
{
    m_enabledNotifiers.removeOne(notifier);
    setEnabledNotifierCount(m_enabledNotifiers.size());

    if (enabledNotifierCount() == 0)
        setEnabled(false);
}

void Monitor::notifierEnabled(Notifier* notifier)
{
    m_enabledNotifiers.append(notifier);
    setEnabledNotifierCount(m_enabledNotifiers.size());
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

    if (notifier->soundEnabled() || notifier->toastEnabled()) {
        m_enabledNotifiers.removeOne(notifier);

        setEnabledNotifierCount(m_enabledNotifiers.size());

        if (enabledNotifierCount() == 0)
            setEnabled(false);
    }

    m_notifiers.removeAt(i);
    delete notifier;
}

QString Monitor::filePath() const
{
    return m_filePath;
}

void Monitor::setFilePath(const QString &newFilePath)
{
    if (m_initialized && m_filePath == newFilePath)
        return;

    m_filePath = newFilePath;

    bool exists = std::filesystem::exists(newFilePath.toStdString());

    if (!exists)
        setFileError(true);
    else
        setFileError(false);

    if (m_enabled && exists)
        emit filePathChangedOverload(m_filePath, newFilePath);

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

    if (newEnabled)
    {
        if (!m_enabledNotifiers.size() || m_fileError)
            return;

        emit monitorEnabled(this);
    }
    else {
        emit monitorDisabled(this);
    }

    m_enabled = newEnabled;

    emit enabledChanged();
}

NotifierList* Monitor::notifiers()
{
    return &m_notifiers;
}

int Monitor::enabledNotifierCount() const
{
    return m_enabledNotifierCount;
}

void Monitor::setEnabledNotifierCount(int newEnabledNotifierCount)
{
    if (m_enabledNotifierCount == newEnabledNotifierCount)
        return;

    m_enabledNotifierCount = newEnabledNotifierCount;

    emit enabledNotifierCountChanged();
}

bool Monitor::fileError() const
{
    return m_fileError;
}

void Monitor::setFileError(bool newFileError)
{
    if (m_fileError == newFileError)
        return;

    m_fileError = newFileError;

    emit fileErrorChanged();
}

QString Monitor::defaultImage() const
{
    return m_defaultImage;
}

void Monitor::setDefaultImage(const QString &newDefaultImage)
{
    if (m_defaultImage == newDefaultImage)
        return;

    m_defaultImage = newDefaultImage;

    emit defaultImageChanged();
}

QString Monitor::defaultSound() const
{
    return m_defaultSound;
}

void Monitor::setDefaultSound(const QString &newDefaultSound)
{
    if (m_defaultSound == newDefaultSound)
        return;

    m_defaultSound = newDefaultSound;

    emit defaultSoundChanged();
}

bool Monitor::imageError() const
{
    return m_imageError;
}

void Monitor::setImageError(bool newImageError)
{
    if (m_imageError == newImageError)
        return;

    m_imageError = newImageError;

    emit imageErrorChanged();
}

bool Monitor::soundError() const
{
    return m_soundError;
}

void Monitor::setSoundError(bool newSoundError)
{
    if (m_soundError == newSoundError)
        return;

    m_soundError = newSoundError;

    emit soundErrorChanged();
}
