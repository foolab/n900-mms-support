TEMPLATE = app
TARGET = mms-ui
DEPENDPATH += . ../parser/ ../common/ settingsdialog/ folderview/
INCLUDEPATH += . ../parser/ ../common/ settingsdialog/ folderview/

QT += dbus

include(../parser/parser.pri)
include(../common/common.pri)
include(settingsdialog/settingsdialog.pri)
include(folderview/folderview.pri)

HEADERS += mmsui.hh mainwindow.hh mmswindow.hh incoming.hh
SOURCES += main.cc mmsui.cc mainwindow.cc mmswindow.cc incoming.cc
