QT       += core network gui widgets

TARGET = teamawesome
TEMPLATE = app

LIBS += -lrec_robotino_api2
LIBS += -ldxl
LIBS += -lsfml-graphics -lsfml-window -lsfml-system
LIBS += -lopencv_core -lopencv_imgproc -lopencv_highgui

SOURCES += \
    mainwindow.cpp \
    main.cpp \
    cam.cpp \
    cam3d.cpp \
    robotinocontrol.cpp \
    gamepad.cpp \
    renderwindow.cpp \
    dxlcon.cpp


HEADERS += \
    mainwindow.h \
    com.h \
    cam.h \
    cam3d.h \
    robotinocontrol.h \
    gamepad.h \
    renderwindow.h \
    dxlcon.h

FORMS    += mainwindow.ui
