TEMPLATE = app
TARGET = mms-manager
DEPENDPATH += . ../parser
INCLUDEPATH += . ../parser

QT = core dbus

HEADERS += adaptor.hh folder.hh wappush.hh folders.hh
SOURCES += adaptor.cc folder.cc main.cc wappush.cc
