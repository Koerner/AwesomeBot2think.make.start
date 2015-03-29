#ifndef OCULUSSENSOR_H
#define OCULUSSENSOR_H

#include <QObject>
#include <OVR.h>

class QTimer;
class QElapsedTimer;

class OculusSensor : public QObject
{
    Q_OBJECT
public:
    explicit OculusSensor(QObject *parent = 0);
    ~OculusSensor();



signals:
    void signalSensorData(double yaw, double roll, double pitch);
    void signalRobotData(double vx, double vy, double theta);

public slots:
    void slotbuttonB(bool);

private slots:
    void slotTimerGetTracking();
    void renderImage();

private:
    ovrHmd hmd;
    QTimer* timer;
    QElapsedTimer* elapsed;

    static const double scaleYaw, scaleRoll, scalePitch;
    static const double yawThreshold, rollThreshold, pitchThreshold;
    static const double lp_0, lp_1;
    double oldYaw, oldRoll, oldPitch;
    double offsetYaw, offsetRoll, offsetPitch;
    int counter;
    bool offsetSet;

    bool buttonB;

};

#endif // OCULUSSENSOR_H
