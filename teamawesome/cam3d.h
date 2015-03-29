#ifndef CAM3D_H
#define CAM3D_H

#include <QObject>

#include <opencv2/opencv.hpp>
#include <OVR.h>

// Forward declaration
class Cam;
class QElapsedTimer;

class Cam3D : public QObject
{
    Q_OBJECT
public:
    explicit Cam3D(QObject *parent = 0);
    ~Cam3D();

    static const int outputWidth, outputHeight;
    static const int imageWidth, imageHeight;
    static const std::string imageFormat;

signals:
    void signalImage(cv::Mat img);

public slots:
    void slotImage (cv::Mat img, int source);

private:
    bool dirtyL, dirtyR;
    Cam *camL, *camR;
    cv::Mat output;
    QElapsedTimer* timer;

};

#endif // CAM3D_H
