#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QObject>

#include <SFML/Graphics/RenderWindow.hpp>
#include <opencv2/opencv.hpp>
#include <aruco/aruco.h>

class RenderWindow : public QObject
{
    Q_OBJECT
public:
    explicit RenderWindow(QObject *parent = 0);
    ~RenderWindow();

signals:

public slots:
    void slotFrame(cv::Mat img);

private:
    sf::RenderWindow *window;
    QList <cv::Mat> ImageAR;
    cv::Mat AR1;
    cv::Mat AR2;
    cv::Mat AR3;
    cv::Mat AR4;
    cv::Mat AR724;
    int AR724_counter;
    aruco::Marker AR724_marker;

    static const float MARRKERSIZE = 0.035;
    static const bool CALIBRATED_CAM = false;
    static const int KEEP_TIME = 10;

    aruco::CameraParameters CameraParams;
    void prepareAR();
};

#endif // RENDERWINDOW_H
