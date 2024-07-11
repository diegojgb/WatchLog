#ifndef MONITOR_H
#define MONITOR_H

#include "nlohmann/json.hpp"
#include "NotifierList.h"
#include "SystemMedia.h"
#include "WinFileManager.h"

#include <QObject>
#include <QVarLengthArray>

using json = nlohmann::json;


class Notifier; // Forward declaration.

class Monitor: public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged FINAL)
    Q_PROPERTY(QString filePath READ filePath WRITE setFilePath NOTIFY filePathChanged FINAL)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(NotifierList* notifiers READ notifiers CONSTANT)
    Q_PROPERTY(int enabledNotifierCount READ enabledNotifierCount WRITE setEnabledNotifierCount NOTIFY enabledNotifierCountChanged FINAL)
    Q_PROPERTY(bool fileError READ fileError WRITE setFileError NOTIFY fileErrorChanged FINAL)
    Q_PROPERTY(QString defaultImage READ defaultImage WRITE setDefaultImage NOTIFY defaultImageChanged FINAL)
    Q_PROPERTY(QString defaultSound READ defaultSound WRITE setDefaultSound NOTIFY defaultSoundChanged FINAL)
    Q_PROPERTY(bool imageError READ imageError NOTIFY imageErrorChanged FINAL)
    Q_PROPERTY(bool soundError READ soundError NOTIFY soundErrorChanged FINAL)

public:
    static json jsonFindByKey(const json& data, const std::string& key);
    static void showTypeError(json::type_error e, const std::string& key);

    template<typename T>
    static T jsonGetValue(const json& data, const std::string& key);

    template<typename T>
    static T jsonGetValue(const json& data, const std::string& key, const T defaultValue);

    explicit Monitor(QObject* parent, const json& monitorData, WinFileManager& winFileManager);
    explicit Monitor(QObject* parent, const QString& name, const QString& filePath, WinFileManager& winFileManager);

    json toJSON() const;

    const QVarLengthArray<Notifier*>& getEnabledNotifiers() const;
    const bool getManyPerUpdate() const;

    QString name() const;
    void setName(const QString& newName);

    QString filePath() const;
    void setFilePath(const QString& newFilePath);

    bool enabled() const;
    void setEnabled(bool newEnabled);

    int enabledNotifierCount() const;
    void setEnabledNotifierCount(int newEnabledNotifierCount);

    bool fileError() const;
    void setFileError(bool newFileError);

    NotifierList* notifiers();

    QString defaultImage() const;
    void setDefaultImage(const QString& newDefaultImage);

    QString defaultSound() const;
    void setDefaultSound(const QString& newDefaultSound);

    bool imageError() const;
    void setImageError(bool newImageError);

    bool soundError() const;
    void setSoundError(bool newSoundError);

    Q_INVOKABLE void addEmptyNotifier();
    Q_INVOKABLE void removeNotifier(int i);

public slots:
    void notifierDisabled(Notifier* notifier);
    void notifierEnabled(Notifier* notifier);

signals:
    void monitorEnabled(const Monitor* monitor);
    void monitorDisabled(const Monitor* monitor);
    void filePathChangedOverload(const QString& oldFilePath, const QString& newFilePath);

    void nameChanged();
    void filePathChanged();
    void enabledChanged();
    void enabledNotifierCountChanged();
    void fileErrorChanged();
    void defaultImageChanged();
    void defaultSoundChanged();
    void imageErrorChanged();
    void soundErrorChanged();

private:
    WinFileManager& m_winFileManager;
    QVarLengthArray<Notifier*> m_enabledNotifiers;
    NotifierList m_notifiers;
    QString m_name; 
    QString m_filePath;
    QString m_defaultImage;
    QString m_defaultSound;
    QString m_staticDefaultImage;
    bool m_manyPerUpdate;
    bool m_enabled = false;
    bool m_fileError = false;
    bool m_initialized = false;
    bool m_imageError = false;
    bool m_soundError = false;
    int m_enabledNotifierCount = 0;

    void readNotifiers(const json& data);
};

#endif // MONITOR_H
