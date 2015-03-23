#include "cam.h"

#include <rec/robotino/api2/Camera.h>

#include <opencv2/opencv.hpp>

Cam::Cam(QObject *parent)
    : QObject(parent),
      rec::robotino::api2::Camera()
{ }


void Cam::imageReceivedEvent( const unsigned char* data,
                              unsigned int dataSize,
                              unsigned int width,
                              unsigned int height,
                              unsigned int step )
{
    Q_UNUSED(dataSize);
    Q_UNUSED(step);

    const cv::Mat mat(height, width, CV_8UC3, const_cast<unsigned char*>(data));
    cv::Mat img = mat.clone(); // make a copy of our frame
    cv::cvtColor( img, img, CV_BGR2RGB); // convert to opencv's BGR scheme

    emit signalImage(img, this->cameraNumber());
}
