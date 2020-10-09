# -------------------------------------------------
# Project created by QtCreator 2010-11-19T19:43:40
# -------------------------------------------------
QT += network \
    sql \
    xml
QT -= gui
TARGET = patool
CONFIG += console
CONFIG -= app_bundle
CONFIG += debug
TEMPLATE = app
SOURCES += main.cpp \
    csettings.cpp \
    cserialdevice.cpp \
    cserial.cpp \
    cdevice.cpp \
    cvariable.cpp \
    cconsole.cpp \
    cserver.cpp \
    csession.cpp \
    ctcpdevice.cpp
HEADERS += csettings.h \
    cserialdevice.h \
    cserial.h \
    cdevice.h \
    cvariable.h \
    cconsole.h \
    cserver.h \
    csession.h \
    ctcpdevice.h \
    cfault.h

win32::SOURCES += Win32Serial.cpp
win32::HEADERS += Win32Serial.h
