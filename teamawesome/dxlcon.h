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

    enum DxlId {YAW=10, ROLL=20, PITCH=30, TRIGGER=100, NERF_YAW=110, NERF_PITCH=120};
    enum DxlCmd {POS=30, TORQUE=34};

signals:

public slots:
    void setDxlPos(double yaw,double pitch,double roll);
    void setDxlPos(DxlId id, double angleDeg);
    void setDxl(DxlId id, DxlCmd cmd, int val);

private slots:
    void slotConnected();

private:
    QTcpSocket *socket;


};

#endif // DXLCON_H
