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

    // OpenGL Widget Main Scene
    GLWidget *glWidget = new GLWidget(this);
    splitter->addWidget(glWidget);

    // ToolBar
    toolBar = new ToolBar(glWidget);
    InitTools();       
    toolBar->move(10, 30);
    toolBar->setFixedSize(90, 50*(toolBar->GetToolsSize() + 10));

    // Hierarchy
    Hierarchy *hierarchy = new Hierarchy(this);
    connect(glWidget, &GLWidget::rootNodeCreated, hierarchy, &Hierarchy::setRootNode);
    // hierarchy->addObject("Cube");
    // hierarchy->addObject("Camera");

    // Inspector
    Inspector *inspector = new Inspector(this, toolBar);
    
    rightLayout->addWidget(hierarchy);
    rightLayout->addWidget(inspector);


    // QSplitter for resize
    splitter->addWidget(rightContainer);

    splitter->setStretchFactor(0, 8);
    splitter->setStretchFactor(1, 2);

    setCentralWidget(splitter);
}

MainWindow::~MainWindow() {}

void MainWindow::InitTools() {
    Tool move = Tool("Move", QIcon("src/app/icons/move.svg"));
    toolBar->addTool(move);

    // Selection
    Tool select = Tool("Select", QIcon("src/app/icons/select/select_set.svg"));
    Tool rectangleSelect = Tool("Rectangle Select", QIcon("src/app/icons/select/rectangle_select.svg"));
    Tool circleSelect = Tool("Circle Select", QIcon("src/app/icons/select/circle_select.svg"));
    Tool vertexSelect = Tool("Vertex Select", QIcon("src/app/icons/select/vertex_select.svg"));
    Tool faceSelect = Tool("Face Select", QIcon("src/app/icons/select/face_select.svg"));
    select.addSubTool(rectangleSelect);
    select.addSubTool(circleSelect);
    select.addSubTool(vertexSelect);
    select.addSubTool(faceSelect);
    toolBar->addTool(select);

}

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