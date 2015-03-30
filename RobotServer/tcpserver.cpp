#include "tcpserver.h"

#include <QtNetwork>
#include <QtCore>

#include <cstdlib>

#include <dynamixel.h>

#include <phidget21.h>

#include "laser.h"

const int TcpServer::port = 50000;
const int TcpServer::phidgetTimeout = 1000;
const int TcpServer::timeout = 5000;
const int TcpServer::dxlBaud = 34; // 57k6 Bd

TcpServer::TcpServer(QObject *parent) :
    QObject(parent)
{
    qDebug() << "Team Awesome's RobotServer, version" << BUILDDATE << BUILDTIME;

    // Laser einschalten
    try {
        laser = new Laser;
        laser->laserOn();
    } catch (...) {
        qWarning() << "laser error";
    }

    // Dynamixel init
    int index = 0, r = 0;
    while( r==0 && index < 5)
    {
        r = dxl_initialize(index++, dxlBaud);
    }
    if(r == 0)
    {
        qCritical() << "Could not initialize dynamixel.";
//        exit(1);
    }

    // Phidget init
    CPhidgetMotorControl_create(&motor);
    CPhidget_open((CPhidgetHandle) motor, -1);
    int result;
    const char* err;
    if((result = CPhidget_waitForAttachment((CPhidgetHandle) motor, phidgetTimeout)))
    {
        CPhidget_getErrorDescription(result, &err);
        qCritical() << "Problem waiting for motor attachment:" << err;
        motor = NULL;
//        exit (1);
    }

    // TCP Stream buffers init
    buffer = new QByteArray();
    server = new QTcpServer;
    connect(server, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));
    if(!server->listen(QHostAddress::Any, port))
    {
        qCritical() << "Could not start TCP server.";
        exit(1);
    }
    qDebug() << "Server started on port:" << server->serverPort();

    timer = new QTimer;
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(slotTimeout()));
    timer->start(timeout);
    timedout = true;
}

TcpServer::~TcpServer()
{
    delete timer;
    delete server;
    delete buffer;
    CPhidget_close((CPhidgetHandle) motor);
    CPhidget_delete((CPhidgetHandle) motor);
}

void TcpServer::slotNewConnection()
{
//    qDebug() << "new incoming connection";

    clientConnection = server->nextPendingConnection();
    connect(clientConnection, SIGNAL(readyRead()),
            this, SLOT(slotReadyRead()));
}

void TcpServer::slotReadyRead()
{
//    qDebug() << "new data";
    if(!clientConnection) return;
    buffer->append(clientConnection->readAll());
    QList<QString> list = QString(*buffer).split('\n', QString::SkipEmptyParts);
    while(!list.isEmpty())
    {
        InterpreterSuccess ok = interpretLine(list.first());
        if(ok == OK || ok == WRONG)
        {
            timer->start(timeout);
            list.removeFirst();
        } else if(ok == UNFINISHED) {
            break;
        }
    }
    buffer->clear();
    foreach(QString listElement, list)
    {
        buffer->append(QByteArray(listElement.toStdString().c_str()));
    }
}

void TcpServer::slotTimeout()
{
    qDebug() << "Timeout: Shut down Phidget Motors";
    if(motor)
    {
        CPhidgetMotorControl_setVelocity (motor, 0, 0);
        CPhidgetMotorControl_setVelocity (motor, 1, 0);
    }
    dxl_write_word(254, 34, 0); // Broadcast: All dynamixels loose torque
    timedout = true;

}

TcpServer::InterpreterSuccess TcpServer::interpretLine(QString line)
{
    QList<QString> parts = line.split(';', QString::SkipEmptyParts);
    if(parts.size() < 3)
    {
        qDebug() << "line" << line << "contains less than 3 parts";
        return UNFINISHED;
    }
    if(parts.size() > 3)
    {
        qDebug() << "line" << line << "contains more than 3 parts";
        return WRONG;
    }
    bool ok;

    // get DXL ID
    int id = parts.at(0).toInt(&ok);
    if(!ok)
    {
        qDebug() << "could not convert part" << parts.at(0) << "to int";
        return WRONG;
    }

    // get DXL Addr
    int addr = parts.at(1).toInt(&ok);
    if(!ok)
    {
        qDebug() << "could not convert part" << parts.at(1) << "to int";
        return WRONG;
    }

    // get DXL Val
    int val = parts.at(2).toInt(&ok);
    if(!ok)
    {
        qDebug() << "could not convert part" << parts.at(2) << "to int";
        return WRONG;
    }

    qDebug() << "Got id" << id << ", addr" << addr << ", val" << val;

    if(id == 333)
    {
        qDebug() << "Idle packet";
        return OK;
    }

    // Phidget Motor Controller gets ID 1000
    if(id == 1000)
    {
        qDebug() << "Set Phidget Motor" << addr << "to" << val;
        CPhidgetMotorControl_setVelocity (motor, addr, val);
        return OK;
    }

    // Audio Control
    if(id == 2000)
    {
        qDebug() << "Playing audio file #" << val;
        switch (val)
        {
        case 1:
            play("hello.wav");
            break;
        case 2:
            play("awesome.wav");
            break;
        case 3:
            play("back.wav");
            break;
        case 4:
            play("eatshit.wav");
            break;
        default:
            qWarning() << "unknown audio file index";
        }
    }

    qDebug() << "Got id" << id << ", addr" << addr << ", val" << val;

    dxl_write_word(id, addr, val);
    if(timedout && addr != 34)
    {
        dxl_write_word(254, 34, 1023); // Broadcast: Set max torque
        timedout = false;
    }

    int result = dxl_get_result();
    if( result == COMM_TXSUCCESS || result == COMM_RXSUCCESS )
    {
//        qDebug() << "write succeded";
    }
    else if( result == COMM_TXFAIL || result == COMM_RXFAIL || COMM_TXERROR )
    {
        qWarning() << "Dynamixel comm failed.";
    }
    else if( result == COMM_RXWAITING )
    {
    }


    return OK;
}

int TcpServer::play(QString filename)
{
    QString cmd("aplay \"/home/robotino/audio/");
    cmd.append(filename);
    cmd.append("\" &");
    return system(cmd.toStdString().c_str());
}
