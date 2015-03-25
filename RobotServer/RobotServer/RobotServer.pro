
TARGET = robotserver
TEMPLATE = app

QT += core network

LIBS += -ldxl

SOURCES += \
    main.cpp \
    tcpserver.cpp

HEADERS += \
    tcpserver.h
