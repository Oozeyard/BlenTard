# -------------------------------------------------
#                     BlenTard
# -------------------------------------------------

QT += core gui opengl

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

CONFIG += c++11

TARGET = BlenTard

MOC_DIR = ./tmp/moc
OBJECTS_DIR = ./tmp/obj

SOURCES += main.cpp \
    src/app/MainWindow.cpp \
    srd/app/widgets/GLWidget.cpp \
    srd/app/widgets/Inspector.cpp \
    srd/app/widgets/Hierarchy.cpp \
    srd/app/widgets/Tools.cpp \
    src/Mesh.cpp \
    src/Camera.cpp

HEADERS += src/app/MainWindow.h \
    src/app/widgets/GLWidget.h \
    src/app/widgets/Inspector.h \
    src/app/widgets/Hierarchy.h \
    src/app/widgets/Tools.h \
    src/includes/Mesh.h \    
    src/includes/Camera.h

INCLUDEPATH += ./libs/glm \
    ./libs/assimp/include \
    ./src \
    ./src/widgets \
    ./src/shaders \
    ./src/includes 

LIBS += -L./libs/assimp/bin -lassimp


unix: LIBS += -lGL -ldl -lpthread
win32: LIBS += -lopengl32 -lgdi32
