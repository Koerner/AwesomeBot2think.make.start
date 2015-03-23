#ifndef COM_H
#define COM_H

#include <rec/robotino/api2/Com.h>


class Com : public rec::robotino::api2::Com
{
public:
    Com()
    : rec::robotino::api2::Com( "example_camera" )
    {
    }

    void errorEvent( const char* errorString )
    {
        std::cerr << "Error: " << errorString << std::endl;
    }

    void connectedEvent()
    {
        std::cout << "Connected." << std::endl;
    }

    void connectionClosedEvent()
    {
        std::cout << "Connection closed." << std::endl;
    }

    void logEvent( const char* message, int level )
    {
        std::cout << message << std::endl;
    }

    void pingEvent( float timeMs )
    {
        std::cout << "Ping: " << timeMs << "ms" << std::endl;
    }
};

#endif // COM_H
