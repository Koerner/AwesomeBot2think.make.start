#ifndef CAM3D_H
#define CAM3D_H

#include <QObject>

#include <opencv2/opencv.hpp>

// Forward declaration
class Cam;

class Cam3D : public QObject
{
    Q_OBJECT
public:
    explicit Cam3D(QObject *parent = 0);
    ~Cam3D();

    void run();

    static const int imageWidth, imageHeight;

signals:

public slots:
    void slotImage (cv::Mat img, int source);

private:
    Cam *camL, *camR;
    cv::Mat output;

};

#endif // CAM3D_H
