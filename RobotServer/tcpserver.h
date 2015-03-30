#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>

#include <phidget21.h>

class QTcpServer;
class QAbstractSocket;
class QByteArray;
class QTimer;

class TcpServer : public QObject
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0);
    ~TcpServer();

    static const int port;
    static const int phidgetTimeout;
    static const int dxlBaud;
    static const int timeout;

signals:

private slots:
    void slotNewConnection();
    void slotReadyRead();
    void slotTimeout();

private:
    enum InterpreterSuccess {OK, WRONG, UNFINISHED};

    QTcpServer *server;
    QAbstractSocket* clientConnection;
    QByteArray* buffer;

    CPhidgetMotorControlHandle motor;

    InterpreterSuccess interpretLine(QString line);
    int play(QString filename);

    QTimer* timer;
    bool timedout;

};

#endif // TCPSERVER_H
