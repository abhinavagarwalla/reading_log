######################################################################
# Automatically generated by qmake (2.01a) Sun Aug 3 02:20:38 2014
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH += .

CONFIG += qwt
# Input
HEADERS += dialog.h geometry.h pose.h renderarea.h
FORMS += dialog.ui
SOURCES += dialog.cpp main.cpp pose.cpp renderarea.cpp


INCLUDEPATH += /usr/include/qwt-qt4/
LIBS += -l qwt-qt4\
        -lprotobuf
LIBS += -lgsl -lgslcblas

INCLUDEPATH += .
