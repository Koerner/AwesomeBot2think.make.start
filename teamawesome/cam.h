#ifndef CAM_H
#define CAM_H

#include <QObject>

class Cam : public QObject, Camera
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
