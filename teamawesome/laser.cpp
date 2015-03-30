#include "laser.h"

#include <QtCore>
#include <QtSerialPort/QSerialPort>


Laser::Laser(QObject *parent) :
    QObject(parent)
{
    port = new QSerialPort("/dev/ttyACM0");
    port->open(QIODevice::ReadWrite);
    port->setBaudRate(9600);
}

Laser::~Laser()
{
    delete port;
}

void Laser::laserOn()
{
    laser.SendCommand(Laser::LASERON, NULL, 0, 1000);
}

void Laser::laserOff()
{
    laser.SendCommand(Laser::LASEROFF, NULL, 0, 1000);
}

double Laser::measure()
{
    double val = -1;
    laser.MeasureDistance (&val, Laser::MEAS_NORMAL);
    return val;
}

//***************************************************************************************************************************
// Calculate the CRC8 checksum for the given value. No reflection, inversion, reversion or final XOR used. Just plain CRC8.
//   input:
//     -- Data:         Data to calculate the CRC for.
//     -- InitialValue: The initial value for the CRC (e.g. previous calculated value).
//   output:
//      -- Calculated CRC8 checksum
//
//***************************************************************************************************************************
unsigned char Laser::CalcCrc8(unsigned char Data, unsigned char InitialValue)
{
    unsigned char i;

    for (i=0; i<8; i++)
    {
        if (((InitialValue & 0x80) != 0) != ((Data >> (7-i)) & 1))
        {
            InitialValue = (InitialValue << 1) ^ CRC8_POLYNOMIAL;
        } else {
            InitialValue <<= 1;
        }
    }
    return InitialValue;
}

//***************************************************************************************************************************
// Calculate the CRC8 checksum for the given array. No reflection, inversion, reversion or final XOR used. Just plain CRC8.
//   input:
//     -- pData:        Pointer to the array to calculate the CRC for.
//     -- NumBytes:     Size of array in bytes.
//     -- InitialValue: The initial value for the CRC (e.g. previous calculated value).
//   output:
//      -- Calculated CRC8 checksum
//
//***************************************************************************************************************************

unsigned char Laser::CalcCrc8FromArray(const unsigned char* pData, unsigned short int NumBytes, unsigned char InitialValue)
{
    const unsigned char* pDataTemp = pData;
    const unsigned char* pEnd = pData + NumBytes;
    while(pDataTemp < pEnd)
    {
        InitialValue = CalcCrc8(*pDataTemp++, InitialValue);
    }
    return InitialValue;
}

//***************************************************************************
// Transmit Standard Command
// timeout: in ms
//***************************************************************************
int Laser::SendCommand(COMMANDS cmd, char *rx, int rxbytes, int timeout)
{
    int status, length, txbytes;
    char sum, command[30], temp;

    port->flush();

    memcpy ( command, CMD[cmd], LEN[cmd] );
    txbytes = LEN[cmd];

    // Calculate Checksum during sending of bytes
    sum = CalcCrc8FromArray ( (unsigned char*) command, txbytes, CRC8_INITIAL_VALUE );
    port->write(command, txbytes);
    port->write(&sum, 1);

    QElapsedTimer timer;
    timer.start();
    while(port->bytesAvailable() == 0 && timer.elapsed() < timeout)
    {
        QThread::msleep(10);
    }

    qDebug() << "Command sent";

    // timeout occured
    if (port->bytesAvailable() == 0)
        return RET_ERR_TIMEOUT;

    // collect status byte
    port->read(&temp, 1);
    status = temp;
    // check status
    if (status != 0)
    {
        // error
        qWarning() << "Laser error.";
        return RET_ERR_STATUS;
    }

    // get response length
    port->read(&temp, 1);
    length = temp;

    // wait until answer is received:
    timer.start();
    while(port->bytesAvailable() < length+1  && timer.elapsed() < timeout)
    {
        QThread::msleep(10);
    }
    port->read(rx, length);

    qDebug() << "Received reply";

    return RET_NOERR;
}

//***************************************************************************
// Measure distance
//***************************************************************************
int Laser::MeasureDistance( double *Value, COMMANDS cmd )
{
    char read[11];

    // get new measurement value
    if (RET_NOERR != SendCommand( cmd, read, 10, 6000))
    {
        qWarning() << "Measurement timed out. Reconnect the device and restart the program.";
        return (RET_ERR_STATUS);
    }

    if(Value) *Value = *((unsigned long int*)read)*50e-6;

    return RET_NOERR;
}


