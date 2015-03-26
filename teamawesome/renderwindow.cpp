#include "renderwindow.h"

#include <iostream>

#include <SFML/Graphics.hpp>
#include <opencv2/opencv.hpp>

#include "cam3d.h"

RenderWindow::RenderWindow(QObject *parent) :
    QObject(parent)
{
    window = new sf::RenderWindow(sf::VideoMode(1920, 1080 ), "Team Awesome");
    window->display();

}

RenderWindow::~RenderWindow()
{
    delete window;
}

void RenderWindow::slotFrame(cv::Mat img)
{
    //std::cout << "slotFrame" << std::endl;

    sf::Image image;
    cv::Mat frameRGBA(img);
    cv::cvtColor(img, frameRGBA, cv::COLOR_BGR2RGBA);
    image.create(frameRGBA.cols, frameRGBA.rows, frameRGBA.ptr());
    sf::Texture texture;
    texture.loadFromImage(image);
    sf::Sprite sprite;
    sprite.setTexture(texture);
    window->draw(sprite);
    window->display();
}
