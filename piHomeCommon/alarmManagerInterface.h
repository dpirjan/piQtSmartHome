#ifndef ALARMMANAGERINTERFACE_H
#define ALARMMANAGERINTERFACE_H

#include <QObject>
#include <QtDBus>

class QDBusInterface;

class AlarmManagerInterface : public QObject
{
    Q_OBJECT

    explicit AlarmManagerInterface(QObject *parent = 0);
    virtual ~AlarmManagerInterface();
    Q_DISABLE_COPY(AlarmManagerInterface)

    bool connectToDBus();

public:
    static AlarmManagerInterface &instance()
    {
        static AlarmManagerInterface obj;
        return obj;
    }

signals:
    void alarmEventNotification();

public slots:
    void setAlarmGeneralState(const bool&);
    void setAlarmNightState(const bool&);
    void setAlarmVacationState(const bool&);

    bool getAlarmGeneralState() const;
    bool getAlarmNightState() const;
    bool getAlarmVacationState() const;

private:
    QDBusInterface *m_iface;
};

#endif // ALARMMANAGERINTERFACE_H
