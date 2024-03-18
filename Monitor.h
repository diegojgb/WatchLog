#ifndef MONITOR_H
#define MONITOR_H

#include "Notifier.h"

#include <QObject>
#include <QVarLengthArray>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;


class Monitor : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged FINAL)
    Q_PROPERTY(QString filePath READ filePath WRITE setFilePath NOTIFY filePathChanged FINAL)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)

public:
    std::ifstream m_file;
    QVarLengthArray<Notifier*> m_enabledNotifiers;

    explicit Monitor(QObject *parent, const json &monitorData);

    QString name() const;
    void setName(const QString &newName);

    QString filePath() const;
    void setFilePath(const QString &newFilePath);

    bool enabled() const;
    void setEnabled(bool newEnabled);

    void startFile();

public slots:
    int nListLength() const;
    Notifier* nListAt(int i) const;

    void notifierDisabled(Notifier* notifier);
    void notifierEnabled(Notifier* notifier);

signals:
    void nameChanged();
    void filePathChanged();
    void filePathChangedOverload(const QString& oldFilePath, const QString& newFilePath);
    void enabledChanged();
    void monitorEnabled(const Monitor* monitor);
    void monitorDisabled(const Monitor* monitor);

private:
    QString m_name;
    QList<Notifier*> m_notifiers;
    QString m_filePath;
    bool m_enabled;

    void readNotifiers(const json &data);
    auto jsonFindByKey(const json &data, const std::string &key);
};

#endif // MONITOR_H
