#ifndef ROBOTINOCONTROL_H
#define ROBOTINOCONTROL_H

#include <iostream>
#include <QObject>

#include <rec/robotino/api2/OmniDrive.h>


class RobotinoControl : public QObject, public rec::robotino::api2::OmniDrive
{
    Q_OBJECT

private:



public:
    explicit RobotinoControl(QObject *parent = 0);

signals:

public slots:
    void setCarLike(double vx, double vy, double theta);

};

#endif // ROBOTINOCONTROL_H
