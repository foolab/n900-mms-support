TEMPLATE = app
TARGET = viewer
DEPENDPATH += . ../../parser/
INCLUDEPATH += . ../../parser/

QT = core gui xml phonon

include(../../parser/parser.pri)

# Input
SOURCES += main.cc smilviewer.cc videoplayer.cc
HEADERS += smilviewer.hh videoplayer.hh
