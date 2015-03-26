#include "oculussensor.h"

#include <OVR.h>
#include <QtCore>

OculusSensor::OculusSensor(QObject *parent) :
    QObject(parent)
{
    ovr_Initialize();
    hmd = ovrHmd_Create(0);
    if(!hmd)
    {
        qCritical() << "Could not connect Oculus";
    } else {
        qDebug() << "Connected to" << hmd->ProductName;
    }

    // Start the sensor which provides the Rift’s pose and motion.
    ovrHmd_ConfigureTracking(hmd, ovrTrackingCap_Orientation |ovrTrackingCap_MagYawCorrection |ovrTrackingCap_Position, 0);

    timer = new QTimer();
    timer->setInterval(20);
    connect(timer, SIGNAL(timeout()), this, SLOT(slotTimerGetTracking()));
    timer->start();
}

OculusSensor::~OculusSensor()
{
    if(hmd) ovrHmd_Destroy(hmd);
    ovr_Shutdown();
}

void OculusSensor::slotTimerGetTracking()
{
    float yaw, eyePitch, eyeRoll;
    ovrTrackingState ts = ovrHmd_GetTrackingState(hmd, ovr_GetTimeInSeconds());
    if (ts.StatusFlags & (ovrStatus_OrientationTracked | ovrStatus_PositionTracked))
    {
        OVR::Quatf quaternion = ts.HeadPose.ThePose.Orientation;
        quaternion.GetEulerAngles<OVR::Axis_Y, OVR::Axis_X, OVR::Axis_Z>(&yaw, &eyePitch, &eyeRoll);
    }
    emit signalSensorData(yaw, eyePitch, eyeRoll);
}
