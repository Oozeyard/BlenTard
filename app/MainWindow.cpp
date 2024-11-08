#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // Initialize main layout
    setWindowTitle("BlenTard");
    resize(1280, 720);
    InitMenuBar(); // Initialize menu bar
    QSplitter *splitter = new QSplitter(this);

    // glWidget
    GLWidget *glWidget = new GLWidget(this);
    splitter->addWidget(glWidget);

    QWidget *rightContainer = new QWidget(this);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightContainer);

    // Hierarchy
    Hierarchy *hierarchy = new Hierarchy(this);
    hierarchy->addObject("Cube");
    hierarchy->addObject("Camera");

    // Inspector
    Inspector *inspector = new Inspector(this);
    
    rightLayout->addWidget(hierarchy);
    rightLayout->addWidget(inspector);

    // QSplitter for resize
    
    splitter->addWidget(rightContainer);

    splitter->setStretchFactor(0, 3);
    splitter->setStretchFactor(1, 1);

    setCentralWidget(splitter);
}

MainWindow::~MainWindow() {}

void MainWindow::InitMenuBar() {
    QMenuBar *menuBar = new QMenuBar(this);

    // File menu
    QMenu *fileMenu = menuBar->addMenu("File");
    QAction *newAction = fileMenu->addAction("New");
    QAction *openAction = fileMenu->addAction("Open");
    QAction *saveAction = fileMenu->addAction("Save");
    QAction *exitAction = fileMenu->addAction("Exit");
    connect(exitAction, &QAction::triggered, this, &QMainWindow::close);

    // Edit menu
    QMenu *editMenu = menuBar->addMenu("Edit");
    QAction *undoAction = editMenu->addAction("Undo");
    QAction *redoAction = editMenu->addAction("Redo");

    // Render menu
    QMenu *renderMenu = menuBar->addMenu("Render");
    QAction *renderAction = renderMenu->addAction("Render Image");

    setMenuBar(menuBar);
}