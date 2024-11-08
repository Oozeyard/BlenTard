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

SOURCES += main.cpp \
    app/MainWindow.cpp \
    app/widgets/GLWidget.cpp \
    app/widgets/Inspector.cpp \
    app/widgets/Hierarchy.cpp \
    app/widgets/Tools.cpp \
    source/Mesh.cpp 

HEADERS += app/MainWindow.h \
    app/widgets/GLWidget.h \
    app/widgets/Inspector.h \
    app/widgets/Hierarchy.h \
    app/widgets/Tools.h \
    source/headers/Mesh.h
