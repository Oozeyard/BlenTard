#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Hello World");
    setFixedSize(1280, 720);

    setCentralWidget(glWidget);
}

MainWindow::~MainWindow() {
    delete glWidget;
}

