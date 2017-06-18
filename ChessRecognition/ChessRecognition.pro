TEMPLATE = app
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += E:\Programming\libs\opencv\release\install\include
LIBS += -LE:/Programming/libs/opencv/release/bin
LIBS += -lopencv_core320 -lopencv_highgui320 -lopencv_imgproc320 -lopencv_imgcodecs320

SOURCES += main.cpp \
    fieldfinder.cpp \
    board.cpp \
    figure.cpp

HEADERS += \
    fieldfinder.h \
    board.h \
    figure.h
