#include "oculussensor.h"

#include <OVR.h>
#include <cmath>
#include <QtCore>
#include <iostream>

double sign(double x)
{
    if(x > 0) {return 1;}
    else if (x < 0 ) {return -1;}
    else {return 0;}
}


const double OculusSensor::scaleYaw = 1.0;
const double OculusSensor::scalePitch = -1.0;
const double OculusSensor::scaleRoll = 1.0;

const double OculusSensor::yawThreshold = 90;
const double OculusSensor::pitchThreshold = 60;
const double OculusSensor::rollThreshold = 60;

const double OculusSensor::lp_0 = 0.01;
const double OculusSensor::lp_1 = 0.99;


OculusSensor::OculusSensor(QObject *parent) :
    QObject(parent)
{
    counter = 0;
    oldPitch = 0;
    oldRoll = 0;
    oldYaw = 0;

    offsetSet = false;
    offsetYaw = 0;
    offsetRoll = 0;
    offsetPitch = 0;

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

    elapsed = new QElapsedTimer;
    elapsed->start();
}

OculusSensor::~OculusSensor()
{
    delete elapsed;
    delete timer;

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

    yaw = yaw * 180.0/M_PI;
    eyePitch= eyePitch * 180.0/M_PI;
    eyeRoll = eyeRoll * 180.0/M_PI;

    if(elapsed->elapsed() > 1000 && elapsed->elapsed() < 1100)
    {
        offsetYaw = offsetYaw + yaw;
        offsetPitch = offsetPitch + eyePitch;
        offsetRoll = offsetRoll + eyeRoll;

        counter = counter + 1;
    }
    else if(elapsed->elapsed() >= 1100 && offsetSet == false)
    {
        offsetYaw = offsetYaw / counter;
        offsetPitch = offsetPitch / counter;
        offsetRoll = offsetRoll / counter;

        offsetSet = true;
    }
    else if(offsetSet == true)
    {
        eyePitch = eyePitch - offsetPitch;
        eyeRoll = eyeRoll - offsetRoll;
        yaw = yaw - offsetYaw;

        if(fabs(eyePitch) > pitchThreshold) {eyePitch = sign(eyePitch) * pitchThreshold;}
        if(fabs(eyeRoll) > rollThreshold) {eyeRoll = sign(eyeRoll) * rollThreshold;}

        if(buttonB == true)
        {
            Q_EMIT signalSensorData((lp_0 * oldYaw + lp_1 * yaw)*scaleYaw, (lp_0 * oldRoll + lp_1 * eyeRoll)*scaleRoll, (lp_0 *oldPitch + lp_1 * eyePitch)*scalePitch);

            if(fabs(yaw) > yawThreshold)
            {
                yaw = sign(yaw) * yawThreshold;
                Q_EMIT signalRobotData(0,0,sign(yaw));
            }

        }
    }
}

void OculusSensor::slotbuttonB(bool state)
{
    buttonB = state;
}


void OculusSensor::renderImage()
{
    //    // Configure Stereo settings.
    //    OVR::Sizei recommenedTex0Size = ovrHmd_GetFovTextureSize(hmd, ovrEye_Left,hmd->DefaultEyeFov[0], 1.0f);
    //    OVR::Sizei recommenedTex1Size = ovrHmd_GetFovTextureSize(hmd, ovrEye_Right,hmd->DefaultEyeFov[1], 1.0f);

    //    OVR::Sizei renderTargetSize;
    //    renderTargetSize.w = recommenedTex0Size.w + recommenedTex1Size.w;

    //    renderTargetSize.h = std::max ( recommenedTex0Size.h, recommenedTex1Size.h );

    //    const int eyeRenderMultisample = 1;
    //    Texture* pRendertargetTexture = OVR::pRender->CreateTexture(Texture_RGBA | Texture_RenderTarget | eyeRenderMultisample,OVR::renderTargetSize.w, renderTargetSize.h, NULL);

    //    // The actual RT size may be different due to HW limits.
    //    OVR::renderTargetSize.w = OVR::pRendertargetTexture->GetWidth();
    //    OVR::renderTargetSize.h = OVR::pRendertargetTexture->GetHeight();
}
