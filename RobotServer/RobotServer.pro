
TARGET = robotserver
TEMPLATE = app

QT += core network

LIBS += -ldxl
LIBS += -lphidget21

SOURCES += \
    main.cpp \
    tcpserver.cpp

HEADERS += \
    tcpserver.h
