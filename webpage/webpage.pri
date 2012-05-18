VPATH += $$PWD
INCLUDEPATH += $$PWD

QT += webkit gui

HEADERS += \
        webpage/webpage.h \
        webpage/page.h \
    webpage/networkaccessmanager.h

SOURCES += \
        webpage/webpage.cpp \
        webpage/page.cpp \
    webpage/networkaccessmanager.cpp


