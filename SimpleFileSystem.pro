QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cdrom.cpp \
    dialogcreatefile.cpp \
    dialogrename.cpp \
    dialogshowfile.cpp \
    disk.cpp \
    fatSystem.cpp \
    inodesystem.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    cdrom.h \
    dialogcreatefile.h \
    dialogrename.h \
    dialogshowfile.h \
    disk.h \
    fatSystem.h \
    inodesystem.h \
    mainwindow.h

FORMS += \
    dialogcreatefile.ui \
    dialogrename.ui \
    dialogshowfile.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
