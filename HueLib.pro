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
        HueLib/Models/abstracttreemodel.cpp \
        HueLib/Models/huegroupinfotreemodel.cpp \
        HueLib/Models/huegrouplistmodel.cpp \
        HueLib/Models/huelightinfotreemodel.cpp \
        HueLib/Models/treeitem.cpp \
        HueLib/hueabstractobject.cpp \
        HueLib/huebridge.cpp \
        HueLib/huegroup.cpp \
        HueLib/huelight.cpp \
        HueLib/huereply.cpp \
        HueLib/huerequest.cpp \
        HueLib/huesynchronizer.cpp \
        HueLib/huetypes.cpp

HEADERS += \
        HueLib/Models/abstracttreemodel.h \
        HueLib/Models/huegroupinfotreemodel.h \
        HueLib/Models/huegrouplistmodel.h \
        HueLib/Models/huelightinfotreemodel.h \
        HueLib/Models/treeitem.h \
        HueLib/hueabstractobject.h \
        HueLib/huebridge.h \
        HueLib/huegroup.h \
        HueLib/huelib.h \
        HueLib/huelight.h \
        HueLib/hueobjectlist.h \
        HueLib/huereply.h \
        HueLib/huerequest.h \
        HueLib/huesynchronizer.h \
        HueLib/huetypes.h
