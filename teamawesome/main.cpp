
#include <string>
#include <iostream>

#include <rec/robotino/api2/all.h>

#include "com.h"


// Global pointer to Com object
Com* com;

int main (int argc, char** argv) {


    // Set up Robotino connection
    std::cout << "connecting to " << hostname << "... ";

    try {
        com = new Com("Awesome", "172.26.1.1");
        if(! com->isConnected() ) {
            std::cerr << "could not connect to robot" << std::endl;
            rec::robotino::api2::shutdown();
            exit(1);
        }
        std::cout << "connected." << std::endl;




    } catch ( ... ) {
        std::cerr << "an error occurred.";
        exit(1);
        rec::robotino::api2::shutdown();
    }

}
