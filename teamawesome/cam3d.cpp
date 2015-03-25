#include "cam3d.h"

#include <iostream>

#include <QtCore>

#include "cam.h"

const int Cam3D::imageWidth = 640;
const int Cam3D::imageHeight = 480;
const std::string Cam3D::imageFormat = "mjpeg";

Cam3D::Cam3D(QObject *parent) :
    QObject(parent)
{
    // Set up cameras
    if(rec::robotino::api2::Camera::numCameras() < 2){
        std::cerr << "not enough cameras (" << rec::robotino::api2::Camera::numCameras() << ") found" << std::endl;
    }

    output = cv::Mat(imageHeight, 2 * imageWidth, CV_8UC3);

    camL = new Cam();
    camR = new Cam();
    camL->setCameraNumber(0);
    camL->setFormat(imageWidth, imageHeight, imageFormat.c_str());

    qDebug() << "Cam # " << camL->numCameras();
    camR->setCameraNumber(1);
    camR->setFormat(imageWidth, imageHeight, imageFormat.c_str());
    qDebug() << "Cam # " << camR->numCameras();

    QObject::connect(camL, SIGNAL(signalImage(cv::Mat, int)), this, SLOT(slotImage(cv::Mat,int)));
    QObject::connect(camR, SIGNAL(signalImage(cv::Mat, int)), this, SLOT(slotImage(cv::Mat,int)));

    dirtyL = false;
    dirtyR = false;

    timer = new QElapsedTimer;
    timer->start();

}

Cam3D::~Cam3D()
{
    delete camL;
    delete camR;
}


void Cam3D::slotImage(cv::Mat img, int source)
{
    std::cout << "got img from " << source << std::endl;

    cv::Mat outL(output, cv::Rect(0, 0, imageWidth, imageHeight));
    cv::Mat outR(output, cv::Rect(imageWidth, 0, imageWidth, imageHeight));

    if(source == 0){
        img.copyTo(outL);
        dirtyL = true;
    } else {
        img.copyTo(outR);
        dirtyR = true;
    }

    // "sync" images
    if(!(dirtyL && dirtyR))
        return;
    dirtyL = false;
    dirtyR = false;
    qDebug()<<"send sync image";

    emit signalImage(output);
}
