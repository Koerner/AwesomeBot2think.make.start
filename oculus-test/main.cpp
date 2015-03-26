#include <cstdlib>

#include <OVR.h>

#include <QtCore>
#include <iostream>

int main (int argc, char** argv)
{

    ovr_Initialize();

    int oculusCount = ovrHmd_Detect();
    qDebug() << "detected" << oculusCount << "oculus HMDs";
    if(!oculusCount) return 1;

    ovrHmd hmd = ovrHmd_Create(0);
    if(!hmd)
    {
        qCritical() << "hmd couldnt be created.";
        exit(1);
    }
    qDebug() << "connected to" << hmd->ProductName;

    // Start the sensor which provides the Rift’s pose and motion.
    ovrHmd_ConfigureTracking(hmd, ovrTrackingCap_Orientation |ovrTrackingCap_MagYawCorrection |ovrTrackingCap_Position, 0);
    // Query the HMD for the current tracking state.

    float yaw, eyePitch, eyeRoll;

    while(true)
    {
        ovrTrackingState ts = ovrHmd_GetTrackingState(hmd, ovr_GetTimeInSeconds());


        if (ts.StatusFlags & (ovrStatus_OrientationTracked | ovrStatus_PositionTracked))
        {
            OVR::Quatf quaternion = ts.HeadPose.ThePose.Orientation;
            quaternion.GetEulerAngles<OVR::Axis_Y, OVR::Axis_X, OVR::Axis_Z>(&yaw, &eyePitch, &eyeRoll);
        }

        std::cout << "(Yaw, Pitch, Roll) = (" << yaw << ", " << eyePitch << ", " << eyeRoll << ")" << std::endl;
        QThread::msleep(20);
    }

    if(hmd) ovrHmd_Destroy(hmd);
    ovr_Shutdown();
    return 0;
}
