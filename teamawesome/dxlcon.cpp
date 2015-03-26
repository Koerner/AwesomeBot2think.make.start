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
}

DxlCon::~DxlCon()
{
    socket->abort();
    delete socket;
}

void DxlCon::setDxlPos(double yaw,double pitch,double roll)
{
    this->setDxlPos(DxlCon::YAW, yaw);
    this->setDxlPos(DxlCon::PITCH, pitch);
    this->setDxlPos(DxlCon::ROLL, roll);
    return;
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
