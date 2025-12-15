QT += core gui widgets sql

CONFIG += c++17

TARGET = CourseProject
TEMPLATE = app

DESTDIR = bin

SOURCES += \
    src/main.cpp \
    src/db/DatabaseManager.cpp

HEADERS += \
    src/db/DatabaseManager.h

# Include paths
INCLUDEPATH += src

# PostgreSQL support
unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += libpq
}

# Debug configuration
CONFIG(debug, debug|release) {
    DEFINES += DEBUG
}