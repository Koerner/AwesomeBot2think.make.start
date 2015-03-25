#include <QtCore>

#include "dxlcon.h"
#include "dxltest.h"

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    DxlCon dxlCon;

    DxlTest dxlTest(&dxlCon);
    QThread testThread;
    dxlTest.moveToThread(&testThread);
    testThread.start();
    QMetaObject::invokeMethod(&dxlTest, "run");

    return app.exec();
}
