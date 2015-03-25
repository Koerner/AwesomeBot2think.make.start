# AwesomeBot2think.make.start

API2 Docs: http://wiki.openrobotino.org/index.php?title=API2

Installation: 


sudo echo "deb http://doc.openrobotino.org/download/packages/amd64 ./" >> /etc/apt/sources.list

sudo apt-get update

sudo apt-get install robotino-api2 robotino-examples


sudo ln -s /usr/local/robotino/api2/include/rec /usr/local/include/rec

sudo ln -s /usr/local/robotino/api2/lib/librec_robotino_api2.so /usr/local/lib/librec_robotino_api2.so


sudo apt-get install libsfml-dev



oculus install

in LibOVR ordner:

make
sudo cp 90-oculus.rules /etc/udev/rules.d/90-oculus.rules
sudo cp -r Include/* /usr/local/include/
sudo cp -r Lib/Linux/Release/x86_64/libovr.a /usr/local/lib/libovr.a
