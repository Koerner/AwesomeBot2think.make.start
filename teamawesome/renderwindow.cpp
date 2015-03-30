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
#include <QImage>

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

    AR724 = cv::imread("AR/A724.png", CV_LOAD_IMAGE_COLOR);
    AR724_counterL=0;
    AR724_counterR=0;

    AR727 = cv::imread("AR/A727.png", CV_LOAD_IMAGE_COLOR);
    AR727_counterL=0;
    AR727_counterR=0;

    AR726 = cv::imread("AR/A726.png", CV_LOAD_IMAGE_COLOR);
    AR726_counterL=0;
    AR726_counterR=0;

    AR725 = cv::imread("AR/A725.png", CV_LOAD_IMAGE_COLOR);
    AR725_counterL=0;
    AR725_counterR=0;

    if(AR724.empty() || AR725.empty() || AR726.empty() || AR727.empty())
    {
        qDebug() << "Loading failed!!!!";
    }
    else
    {
        qDebug() << "----> PARTY <----- (Images loaded)";
    }


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
    if(AR_ACTIVATED)
    {

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



            temp.draw(img,cv::Scalar(0,0,255),2);


            switch (temp.id)
            {
            case 724:
            {

                tempMat = AR724;
                qDebug()<<"Marker "<< temp.id;
                cv::Point2f center_temp = temp.getCenter();

                if(center_temp.x < ( img.cols / 2))
                {
                    tempMat = AR724;
                    AR724_markerL = temp;
                    qDebug() << "LINKS";
                    AR724_counterL = KEEP_TIME;

                }

                if(center_temp.x > ( img.cols / 2))
                {
                    AR724_markerR = temp;
                    qDebug() << "RECHTS";
                    AR724_counterR = KEEP_TIME;
                }
            }
                break;

            case 725:
            {

                tempMat = AR725;
                qDebug()<<"Marker "<< temp.id;
                cv::Point2f center_temp = temp.getCenter();

                if(center_temp.x < ( img.cols / 2))
                {
                    tempMat = AR725;
                    AR725_markerL = temp;
                    qDebug() << "LINKS";
                    AR725_counterL = KEEP_TIME;

                }

                if(center_temp.x > ( img.cols / 2))
                {
                    AR725_markerR = temp;
                    qDebug() << "RECHTS";
                    AR725_counterR = KEEP_TIME;
                }
            }
                break;

            case 726:
            {

                tempMat = AR726;
                qDebug()<<"Marker "<< temp.id;
                cv::Point2f center_temp = temp.getCenter();

                if(center_temp.x < ( img.cols / 2))
                {
                    tempMat = AR726;
                    AR726_markerL = temp;
                    qDebug() << "LINKS";
                    AR726_counterL = KEEP_TIME;

                }

                if(center_temp.x > ( img.cols / 2))
                {
                    AR726_markerR = temp;
                    qDebug() << "RECHTS";
                    AR726_counterR = KEEP_TIME;
                }
            }
                break;

            case 727:
            {

                tempMat = AR727;
                qDebug()<<"Marker "<< temp.id;
                cv::Point2f center_temp = temp.getCenter();

                if(center_temp.x < ( img.cols / 2))
                {
                    tempMat = AR727;
                    AR727_markerL = temp;
                    qDebug() << "LINKS";
                    AR727_counterL = KEEP_TIME;

                }

                if(center_temp.x > ( img.cols / 2))
                {
                    AR727_markerR = temp;
                    qDebug() << "RECHTS";
                    AR727_counterR = KEEP_TIME;
                }
            }
                break;

            default:
                qDebug() << "Diese ID ist nicht konfiguriert";
            }

        }

        //AR show AR Image

        //LINKS

        if(AR724_counterL>0)
        {
            AR724_counterL = AR724_counterL - 1;
            tempMat = AR724;


            if(!AR724_markerL.empty())
            {
                cv::Mat tempMatResized1;
                cv::Size sizeL(AR724_markerL.getPerimeter()/4,AR724_markerL.getPerimeter()/4);
                cv::resize(AR724,tempMatResized1,sizeL,0,0,cv::INTER_CUBIC);
                //Center of Marker
                cv::Point2f originMarker1 = AR724_markerL.getCenter();
                //Rectangle of image
                cv::Rect roiL( cv::Point(originMarker1.x - (tempMatResized1.size().width /2 ),
                                         originMarker1.y - (tempMatResized1.size().height /2 )),
                               tempMatResized1.size() );
                //Copy image into camera frame
                tempMatResized1.copyTo( img( roiL ) );
            }


        }
        if(AR725_counterL>0)
        {
            AR725_counterL = AR725_counterL - 1;
            tempMat = AR725;


            if(!AR725_markerL.empty())
            {
                cv::Mat tempMatResized1;
                cv::Size sizeL(AR725_markerL.getPerimeter()/4,AR725_markerL.getPerimeter()/4);
                cv::resize(AR725,tempMatResized1,sizeL);
                //Center of Marker
                cv::Point2f originMarker1 = AR725_markerL.getCenter();
                //Rectangle of image
                cv::Rect roiL( cv::Point(originMarker1.x - (tempMatResized1.size().width /2 ),
                                         originMarker1.y - (tempMatResized1.size().height /2 )),
                               tempMatResized1.size() );
                //Copy image into camera frame
                tempMatResized1.copyTo( img( roiL ) );
            }


        }
        if(AR726_counterL>0)
        {
            AR726_counterL = AR726_counterL - 1;
            tempMat = AR726;


            if(!AR726_markerL.empty())
            {
                cv::Mat tempMatResized1;
                cv::Size sizeL(AR726_markerL.getPerimeter()/4,AR726_markerL.getPerimeter()/4);
                cv::resize(AR726,tempMatResized1,sizeL);
                //Center of Marker
                cv::Point2f originMarker1 = AR726_markerL.getCenter();
                //Rectangle of image
                cv::Rect roiL( cv::Point(originMarker1.x - (tempMatResized1.size().width /2 ),
                                         originMarker1.y - (tempMatResized1.size().height /2 )),
                               tempMatResized1.size() );
                //Copy image into camera frame
                tempMatResized1.copyTo( img( roiL ) );
            }


        }
        if(AR727_counterL>0)
        {
            AR727_counterL = AR727_counterL - 1;
            tempMat = AR727;


            if(!AR727_markerL.empty())
            {
                cv::Mat tempMatResized1;
                cv::Size sizeL(AR727_markerL.getPerimeter()/4,AR727_markerL.getPerimeter()/4);
                cv::resize(AR727,tempMatResized1,sizeL);
                //Center of Marker
                cv::Point2f originMarker1 = AR727_markerL.getCenter();
                //Rectangle of image
                cv::Rect roiL( cv::Point(originMarker1.x - (tempMatResized1.size().width /2 ),
                                         originMarker1.y - (tempMatResized1.size().height /2 )),
                               tempMatResized1.size() );
                //Copy image into camera frame
                tempMatResized1.copyTo( img( roiL ) );
            }


        }

         //RECHTS

        if(AR724_counterR>0){
            AR724_counterR = AR724_counterR - 1;
            tempMat = AR724;

            if(!AR724_markerR.empty())
            {
                cv::Mat tempMatResized2;
                cv::Size sizeR(AR724_markerR.getPerimeter()/4,AR724_markerR.getPerimeter()/4);
                cv::resize(AR724,tempMatResized2,sizeR);
                //Center of Marker
                cv::Point2f originMarker2 = AR724_markerR.getCenter();
                //Rectangle of image
                cv::Rect roiR( cv::Point(originMarker2.x - (tempMatResized2.size().width /2 ),
                                         originMarker2.y - (tempMatResized2.size().height /2 )),
                               tempMatResized2.size() );
                //Copy image into camera frame
                tempMatResized2.copyTo( img( roiR ) );
            }
        }
        if(AR725_counterR>0){
            AR725_counterR = AR725_counterR - 1;
            tempMat = AR725;

            if(!AR725_markerR.empty())
            {
                cv::Mat tempMatResized2;
                cv::Size sizeR(AR725_markerR.getPerimeter()/4,AR725_markerR.getPerimeter()/4);
                cv::resize(AR725,tempMatResized2,sizeR);
                //Center of Marker
                cv::Point2f originMarker2 = AR725_markerR.getCenter();
                //Rectangle of image
                cv::Rect roiR( cv::Point(originMarker2.x - (tempMatResized2.size().width /2 ),
                                         originMarker2.y - (tempMatResized2.size().height /2 )),
                               tempMatResized2.size() );
                //Copy image into camera frame
                tempMatResized2.copyTo( img( roiR ) );
            }
        }
        if(AR726_counterR>0){
            AR726_counterR = AR726_counterR - 1;
            tempMat = AR726;

            if(!AR726_markerR.empty())
            {
                cv::Mat tempMatResized2;
                cv::Size sizeR(AR726_markerR.getPerimeter()/4,AR726_markerR.getPerimeter()/4);
                cv::resize(AR726,tempMatResized2,sizeR);
                //Center of Marker
                cv::Point2f originMarker2 = AR726_markerR.getCenter();
                //Rectangle of image
                cv::Rect roiR( cv::Point(originMarker2.x - (tempMatResized2.size().width /2 ),
                                         originMarker2.y - (tempMatResized2.size().height /2 )),
                               tempMatResized2.size() );
                //Copy image into camera frame
                tempMatResized2.copyTo( img( roiR ) );
            }
        }
        if(AR727_counterR>0){
            AR727_counterR = AR727_counterR - 1;
            tempMat = AR727;

            if(!AR727_markerR.empty())
            {
                cv::Mat tempMatResized2;
                cv::Size sizeR(AR727_markerR.getPerimeter()/4,AR727_markerR.getPerimeter()/4);
                cv::resize(AR727,tempMatResized2,sizeR);
                //Center of Marker
                cv::Point2f originMarker2 = AR727_markerR.getCenter();
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
