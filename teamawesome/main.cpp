
#include <string>
#include <iostream>

#include <QtCore>

#include <rec/robotino/api2/all.h>

#include "com.h"


// Global pointer to Com object
Com* com;

int main (int argc, char** argv) {
    Q_UNUSED(argc);
    Q_UNUSED(argv);


    try {
        // Set up Robotino connection
        com = new Com("Awesome", "172.26.1.1");
        if(! com->isConnected() ) {
            std::cerr << "could not connect to robot" << std::endl;
            rec::robotino::api2::shutdown();
            exit(1);
        }
        std::cout << "connected." << std::endl;


        // das ist die hauptschleife, später vlt in eigenes qobject auf eigenem thread?
        while(true) {
            com->processEvents();

        }

    } catch ( ... ) {
        std::cerr << "an error occurred.";
        exit(1);
        rec::robotino::api2::shutdown();
    }

}
