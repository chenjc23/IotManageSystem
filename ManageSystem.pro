QT += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets charts

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../config.cpp \
    analysewidget.cpp \
    datawidget.cpp \
    devicedatachartview.cpp \
    devicedataformview.cpp \
    devicewidget.cpp \
    editdevicewidget.cpp \
    editfuncwidget.cpp \
    editproductwidget.cpp \
    groupingwidget.cpp \
    homewidget.cpp \
    labels.cpp \
    main.cpp \
    entrywidget.cpp \
    productwidget.cpp \
    serverwidget.cpp \
    util.cpp \
    viewdevicewidget.cpp \
    viewproductwidget.cpp

HEADERS += \
    ../config.h \
    analysewidget.h \
    datawidget.h \
    devicedatachartview.h \
    devicedataformview.h \
    devicewidget.h \
    editdevicewidget.h \
    editfuncwidget.h \
    editproductwidget.h \
    entrywidget.h \
    groupingwidget.h \
    homewidget.h \
    labels.h \
    productwidget.h \
    serverwidget.h \
    util.h \
    viewdevicewidget.h \
    viewproductwidget.h

FORMS += \
    entrywidget.ui \
    homewidget.ui \
    serverwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
