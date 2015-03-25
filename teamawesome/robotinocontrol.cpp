#include "robotinocontrol.h"

RobotinoControl::RobotinoControl(QObject *parent) :
    QObject(parent),
    OmniDrive()
{
}

void RobotinoControl::setCarLike(double vx, double vy, double theta)
{
//    std::cout << "Set Car Like (" << vx << "mm/s , "<< vy << "mm/s , "<< theta << "deg/s )" << std::endl;
    this->setVelocity(vx,vy,theta);
    return;
}
