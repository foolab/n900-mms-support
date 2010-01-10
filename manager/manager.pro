TEMPLATE = app
TARGET = mms-manager
DEPENDPATH += . ../parser
INCLUDEPATH += . ../parser

QT = core dbus

include(../parser/parser.pri)

HEADERS += folder.hh incoming.hh
SOURCES += folder.cc main.cc incoming.cc
