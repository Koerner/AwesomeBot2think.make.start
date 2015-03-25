#include "dxlctrl.h"

#include <dynamixel.h>

const int DxlCtrl::dxlDeviceIndex = 99;
const int DxlCtrl::dxlBaud = 1;

DxlCtrl::DxlCtrl(QObject *parent) :
    QObject(parent)
{
    dxl_initialize(dxlDeviceIndex, dxlBaud);

    dxl_write_word(YAW, GOALPOS, 0);
}

DxlCtrl::~DxlCtrl()
{
    dxl_terminate();
}
