#include "dxlcon.h"

#include <QtCore>
#include "iostream"
#include <QtNetwork>

#include "gamepad.h"

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

    lastAuioPlayed = new QElapsedTimer;
    lastAuioPlayed->start();

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

void DxlCon::setDxlInter(double yaw, double roll, double pitch)
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

void DxlCon::sendIdle()
{
    qDebug() << "Send Idle";
    setDxl(DxlCon::IDLE, DxlCon::DONTCARE, 0);
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

void DxlCon::playAudio(const int audioID)
{
    qDebug() << "Play Audio #" << audioID;
    setDxl(DxlCon::AUDIO, DxlCon::DONTCARE, audioID);
}

void DxlCon::buttonAudio(int play)
{

    switch(play) {
    case 2:
        playAudio(DxlCon::AUDIO_AWESOME);
        break;
    case 3:
        playAudio(DxlCon::AUDIO_BACK);
        break;
    case 1:
        playAudio(DxlCon::AUDIO_HELLO);
        break;
    case 4:
        playAudio(DxlCon::AUDIO_EATSHIT);
        break;
    }
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

    if( !dxlTempVal.contains(id) )
    {
        dxlTempVal[id] = val;
    } else if( val != dxlTempVal[id] ) {
        dxlTempVal[id] = val;
    } else if ( id == DxlCon::AUDIO ) {
        if(lastAuioPlayed->elapsed() > 500)
        {
            lastAuioPlayed->restart();
        } else {
            return; // don't send this, not enough time has passed
        }
    } else if( id != DxlCon::IDLE) {
        return; // no change of value, so don't send anything
    }

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
