TEMPLATE = app
TARGET = dumpmms
DEPENDPATH += . ../parser/
INCLUDEPATH += . ../parser/

QT = core

SOURCES += main.cc

include(../parser/parser.pri)
