#-------------------------------------------------
#
# Project created by QtCreator 2018-12-05T12:25:54
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = GraphBuilder
TEMPLATE = app

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
        main.cpp \
        mainwindow.cpp \
    paintscene.cpp \
    node.cpp \
    itemparent.cpp \
    edge.cpp \
    matrix.cpp \
    paintgraph.cpp \
    eulerian_path.cpp \
    auth.cpp \
    ostov_tree.cpp \
    smtp.cpp \
    bfs.cpp \
    graphscene.cpp \
    dijkstra.cpp

HEADERS += \
        mainwindow.h \
    paintscene.h \
    node.h \
    itemparent.h \
    edge.h \
    matrix.h \
    paintgraph.h \
    eulerian_path.h \
    auth.h \
    ostov_tree.h \
    smtp.h \
    bfs.h \
    graphscene.h \
    dijkstra.h

FORMS += \
        mainwindow.ui \
    auth.ui

RESOURCES += \
    logo.qrc

win32: RC_ICONS = $$PWD/logo/logo.ico
