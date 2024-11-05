#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), glWidget(new GLWidget(this)) {
    setWindowTitle("Hello World");
    setFixedSize(1280, 720);

    setCentralWidget(glWidget);

    Mesh mesh = Mesh();

}

MainWindow::~MainWindow() {}