# -------------------------------------------------
#                     BlenTard
# -------------------------------------------------

QT += core gui opengl

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

CONFIG += c++11

TARGET = BlenTard

MOC_DIR = ./tmp/moc
OBJECTS_DIR = ./tmp/obj

SOURCES += Main.cpp \
    src/app/MainWindow.cpp \
    src/app/widgets/GLWidget.cpp \
    src/app/widgets/Inspector.cpp \
    src/app/widgets/Hierarchy.cpp \
    src/app/widgets/Tools.cpp \
    src/Node.cpp \
    src/Mesh.cpp \
    src/Model.cpp \
    src/Camera.cpp \
    src/GridOverlay.cpp \
    libs/stb_image.cpp 

HEADERS += src/app/MainWindow.h \
    src/app/widgets/GLWidget.h \
    src/app/widgets/Inspector.h \
    src/app/widgets/Hierarchy.h \
    src/app/widgets/Tools.h \
    src/includes/Transform.h \
    src/includes/Node.h \
    src/includes/Mesh.h \
    src/includes/Model.h \
    src/includes/Camera.h \
    src/includes/GridOverlay.h \
    libs/stb_image.h 

INCLUDEPATH += ./libs/glm-0.9.7.1 \
    ./libs/assimp-5.4.3/include \
    ./src \
    ./src/app \
    ./src/app/widgets \
    ./src/shaders \
    ./src/includes \
    ./libs

LIBS += -L./libs/assimp/bin -lassimp

unix: LIBS += -lGL -ldl -lpthread
win32: LIBS += -lopengl32 -lgdi32
