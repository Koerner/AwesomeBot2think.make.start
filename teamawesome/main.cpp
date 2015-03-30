
#include <string>
#include <iostream>

#include <QtCore>
#include <QtTest/QtTest>
#include <QApplication>

#include <rec/robotino/api2/all.h>

#include "com.h"
#include "cam3d.h"
#include "gamepad.h"
#include "robotinocontrol.h"
#include "mainwindow.h"
#include "renderwindow.h"
#include "dxlcon.h"
#include "oculussensor.h"

// Global pointers
Com* com;


int main (int argc, char** argv) {
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    qRegisterMetaType <cv::Mat> ("cv::Mat");

    QCoreApplication a(argc, argv);


    try {
        // Set up Robotino connection
        com = new Com("Awesome", "172.26.1.1");
        if(! com->isConnected() ) {
            std::cerr << "could not connect to robot" << std::endl;
            rec::robotino::api2::shutdown();
            exit(1);
        }
        std::cout << "Connected." << std::endl;


        // Robot Server Verbidnung / Dynamixel Ansteuerung
        DxlCon dxlCon;

        dxlCon.playAudio(DxlCon::AUDIO_HELLO);

        dxlCon.setDxlPos(DxlCon::CAM_YAW, 0);
        dxlCon.setDxlPos(DxlCon::CAM_ROLL, 0);
        dxlCon.setDxlPos(DxlCon::CAM_PITCH, 0);

        dxlCon.setDxlPos(DxlCon::NERF_PITCH, 0);
        dxlCon.setDxlPos(DxlCon::NERF_YAW, 0);
        dxlCon.setDxlPos(DxlCon::NERF_TRIGGER, 0);

        // Send idle packets
        QTimer idleTimer;
        QObject::connect(&idleTimer, SIGNAL(timeout()), &dxlCon, SLOT(sendIdle()));
        idleTimer.setInterval(1000);
        idleTimer.start();

        // Nerf Motor
        dxlCon.setNerfMotor(100);
        //QThread::msleep(1000);
        QTest::qSleep(1000);
        dxlCon.setNerfMotor(20);

        // Cam3D erstellen
        QThread threadCam;
        threadCam.setObjectName("Camera Thread");
        Cam3D cam;
        cam.moveToThread(&threadCam);
        threadCam.start();

        // renderWindow für Oculus Display
        RenderWindow render;
        QObject::connect(&cam, SIGNAL(signalImage(cv::Mat)), &render, SLOT(slotFrame(cv::Mat)));

        // Gamepad erstellen
        QThread* threadOfJoy = new QThread();
        Gamepad joystick;
        joystick.moveToThread(threadOfJoy);
        threadOfJoy->setObjectName("Thread of Joy");

        threadOfJoy->start();

        // Driving Control of the Robotino
        RobotinoControl robotinoControl;
        QObject::connect(&joystick, SIGNAL(setCarLike(double,double,double)), &robotinoControl, SLOT(setCarLike(double,double,double)));
        //QObject::connect(&joystick, SIGNAL(setInteraction(double,double)), &dxlCon, SLOT(setDxlInter(double,double)));
        QObject::connect(&joystick, SIGNAL(setTrig()), &dxlCon, SLOT(setDxlTrig()));
        QObject::connect(&joystick, SIGNAL(startNerf(int)),&dxlCon, SLOT(setNerfMotor(int)));


        // Oculus Sensoren
        OculusSensor oculus;
        QObject::connect(&oculus, SIGNAL(signalSensorData(double,double,double)), &dxlCon, SLOT(setDxlCamera(double,double,double)));
        QObject::connect(&oculus, SIGNAL(signalSensorData(double,double,double)),&dxlCon, SLOT(setDxlInter(double,double,double)));
        QObject::connect(&oculus, SIGNAL(signalRobotData(double,double,double)), &robotinoControl, SLOT(setCarLike(double,double,double)));
        QObject::connect(&joystick, SIGNAL(buttonB(bool)), &oculus, SLOT(slotbuttonB(bool)));

        // Joystick loop starten
        QMetaObject::invokeMethod(&joystick, "run");

        // das ist die hauptschleife, später vlt in eigenes qobject auf eigenem thread?
        while(com->isConnected()) {

            QCoreApplication::processEvents();
            com->processEvents();

//            rec::robotino::api2::msleep( 20 );
//            QThread::msleep(20);

        }


    } catch ( ... ) {
        std::cerr << "an error occurred." << std::endl;
        exit(1);
        rec::robotino::api2::shutdown();
    }


}
