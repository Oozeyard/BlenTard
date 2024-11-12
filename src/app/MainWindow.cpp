#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // Initialize main layout
    setWindowTitle("BlenTard");
    resize(1280, 720);
    QSplitter *splitter = new QSplitter(this);

    // menu bar
    InitMenuBar();

    // Layout
    QWidget *rightContainer = new QWidget(this);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightContainer);

    // GLWidget
    glWidget = new GLWidget(this);
    splitter->addWidget(glWidget);

    // ToolBar
    ToolBar *toolBar = new ToolBar(glWidget);
    toolBar->addTool(Tool("Move", QIcon("src/app/icons/light_krita_tool_move.svg")));
    toolBar->addTool(Tool("Rectangle", QIcon("src/app/icons/light_krita_tool_rectangle.svg")));
    toolBar->move(10, 30);
    toolBar->setFixedSize(40, 50*toolBar->GetToolsSize());

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

    splitter->setStretchFactor(0, 8);
    splitter->setStretchFactor(1, 2);

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