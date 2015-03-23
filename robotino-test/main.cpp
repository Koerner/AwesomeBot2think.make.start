//  Copyright (C) 2004-2008, Robotics Equipment Corporation GmbH

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <stdlib.h>
#include <signal.h>

#include <sstream>
#include <stdio.h>
#include <stdlib.h>

#include <rec/robotino/api2/all.h>

using namespace rec::robotino::api2;

bool _run = true;

void sigint_handler( int signum )
{
    _run = false;
}

class MyCom : public Com
{
public:
    MyCom()
        : Com( "example_camera" )
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

class MyCamera : public Camera
{
public:
    MyCamera()
    {
    }

    void imageReceivedEvent( const unsigned char* data,
                                                 unsigned int dataSize,
                                                 unsigned int width,
                                                 unsigned int height,
                                                 unsigned int step )
    {
        static unsigned int seq = 0;

        std::ostringstream os;
        os << "image" << seq << ".ppm";

        FILE* fp = fopen( os.str().c_str(), "w");
        if ( fp == NULL )
        {
            std::cerr << "Error: Cannot open file " << os.str() << std::endl;
        }
        else
        {
            std::cout << "Writing image to " << os.str() << std::endl;
            fprintf(fp, "P6 %d %d 255\n", width, height);
            fwrite(data, width * height * 3, 1, fp);
        }

        ++seq;

        if( isLocalConnection() )
        {
            std::cout << "Local connection" << std::endl;
            setFormat( 320, 240, "raw" );
        }
    }
};


MyCom com;
MyCamera camera;

void init( const std::string& hostname )
{
    // Initialize the actors

    // Connect
    std::cout << "Connecting...";
    com.setAddress( hostname.c_str() );

    com.connectToServer( true );

    if( false == com.isConnected() )
    {
        std::cout << std::endl << "Could not connect to " << com.address() << std::endl;
        rec::robotino::api2::shutdown();
        exit( 1 );
    }
    else
    {
        std::cout << "success" << std::endl;
    }
}

void drive()
{
    Bumper bumper;

    while( com.isConnected() && false == bumper.value() && _run )
    {
        com.processEvents();
        rec::robotino::api2::msleep( 1000 );
    }
}

void destroy()
{
    com.disconnectFromServer();
}

int main( int argc, char **argv )
{
    std::string hostname = "172.26.1.1";
    if( argc > 1 )
    {
        hostname = argv[1];
    }

    struct sigaction act;
    memset( &act, 0, sizeof( act ) );
    act.sa_handler = sigint_handler;
    sigaction( SIGINT, &act, NULL );

    try
    {
        init( hostname );
        drive();
        destroy();
    }
    catch( const rec::robotino::api2::RobotinoException& e )
    {
        std::cerr << "Com Error: " << e.what() << std::endl;
    }
    catch( const std::exception& e )
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    catch( ... )
    {
        std::cerr << "Unknow Error" << std::endl;
    }

    rec::robotino::api2::shutdown();

#ifdef WIN32
    std::cout << "Press any key to exit..." << std::endl;
    rec::robotino::api2::waitForKey();
#endif
}
