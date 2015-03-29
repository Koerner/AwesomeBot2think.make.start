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
    static const int timeReset;

    enum DxlId {CAM_YAW=10, CAM_ROLL=20, CAM_PITCH=30, NERF_TRIGGER=100, NERF_YAW=110, NERF_PITCH=120, NERF_MOTOR=1000};
    enum DxlCmd {POS=30, NERF_MOTOR_SET=1, TORQUE_LIMIT=34};

signals:

public slots:
    void setDxlNerf(double yaw, double pitch);

    void setDxlTrig();

    void setNerfMotor(const int val);

    void setDxlCamera(double yaw,double pitch,double roll);
    void setDxlPos(DxlId id, double angleDeg);
    void setDxl(DxlId id, DxlCmd cmd, int val);


private slots:
    void slotConnected();
    void resetTrig();
    void resetDxlTrig();



private:
    QTcpSocket *socket;

    bool triggerReset;


};

#endif // DXLCON_H
