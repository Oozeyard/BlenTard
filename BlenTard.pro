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
    src/MainWindow.cpp \
    src/widgets/GLWidget.cpp \
    src/Mesh.cpp \
    src/Camera.cpp

HEADERS += src/widgets/GLWidget.h \
    src/includes/MainWindow.h \
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
