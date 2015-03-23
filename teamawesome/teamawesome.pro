

TARGET = teamawesome
TEMPLATE = app

LIBS += -lrec_robotino_api2

LIBS += -lopencv_core -lopencv_imgproc -lopencv_highgui

SOURCES += \
    main.cpp \
    cam.cpp \
    cam3d.cpp

HEADERS += \
    com.h \
    cam.h \
    cam3d.h
