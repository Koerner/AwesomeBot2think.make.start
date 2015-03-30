
TARGET = robotserver
TEMPLATE = app

QT += core network serialport

LIBS += -ldxl
LIBS += -lphidget21

DEFINES += BUILDTIME=\\\"$$system(date '+%H:%M')\\\"
DEFINES += BUILDDATE=\\\"$$system(date '+%Y-%m-%d ')\\\"

SOURCES += \
    main.cpp \
    tcpserver.cpp \
    laser.cpp

HEADERS += \
    tcpserver.h \
    laser.h
