
TARGET = robotserver
TEMPLATE = app

QT += core network

LIBS += -ldxl
LIBS += -lphidget21

DEFINES += BUILDTIME=\\\"$$system(date '+%H:%M')\\\"
DEFINES += BUILDDATE=\\\"$$system(date '+%Y-%m-%d ')\\\"

SOURCES += \
    main.cpp \
    tcpserver.cpp

HEADERS += \
    tcpserver.h
