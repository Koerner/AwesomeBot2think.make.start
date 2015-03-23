#ifndef CAM_H
#define CAM_H

#include <QObject>

#include <rec/robotino/api2/Camera.h>

#include <opencv2/opencv.hpp>


class Cam : public QObject, public rec::robotino::api2::Camera
{
    Q_OBJECT
public:
    explicit Cam(QObject *parent = 0);

signals:
    void signalImage ( cv::Mat img , int source );

public slots:
    void imageReceivedEvent( const unsigned char* data,
                             unsigned int dataSize,
                             unsigned int width,
                             unsigned int height,
                             unsigned int step );
};

#endif // CAM_H
