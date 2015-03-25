#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QObject>

#include <SFML/Graphics/RenderWindow.hpp>
#include <opencv2/opencv.hpp>

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
};

#endif // RENDERWINDOW_H
