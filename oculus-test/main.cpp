#include <cstdlib>

#include <OVR.h>

#include <QtCore>

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


    if(hmd) ovrHmd_Destroy(hmd);
    ovr_Shutdown();
    return 0;
}
