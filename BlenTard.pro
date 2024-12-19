# -------------------------------------------------
#                     BlenTard
# -------------------------------------------------

QT += core gui opengl

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

CONFIG += c++11 release
QMAKE_CXXFLAGS_RELEASE = -O3

TARGET = BlenTard

MOC_DIR = ./tmp/moc
OBJECTS_DIR = ./tmp/obj

SOURCES += Main.cpp \
    src/app/MainWindow.cpp \
    src/app/widgets/GLWidget.cpp \
    src/app/widgets/Inspector.cpp \
    src/app/widgets/Hierarchy.cpp \
    src/app/widgets/Tools.cpp \
    src/app/widgets/Context.cpp \
    src/Node.cpp \
    src/Mesh.cpp \
    src/Model.cpp \
    src/Camera.cpp \
    src/GridOverlay.cpp \
    src/Shader.cpp \
    src/Gizmo.cpp \
    src/Utils.cpp \
    src/GLPainter.cpp \
    src/Light.cpp \
    libs/stb_image.cpp

HEADERS += src/app/MainWindow.h \
    src/app/widgets/GLWidget.h \
    src/app/widgets/Inspector.h \
    src/app/widgets/Hierarchy.h \
    src/app/widgets/Tools.h \
    src/app/widgets/Context.h \
    src/includes/Transform.h \
    src/includes/Node.h \
    src/includes/Mesh.h \  
    src/includes/Model.h \
    src/includes/Camera.h \
    src/includes/GridOverlay.h \
    src/includes/Shader.h \
    src/includes/Gizmo.h \
    src/includes/Utils.h \
    src/includes/GLPainter.h \
    src/includes/Light.h \
    libs/stb_image.h 

INCLUDEPATH += ./libs/glm-0.9.7.1 \
    ./libs \
    ./src \
    ./src/app \
    ./src/app/widgets \
    ./src/shaders \
    ./src/includes 
win32: INCLUDEPATH += ./libs/assimp-5.4.3/include

unix: LIBS += -lGL -ldl -lpthread -lassimp
win32: LIBS += -lopengl32 -lgdi32 -L./libs/assimp-5.4.3/bin -lassimp

# debuging (remove o3)
# CONFIG+=debug

