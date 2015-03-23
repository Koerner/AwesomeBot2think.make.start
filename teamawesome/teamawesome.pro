QT       += core gui
QT += widgets

TARGET = teamawesome
TEMPLATE = app

LIBS += -lrec_robotino_api2
LIBS += -lsfml-window
LIBS += -lopencv_core -lopencv_imgproc -lopencv_highgui

SOURCES += \
    mainwindow.cpp \
    main.cpp \
    cam.cpp \
    cam3d.cpp \
    robotinocontrol.cpp \
    gamepad.cpp


HEADERS += \
    mainwindow.h \
    com.h \
    cam.h \
    cam3d.h \
    robotinocontrol.h \
    gamepad.h

FORMS    += mainwindow.ui
