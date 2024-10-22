QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    client.cpp \
    gridgraphic.cpp \
    link.cpp \
    main.cpp \
    mainwindow.cpp \
    mapgrid.cpp \
    movablenode.cpp

HEADERS += \
    client.h \
    gridgraphic.h \
    link.h \
    mainwindow.h \
    mapgrid.h \
    movablenode.h

FORMS += \
    gridgraphic.ui \
    mainwindow.ui \
    mapgrid.ui \
    movablenode.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
