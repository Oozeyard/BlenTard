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
           src/GLWidget.cpp \
           src/Mesh.cpp

HEADERS += includes/MainWindow.h \
           includes/GLWidget.h \
           includes/Mesh.h

INCLUDEPATH += ./libs/glm \
               ./libs/assimp/include \
               ./src \
               ./includes \
               ./shaders

LIBS += -L./libs/assimp/bin -lassimp


unix: LIBS += -lGL -ldl -lpthread
win32: LIBS += -lopengl32 -lgdi32
