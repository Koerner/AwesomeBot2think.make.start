#include "cam.h"

#include <sstream>
#include <iostream>

#include <cstdio>

#include <rec/robotino/api2/all.h>

Cam::Cam(QObject *parent)
    : QObject(parent),
      Camera()
{
    return;
}

void Cam::imageReceivedEvent( const unsigned char* data,
                              unsigned int dataSize,
                              unsigned int width,
                              unsigned int height,
                              unsigned int step )
{
    Q_UNUSED(dataSize);
    Q_UNUSED(step);

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
