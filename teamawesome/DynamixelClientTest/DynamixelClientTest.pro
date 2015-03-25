
TARGET = dxlclient
TEMPLATE = app

QT += core network

LIBS += -ldxl

SOURCES += \
    main.cpp \
    dxlcon.cpp \
    dxltest.cpp

HEADERS += \
    dxlcon.h \
    dxltest.h
