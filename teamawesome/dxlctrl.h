#ifndef DXLCTRL_H
#define DXLCTRL_H

#include <QObject>

class DxlCtrl : public QObject
{
    Q_OBJECT
public:
    explicit DxlCtrl(QObject *parent = 0);
    ~DxlCtrl();


    static const int dxlDeviceIndex;
    static const int dxlBaud;

    enum {GOALPOS=30} DxlAddr;

    enum {YAW=1, ROLL=2, PITCH=3} DxlId;

signals:

public slots:

};

#endif // DXLCTRL_H
