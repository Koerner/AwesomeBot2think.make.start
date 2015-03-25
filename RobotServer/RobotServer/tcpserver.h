#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>

class QTcpServer;
class QAbstractSocket;
class QByteArray;

class TcpServer : public QObject
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0);
    ~TcpServer();

    static const int port;
    static const int dxlIndex, dxlBaud;

signals:

private slots:
    void slotNewConnection();
    void slotReadyRead();

private:
    enum InterpreterSuccess {OK, WRONG, UNFINISHED};

    QTcpServer *server;
    QAbstractSocket* clientConnection;
    QByteArray* buffer;

    InterpreterSuccess interpretLine(QString line);

};

#endif // TCPSERVER_H
