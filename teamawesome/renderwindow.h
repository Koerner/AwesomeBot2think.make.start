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
    cv::Mat AR727;
    int AR727_counterL;
    int AR727_counterR;
    aruco::Marker AR727_markerL;
    aruco::Marker AR727_markerR;
    cv::Mat AR726;
    int AR726_counterL;
    int AR726_counterR;
    aruco::Marker AR726_markerL;
    aruco::Marker AR726_markerR;
    cv::Mat AR725;
    int AR725_counterL;
    int AR725_counterR;
    aruco::Marker AR725_markerL;
    aruco::Marker AR725_markerR;
    cv::Mat AR724;
    int AR724_counterL;
    int AR724_counterR;
    aruco::Marker AR724_markerL;
    aruco::Marker AR724_markerR;

    static const bool AR_ACTIVATED = true;
    static const float MARRKERSIZE = 0.035;
    static const bool CALIBRATED_CAM = false;
    static const int KEEP_TIME = 15;

    aruco::CameraParameters CameraParams;
    void prepareAR();
};

#endif // RENDERWINDOW_H
