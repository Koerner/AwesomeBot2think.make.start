#include "tcpserver.h"

#include <QtNetwork>
#include <QtCore>

#include <cstdlib>

#include <dynamixel.h>

const int TcpServer::port = 50000;
const int TcpServer::dxlIndex = 0;
const int TcpServer::dxlBaud = 34; // 57k6 Bd

TcpServer::TcpServer(QObject *parent) :
    QObject(parent)
{
    int r = dxl_initialize(dxlIndex, dxlBaud);
    if(r == 0)
    {
        qCritical() << "Could not initialize dynamixel.";
//        exit(1);
    }

    buffer = new QByteArray();
    server = new QTcpServer;
    if(!server->listen(QHostAddress::Any, port))
    {
        qCritical() << "Could not start TCP server.";
        exit(1);
    }
    qDebug() << "Server started on port:" << server->serverPort();

    connect(server, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));
}

TcpServer::~TcpServer()
{
    delete server;
    delete buffer;
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

    dxl_write_word(id, addr, val);
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
