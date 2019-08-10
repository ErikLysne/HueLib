#-------------------------------------------------
#
# Project created by QtCreator 2019-05-25T14:26:56
#
#-------------------------------------------------

QT       += network
QT       -= gui

TARGET = HueLib
TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++14

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        Models/abstracttreemodel.cpp \
        Models/huegroupinfotreemodel.cpp \
        Models/huegrouplistmodel.cpp \
        Models/huelightinfotreemodel.cpp \
        Models/huelightlistmodel.cpp \
        Models/treeitem.cpp \
        hueabstractobject.cpp \
        huebridge.cpp \
        huegroup.cpp \
        huelight.cpp \
        huereply.cpp \
        huerequest.cpp \
        huesynchronizer.cpp \
        huetypes.cpp \
        hueerror.cpp \
    huelib.cpp

HEADERS += \
        Models/abstracttreemodel.h \
        Models/huegroupinfotreemodel.h \
        Models/huegrouplistmodel.h \
        Models/huelightinfotreemodel.h \
        Models/huelightlistmodel.h \
        Models/treeitem.h \
        hueabstractobject.h \
        huebridge.h \
        huegroup.h \
        huelib.h \
        huelight.h \
        hueobjectlist.h \
        huereply.h \
        huerequest.h \
        huesynchronizer.h \
        huetypes.h \
        hueerror.h
