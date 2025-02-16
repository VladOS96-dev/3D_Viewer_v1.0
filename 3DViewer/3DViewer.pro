macx {  
    QT       += core gui openglwidgets
    LIBS     += -framework GLUT -framework OpenGL -framework Cocoa
} else {
    QT       += core gui opengl
    LIBS     += -lOpenGL -lGLU
}

include(3rdparty/src/gifimage/qtgifimage.pri)
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    widget.cpp \
    mainwindow.cpp \
    transformations.c \
    parsing.c
HEADERS += \
    obj_parcer.h \
    widget.h \
    mainwindow.h

FORMS += mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
