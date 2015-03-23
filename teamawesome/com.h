#ifndef COM_H
#define COM_H

#include <rec/robotino/api2/Com.h>

#include <string>

class Com : public rec::robotino::api2::Com
{
public:
    Com(std::string client_name = "awesome",
        std::string hostname = "172.26.1.1",
        bool blockingConnect = true)
        : rec::robotino::api2::Com( client_name.c_str() )
    {
        ;
        this->setAddress(hostname.c_str());
        this->connectToServer(blockingConnect);
    }
    ~Com(){
        this->disconnectFromServer();
    }

    void errorEvent( const char* errorString )
    {
        std::cerr << "Com: Error: " << errorString << std::endl;
    }

    void connectedEvent()
    {
        std::cout << "Com: Connected." << std::endl;
    }

    void connectionClosedEvent()
    {
        std::cout << "Connection closed." << std::endl;
    }

    void logEvent( const char* message, int level )
    {
        std::cout << "Com: Log (" << level << "): " << message << std::endl;
    }

    void pingEvent( float timeMs )
    {
        std::cout << "Com: Ping: " << timeMs << "ms" << std::endl;
    }
};

#endif // COM_H
