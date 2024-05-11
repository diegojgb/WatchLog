#ifndef MONITOR_H
#define MONITOR_H

#include "nlohmann/json.hpp"
#include "Notifier.h"
#include "NotifierList.h"
#include "SystemMedia.h"

#include <QObject>
#include <QVarLengthArray>

using json = nlohmann::json;


class Monitor : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged FINAL)
    Q_PROPERTY(QString filePath READ filePath WRITE setFilePath NOTIFY filePathChanged FINAL)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(NotifierList* notifiers READ notifiers CONSTANT)
    Q_PROPERTY(int enabledNotifierCount READ enabledNotifierCount WRITE setEnabledNotifierCount NOTIFY enabledNotifierCountChanged FINAL)
    Q_PROPERTY(bool fileError READ fileError WRITE setFileError NOTIFY fileErrorChanged FINAL)

public:
    static json jsonFindByKey(const json &data, const std::string &key);

    explicit Monitor(QObject *parent, const json &monitorData);
    explicit Monitor(QObject *parent, const QString& name, const QString& filePath);

    void showTypeError(json::type_error e, const std::string &key);
    json toJSON() const;

    const QVarLengthArray<Notifier*>& getEnabledNotifiers() const;
    const bool getManyPerUpdate() const;

    QString name() const;
    void setName(const QString &newName);

    QString filePath() const;
    void setFilePath(const QString &newFilePath);

    bool enabled() const;
    void setEnabled(bool newEnabled);

    int enabledNotifierCount() const;
    void setEnabledNotifierCount(int newEnabledNotifierCount);

    bool fileError() const;
    void setFileError(bool newFileError);

    template<typename T>
    T jsonGetValue(const json &data, const std::string &key);

    template<typename T>
    T jsonGetValue(const json &data, const std::string &key, const T defaultValue);

    NotifierList* notifiers();

public slots:
    void notifierDisabled(Notifier* notifier);
    void notifierEnabled(Notifier* notifier);
    void addEmptyNotifier();
    void removeNotifier(int i);

signals:
    void nameChanged();
    void filePathChanged();
    void filePathChangedOverload(const QString& oldFilePath, const QString& newFilePath);
    void enabledChanged();
    void enabledNotifierCountChanged();
    void fileErrorChanged();

    void monitorEnabled(const Monitor* monitor);
    void monitorDisabled(const Monitor* monitor);

private:
    QVarLengthArray<Notifier*> m_enabledNotifiers;
    NotifierList m_notifiers;
    std::string m_defaultImage;
    QString m_name; 
    QString m_filePath;
    bool m_manyPerUpdate;
    bool m_fileError = false;
    bool m_enabled = false;
    bool m_initialized = false;
    int m_enabledNotifierCount = 0;

    void readNotifiers(const json &data);
};

#endif // MONITOR_H
