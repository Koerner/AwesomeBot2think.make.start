#include "tcpserver.h"
#include <QtCore>

int main (int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    TcpServer server;
    return app.exec();
}
