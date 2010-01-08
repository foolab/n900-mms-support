TEMPLATE = app
TARGET = mms-connection-manager
DEPENDPATH += .
INCLUDEPATH += .

QT = core dbus

CONFIG += link_pkgconfig
PKGCONFIG = gconf-2.0 libosso icd2 conic

HEADERS += connectionmanager.hh icd2listener.hh conicconnection.hh
SOURCES += connectionmanager.cc main.cc icd2listener.cc conicconnection.cc
