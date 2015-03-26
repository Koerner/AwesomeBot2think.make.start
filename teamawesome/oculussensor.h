#ifndef OCULUSSENSOR_H
#define OCULUSSENSOR_H

#include <QObject>
#include <OVR.h>

class QTimer;

class OculusSensor : public QObject
{
    Q_OBJECT
public:
    explicit OculusSensor(QObject *parent = 0);
    ~OculusSensor();

    static const double scaleYaw, scaleRoll, scalePitch;

signals:
    void signalSensorData(double yaw, double roll, double pitch);

public slots:

private slots:
    void slotTimerGetTracking();

private:
    ovrHmd hmd;
    QTimer* timer;
};

#endif // OCULUSSENSOR_H
