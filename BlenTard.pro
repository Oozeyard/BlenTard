# -------------------------------------------------
#                     BlenTard
# -------------------------------------------------
QT += core gui widgets opengl
TARGET = BlenTard

INCLUDEPATH += ./app \
    ./app/widgets \
    ./source \
    ./source/shaders \
    ./source/headers \
    ./extern

SOURCES += main.cc \
    app/MainWindow.cc \
    app/widgets/GLWidget.cpp \
    source/Mesh.cpp 

HEADERS += app/MainWindow.h \
    app/widgets/GLWidget.h \
    source/headers/Mesh.h
