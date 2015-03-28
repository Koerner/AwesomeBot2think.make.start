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

    enum DxlId {CAM_YAW=10, CAM_ROLL=20, CAM_PITCH=30, INTER_YAW=110, INTER_PITCH=120, INTER_TRIG=100};
    enum DxlCmd {POS=30, TORQUE_LIMIT=34};

signals:

public slots:
    void setDxlInter(double yaw, double pitch);
    void setDxlTrig();
    void setDxlCamera(double yaw,double pitch,double roll);
    void setDxlPos(DxlId id, double angleDeg);
    void setDxl(DxlId id, DxlCmd cmd, int val);


private slots:
    void slotConnected();

private:
    QTcpSocket *socket;


};

#endif // DXLCON_H
