#include "renderwindow.h"

#include <iostream>

#include <SFML/Graphics.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

//AR
#include <aruco/aruco.h>
#include <aruco/cvdrawingutils.h>
#include <opencv2/highgui/highgui.hpp>
#include <QtCore>

#include "cam3d.h"

RenderWindow::RenderWindow(QObject *parent) :
    QObject(parent)
{
    window = new sf::RenderWindow(sf::VideoMode(1920, 1080 ), "Team Awesome");
    window->setPosition(sf::Vector2i(2000,500));
    window->display();


    prepareAR();



}

RenderWindow::~RenderWindow()
{
    delete window;
}

void RenderWindow::prepareAR()
{
    //AR Image Read

    /*for(int i = 0; i < 1024; i++)
    {
        QString path;
        path.append("/AR/AR");
        path.append(QString::number(i));
        path.append(".png");
        cv::Mat tempMat;

        try
        {
            tempMat=cv::imread("/AR/AR1.png", CV_LOAD_IMAGE_COLOR);
            ImageAR.prepend(tempMat);
            qDebug() << "Loaded image " << path;
        }
        catch (std::exception &ex)
        {
            qDebug ()  << "Image Loading failed";
        }


    }
    */

    AR1     = cv::imread("AR/AR1.png", CV_LOAD_IMAGE_COLOR);
    AR2     = cv::imread("AR/AR1.png", CV_LOAD_IMAGE_COLOR);
    AR3     = cv::imread("AR/AR1.png", CV_LOAD_IMAGE_COLOR);
    AR4     = cv::imread("AR/AR1.png", CV_LOAD_IMAGE_COLOR);
    AR724   = cv::imread("AR/AR724.png", CV_LOAD_IMAGE_COLOR);

    AR724_counter   =   0;

    //AR Calibration
    if(CALIBRATED_CAM)
    {
        try
        {

            CameraParams.readFromXMLFile("AR/camera.yml");
            qDebug() << "CamaraParams loaded";
        }
        catch (std::exception &ex)
        {
            qDebug()<<ex.what()<<endl;
        }
    }


}

void RenderWindow::slotFrame(cv::Mat img)
{
    //std::cout << "slotFrame" << std::endl;

    //AR CODE Detection
    try
    {
        aruco::MarkerDetector MDetector;
        vector<aruco::Marker> Markers;

        if(CALIBRATED_CAM)
        {
            cv::Mat imgUndis;
            cv::undistort(img,imgUndis,CameraParams.CameraMatrix,CameraParams.Distorsion);
            MDetector.detect(imgUndis,Markers);
        }
        else
        {
            MDetector.detect(img,Markers);
        }

        //for each marker, draw info and its boundaries in the image
        cv::Mat tempMat;
        aruco::Marker temp;

        for (unsigned int i=0;i<Markers.size();i++)
        {
            temp = Markers.at(i);

            qDebug()<<"Marker "<< temp.id;
            temp.draw(img,cv::Scalar(0,0,255),2);

            switch (temp.id)
            {
            case 724:
                tempMat = AR724;

                AR724_marker = temp;
                AR724_counter = KEEP_TIME;
                break;

            default:
                qDebug() << "Falsche ID";
            }

            /*if(tempMat.size().height>0)
            {
                cv::Point2f originMarker = temp.getCenter();
                cv::Rect roi( cv::Point(originMarker.x - (tempMat.size().width /2 ), originMarker.y - (tempMat.size().height /2 )), tempMat.size() );
                tempMat.copyTo( img( roi ) );
                qDebug() << "show AR Image";
            }
            else
            {
                qDebug() << "Image not loaded";
            }
            */


        }

        if(AR724_counter>0)
        {
            //
            AR724_counter = AR724_counter - 1;
            tempMat = AR724;

            // Resize the augmented image to fit the image to the area of the Marker
            cv::Mat tempMatResized;
            cv::Size size(AR724_marker.getPerimeter()/4,AR724_marker.getPerimeter()/4);
            cv::resize(AR724,tempMatResized,size);

            // Get Center of the Marker
            cv::Point2f originMarker = AR724_marker.getCenter();

            // Compute the Region of Interest for the Marker
            cv::Rect roi( cv::Point(originMarker.x - (tempMatResized.size().width /2 ), originMarker.y - (tempMatResized.size().height /2 )), tempMatResized.size() );

            // Copy the Image into the Picture
            tempMatResized.copyTo( img( roi ) );
        }

    }
    catch (std::exception &ex)
    {
        cout<<"Exception :"<<ex.what()<<endl;
    }

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
