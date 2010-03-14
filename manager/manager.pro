TEMPLATE = app
TARGET = mms-manager
DEPENDPATH += . ../parser ../common
INCLUDEPATH += . ../parser ../common

include(../parser/parser.pri)
include(../common/common.pri)

QT = core dbus network

HEADERS += folder.hh wappush.hh folders.hh queuemanager.hh queuemanageradaptor.hh proxy.hh mmsprotocolhandler.hh
SOURCES += folder.cc main.cc wappush.cc queuemanager.cc queuemanageradaptor.cc proxy.cc mmsprotocolhandler.cc
