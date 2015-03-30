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


    AR1 = cv::imread("AR/AR1.png", CV_LOAD_IMAGE_COLOR);
    AR2 = cv::imread("AR/AR1.png", CV_LOAD_IMAGE_COLOR);
    AR3 = cv::imread("AR/AR1.png", CV_LOAD_IMAGE_COLOR);
    AR4 = cv::imread("AR/AR1.png", CV_LOAD_IMAGE_COLOR);
    AR72x = cv::imread("AR/AR72x.png", CV_LOAD_IMAGE_COLOR);
    AR72x_counterL=0;
    AR72x_counterR=0;


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
        for (unsigned int i=0;i<Markers.size();i++)
        {
            aruco::Marker temp = Markers.at(i);



            //temp.draw(img,cv::Scalar(0,0,255),2);


            switch (temp.id)
            {
            case 724:
            case 725:
            case 726:
            case 727:
            {

                tempMat = AR72x;
                qDebug()<<"Marker "<< temp.id;
                cv::Point2f center_temp = temp.getCenter();

                if(center_temp.x < ( img.cols / 2))
                {
                    tempMat = AR72x;
                    AR72x_markerL = temp;
                    qDebug() << "LINKS";
                    AR72x_counterL = KEEP_TIME;

                }

                if(center_temp.x > ( img.cols / 2))
                {
                    AR72x_markerR = temp;
                    qDebug() << "RECHTS";
                    AR72x_counterR = KEEP_TIME;
                }
            }
                break;

            default:
                qDebug() << "Diese ID ist nicht konfiguriert";
            }

        }

        //AR show AR Image

        //LINKS

        if(AR72x_counterL>0)
        {
            AR72x_counterL = AR72x_counterL - 1;
            tempMat = AR72x;


            if(!AR72x_markerL.empty())
            {
                cv::Mat tempMatResized1;
                cv::Size sizeL(AR72x_markerL.getPerimeter()/4,AR72x_markerL.getPerimeter()/4);
                cv::resize(AR72x,tempMatResized1,sizeL);
                //Center of Marker
                cv::Point2f originMarker1 = AR72x_markerL.getCenter();
                //Rectangle of image
                cv::Rect roiL( cv::Point(originMarker1.x - (tempMatResized1.size().width /2 ),
                                         originMarker1.y - (tempMatResized1.size().height /2 )),
                               tempMatResized1.size() );
                //Copy image into camera frame
                tempMatResized1.copyTo( img( roiL ) );
            }


        }

         //RECHTS

        if(AR72x_counterR>0){
            AR72x_counterR = AR72x_counterR - 1;
            tempMat = AR72x;

            if(!AR72x_markerR.empty())
            {
                cv::Mat tempMatResized2;
                cv::Size sizeR(AR72x_markerR.getPerimeter()/4,AR72x_markerR.getPerimeter()/4);
                cv::resize(AR72x,tempMatResized2,sizeR);
                //Center of Marker
                cv::Point2f originMarker2 = AR72x_markerR.getCenter();
                //Rectangle of image
                cv::Rect roiR( cv::Point(originMarker2.x - (tempMatResized2.size().width /2 ),
                                         originMarker2.y - (tempMatResized2.size().height /2 )),
                               tempMatResized2.size() );
                //Copy image into camera frame
                tempMatResized2.copyTo( img( roiR ) );
            }
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
