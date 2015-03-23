#ifndef CAM_H
#define CAM_H

#include <QObject>

#include <rec/robotino/api2/Camera.h>

class Cam : public QObject, rec::robotino::api2::Camera
{
    Q_OBJECT
public:
    explicit Cam(QObject *parent = 0);

signals:

public slots:
    void imageReceivedEvent( const unsigned char* data,
                             unsigned int dataSize,
                             unsigned int width,
                             unsigned int height,
                             unsigned int step );

};

#endif // CAM_H
