#ifndef DXLCON_H
#define DXLCON_H

#include <QObject>

class QTcpSocket;

class DxlCon : public QObject
{
    Q_OBJECT
public:
    explicit DxlCon(QObject *parent = 0);
    ~DxlCon();

    static const QString host;
    static const int port;

signals:

public slots:
    void setDxl(int id, int addr, int val);

private slots:
    void slotConnected();

private:
    QTcpSocket *socket;

};

#endif // DXLCON_H
