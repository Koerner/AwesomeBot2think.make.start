
#include <QtCore>
#include "laser.h"

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    Laser laser;

    qDebug() << "trying to switch on laser: " <<
                laser.SendCommand(Laser::LASERON, NULL, 0, 1000);
    QThread::msleep(100);
    qDebug() << "trying to switch off laser: " <<
                laser.SendCommand(Laser::LASERON, NULL, 0, 1000);

//    qDebug() << "measurement...";
//    double val = -1;
//    int ret = laser.MeasureDistance (&val, Laser::MEAS_NORMAL);
//    qDebug() << val << "(" << ret << ")";

//    return app.exec();
    return 0;
}
