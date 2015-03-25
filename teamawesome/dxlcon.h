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

    enum DxlId {YAW=10, ROLL=20, PITCH=30};
    enum DxlCmd {POS=30};
    enum DxlVal {MIN=1500,MID=2000,MAX=2500};

signals:

public slots:
    void setDxl(DxlId id, DxlCmd cmd, DxlVal val);

private slots:
    void slotConnected();

private:
    QTcpSocket *socket;


};

#endif // DXLCON_H
