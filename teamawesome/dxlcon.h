#ifndef DXLCON_H
#define DXLCON_H

#include <QObject>
#include <QHash>

class QTcpSocket;
class QElapsedTimer;

class DxlCon : public QObject
{
    Q_OBJECT
public:
    explicit DxlCon(QObject *parent = 0);
    ~DxlCon();

    static const QString host;
    static const int port;

    enum DxlId {CAM_YAW=10, CAM_ROLL=20, CAM_PITCH=30, NERF_TRIGGER=100, NERF_YAW=110, NERF_PITCH=120, IDLE=333, NERF_MOTOR=1000, AUDIO=2000};
    enum DxlCmd {DONTCARE=0, SET_NERF_MOT=1, POS=30, TORQUE_LIMIT=34};
    enum DxlAudioID {AUDIO_HELLO=1, AUDIO_AWESOME=2, AUDIO_BACK=3, AUDIO_EATSHIT=4};

signals:

public slots:
    void setDxlInter(double yaw, double pitch, double roll);

    void setDxlTrig();

    void sendIdle();

    void setNerfMotor(const int val);

    void playAudio(const int audioID);
    void playAwesome(bool play);

    void setDxlCamera(double yaw,double pitch,double roll);
    void setDxlPos(DxlId id, double angleDeg);
    void setDxl(DxlId id, DxlCmd cmd, int val);


private slots:
    void slotConnected();
    void resetDxlTrig();
    void resetTrig();

private:
    QTcpSocket *socket;
    QElapsedTimer* lastAuioPlayed;

    int resetTime;
    bool triggerReset;

    QHash <int,int> dxlTempVal;


};

#endif // DXLCON_H
