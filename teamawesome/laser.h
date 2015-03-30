#ifndef LASER_H
#define LASER_H

#include <QObject>

class QSerialPort;

class Laser : public QObject
{
    Q_OBJECT
public:
    explicit Laser(QObject *parent = 0);
    ~Laser();

    // commands:
    typedef enum  {
      LASERON = 0,
      LASEROFF,
      MEAS_NORMAL,
      MEAS_CONT_START,
      MEAS_FAST,
      MEAS_CONT_STOP,

      NUM_CMDS
    }COMMANDS;

    void laserOn();
    void laserOff();
    double measure();

    int SendCommand(COMMANDS cmd, char *rx, int rxbytes, int timeout);
    int MeasureDistance(double *Value, COMMANDS cmd);


signals:

public slots:

private:
    QSerialPort* port;

#define  CRC8_POLYNOMIAL   0xA6
#define CRC8_INITIAL_VALUE 0xAA

    static const int RET_NOERR         = 1;
    static const int RET_ERR_TIMEOUT   =-1;
    static const int RET_ERR_STATUS    =-2;

    char CMD[NUM_CMDS][10] = {
      "\xC0\x41\x00",
      "\xC0\x42\x00",
      "\xC0\x40\x01\x00",
      "\xC0\x40\x01\x01",
      "\xC0\x40\x01\x0D",
      "\xC0\x40\x01\x02"
    };

    int LEN[NUM_CMDS] = {
      3,
      3,
      4,
      4,
      4,
      4
    };

    unsigned char CalcCrc8FromArray(const unsigned char *pData, unsigned short NumBytes, unsigned char InitialValue);
    unsigned char CalcCrc8(unsigned char Data, unsigned char InitialValue);
};

#endif // LASER_H
