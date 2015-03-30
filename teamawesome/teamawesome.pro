QT       += core network gui widgets testlib

TARGET = teamawesome
TEMPLATE = app

LIBS += -lrec_robotino_api2
LIBS += -ldxl
LIBS += -lsfml-graphics -lsfml-window -lsfml-system
LIBS += -lopencv_core -lopencv_imgproc -lopencv_highgui
LIBS += -lovr -lrt -lXrandr -lGL -lXxf86vm -lpthread -lX11
LIBS += -laruco

SOURCES += \
    mainwindow.cpp \
    main.cpp \
    cam.cpp \
    cam3d.cpp \
    robotinocontrol.cpp \
    gamepad.cpp \
    renderwindow.cpp \
    dxlcon.cpp \
    oculussensor.cpp


HEADERS += \
    mainwindow.h \
    com.h \
    cam.h \
    cam3d.h \
    robotinocontrol.h \
    gamepad.h \
    renderwindow.h \
    dxlcon.h \
    oculussensor.h

FORMS    += mainwindow.ui
