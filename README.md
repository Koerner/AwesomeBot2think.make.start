# AwesomeBot2think.make.start

API2 Docs: http://wiki.openrobotino.org/index.php?title=API2

Installation: 


sudo echo "deb http://doc.openrobotino.org/download/packages/amd64 ./" >> /etc/apt/sources.list

sudo apt-get update

sudo apt-get install robotino-api2 robotino-examples


sudo ln -s /usr/local/robotino/api2/include/rec /usr/local/include/rec

sudo ln -s /usr/local/robotino/api2/lib/librec_robotino_api2.so /usr/local/lib/librec_robotino_api2.so


sudo apt-get install libsfml-dev
