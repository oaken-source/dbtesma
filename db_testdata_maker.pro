#-------------------------------------------------
#
# Project created by QtCreator 2010-10-22T18:02:22
#
#-------------------------------------------------

QT       += core gui

TARGET = db_testdata_maker
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    datagenerator.cpp \
    FILEHANDLER/csvhandler.cpp \
    FILEHANDLER/dumphandler.cpp \
    DATA/headergenerator.cpp \
    DATA/columnhead.cpp \
    DATA/WRAPPER/intwrapper.cpp \
    DATA/WRAPPER/datatypewrapper.cpp \
    DATA/WRAPPER/varcharwrapper.cpp \
    FILEHANDLER/filehandler.cpp \
    DATA/tableheader.cpp

HEADERS  += mainwindow.h \
    datagenerator.h \
    FILEHANDLER/csvhandler.h \
    FILEHANDLER/dumphandler.h \
    MISC/macros.h \
    MISC/enums.h \
    DATA/headergenerator.h \
    DATA/columnhead.h \
    DATA/WRAPPER/intwrapper.h \
    DATA/WRAPPER/datatypewrapper.h \
    DATA/WRAPPER/varcharwrapper.h \
    FILEHANDLER/filehandler.h \
    DATA/tableheader.h

FORMS    += mainwindow.ui
