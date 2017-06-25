TEMPLATE = app
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

#WINDOWS
#INCLUDEPATH += E:\Programming\libs\opencv\release\install\include
#LIBS += -LE:/Programming/libs/opencv/release/bin
#LIBS += -lopencv_core320 -lopencv_highgui320 -lopencv_imgproc320 -lopencv_imgcodecs320

#LINUX
INCLUDEPATH += /usr/local/include/opencv2
LIBS += -L/usr/local/lib
LIBS += -lopencv_core
LIBS += -lopencv_imgproc
LIBS += -lopencv_highgui
LIBS += -lopencv_ml
LIBS += -lopencv_video
LIBS += -lopencv_features2d
LIBS += -lopencv_calib3d
LIBS += -lopencv_objdetect
LIBS += -lopencv_contrib
LIBS += -lopencv_legacy
LIBS += -lopencv_flann
#LIBS += -lopencv_nonfree

SOURCES += main.cpp \
    fieldfinder.cpp \
    board.cpp \
    figure.cpp

HEADERS += \
    fieldfinder.h \
    board.h \
    figure.h
