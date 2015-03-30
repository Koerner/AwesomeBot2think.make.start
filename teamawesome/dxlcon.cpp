#include "dxlcon.h"

#include <QtCore>
#include "iostream"
#include <QtNetwork>

const QString DxlCon::host = QString("172.26.1.1");
const int DxlCon::port = 50000;

DxlCon::DxlCon(QObject *parent) :
    QObject(parent)
{
    socket = new QTcpSocket();
    connect(socket, SIGNAL(connected()), this, SLOT(slotConnected()));

    socket->connectToHost(host, port);
    setDxl(CAM_YAW,TORQUE_LIMIT,1023);
    setDxl(CAM_YAW,POS, 2048);

    resetTime = 300;
    triggerReset = true;

    this->resetDxlTrig();
}

DxlCon::~DxlCon()
{
    socket->abort();
    delete socket;
}

void DxlCon::setDxlCamera(double yaw,double pitch,double roll)
{
    this->setDxlPos(DxlCon::CAM_YAW, yaw);
    this->setDxlPos(DxlCon::CAM_PITCH, pitch);
    this->setDxlPos(DxlCon::CAM_ROLL, roll);
    return;
}

void DxlCon::setDxlInter(double yaw, double pitch, double roll)
{
    this->setDxlPos(DxlCon::NERF_YAW, yaw);
    this->setDxlPos(DxlCon::NERF_PITCH, pitch);
    return;
}

void DxlCon::setDxlTrig()
{
    if(triggerReset)
    {
        //std::cout << "shot"<<std::endl;
        this->setDxl(DxlCon::NERF_TRIGGER, DxlCon::POS, 1300);
        QTimer::singleShot(resetTime,this,SLOT(resetDxlTrig()));
        triggerReset = false;
    }

    return;
}

void DxlCon::resetDxlTrig()
{
    this->setDxl(DxlCon::NERF_TRIGGER, DxlCon::POS, 2048);
    QTimer::singleShot(resetTime,this,SLOT(resetTrig()));
return;
}

void DxlCon::resetTrig()
{
    triggerReset = true;
    return;
}

void DxlCon::setNerfMotor(const int val)
{
    int motVal = 0;
    if(val >= 0 && val <= 100) motVal = val/2;
    setDxl(DxlCon::NERF_MOTOR, DxlCon::SET_NERF_MOT, motVal);
}


void DxlCon::setDxlPos(DxlCon::DxlId id, double angleDeg)
{
    while(angleDeg < 0) angleDeg += 360.0;
    angleDeg += 180.0; // servos are mounted reverse
    int angleVal = qRound(4096 * angleDeg / 360.0) % 4096;
    setDxl(id, POS, angleVal);
}

void DxlCon::setDxl(DxlId id, DxlCmd cmd, int val)
{
    if(id == 20 && cmd == 30 && val > 2400) val = 2400;
    if(id == 20 && cmd == 30 && val < 800) val = 800;
    if(id == 30 && cmd == 30 && val < 1500) val = 1500;
    if(id == 30 && cmd == 30 && val > 2700) val = 2700;


    QString strId = QString::number(id);
    QString strAddr = QString::number(cmd);
    QString strVal = QString::number(val);
    QString str = strId + ";" + strAddr + ";" + strVal + "\n";
    //qDebug() << "sending" << str;
    QByteArray out(str.toStdString().c_str());
    socket->write(out);
}

void DxlCon::slotConnected()
{
    qDebug() << "Dynamixel control socket is connected on" << socket->localAddress() << ":" << socket->localPort() << "to" << socket->peerAddress() << ":" << socket->peerPort();
}
