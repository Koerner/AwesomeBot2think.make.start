#include "dxltest.h"
#include "dxlcon.h"

#include <QtCore>

DxlTest::DxlTest(DxlCon *dxlCon) :
    dxlCon(dxlCon)
{ }

void DxlTest::run()
{
    forever {
        dxlCon->setDxl(20,30,2048-200);
        QThread::msleep(500);
        dxlCon->setDxl(20,30,2048+200);
        QThread::msleep(500);
    }
}

