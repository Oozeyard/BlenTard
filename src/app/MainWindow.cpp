#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // Initialize main layout
    setWindowTitle("BlenTard");
    resize(1280, 720);
    QSplitter *splitter = new QSplitter(this);

    // Layout
    QWidget *rightContainer = new QWidget(this);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightContainer);

    // OpenGL Widget Main Scene
    glWidget = new GLWidget(this);
    QSurfaceFormat format = QSurfaceFormat::defaultFormat();
    format.setSamples(16);
    glWidget->setFormat(format);
    splitter->addWidget(glWidget);

    // ToolBar
    toolBar = new ToolBar(glWidget);
    InitTools();       
    toolBar->move(10, 30);
    toolBar->setFixedSize(90, 50*(toolBar->GetToolsSize() + 10));
    connect(toolBar, &ToolBar::toolSelected, glWidget, &GLWidget::activateTool);

    // Hierarchy
    hierarchy = new Hierarchy(this);
    connect(glWidget, &GLWidget::updateNode, hierarchy, &Hierarchy::updateNode);
    connect(glWidget, &GLWidget::NodeSelected, hierarchy, &Hierarchy::selectNode);
    // Inspector
    inspector = new Inspector(this);
    connect(hierarchy, &Hierarchy::nodeSelected, inspector, &Inspector::updateTransform);
    connect(toolBar, &ToolBar::toolSelected, inspector, &Inspector::setToolInfo);
    
    rightLayout->addWidget(hierarchy);
    rightLayout->addWidget(inspector);

    connect(hierarchy, &Hierarchy::nodeSelected, glWidget, &GLWidget::setCurrentNode);

    // menu bar
    InitMenuBar();

    // QSplitter for resize
    splitter->addWidget(rightContainer);

    splitter->setStretchFactor(0, 8);
    splitter->setStretchFactor(1, 2);

    setCentralWidget(splitter);
}

void MainWindow::InitTools() {

    // Gizmo
    Tool gizmo = Tool("Gizmo", QIcon("src/app/icons/gizmo/gizmo.svg"), ToolType::GIZMO);
    Tool move = Tool("Move", QIcon("src/app/icons/gizmo/move.svg"), ToolType::MOVE);
    Tool rotate = Tool("Rotate", QIcon("src/app/icons/gizmo/rotate.svg"), ToolType::ROTATE);
    Tool scale = Tool("Scale", QIcon("src/app/icons/gizmo/scale.svg"), ToolType::SCALE);
    gizmo.addSubTool(move);
    gizmo.addSubTool(rotate);
    gizmo.addSubTool(scale);
    toolBar->addTool(gizmo);

    // Selection
    Tool select = Tool("Select", QIcon("src/app/icons/select/select_set.svg"), ToolType::SELECT);
    Tool rectangleSelect = Tool("Rectangle Select", QIcon("src/app/icons/select/rectangle_select.svg"), ToolType::RECTANGLE_SELECT);
    Tool circleSelect = Tool("Circle Select", QIcon("src/app/icons/select/circle_select.svg"), ToolType::CIRCLE_SELECT);
    Tool vertexSelect = Tool("Vertex Select", QIcon("src/app/icons/select/vertex_select.svg"), ToolType::VERTEX_SELECT);
    Tool faceSelect = Tool("Face Select", QIcon("src/app/icons/select/face_select.svg"), ToolType::FACE_SELECT);
    select.addSubTool(rectangleSelect);
    select.addSubTool(circleSelect);
    select.addSubTool(vertexSelect);
    select.addSubTool(faceSelect);
    toolBar->addTool(select);

    // Caméra
    Tool orthographic = Tool("Orthographic", QIcon("src/app/icons/view_ortho.svg"), ToolType::ORTHOGRAPHIC);
    toolBar->addTool(orthographic);
}

void MainWindow::InitMenuBar() {
    QMenuBar *menuBar = new QMenuBar(this);

    // File menu
    QMenu *fileMenu = menuBar->addMenu("File");
    QAction *newAction = fileMenu->addAction("New");
    QAction *openAction = fileMenu->addAction("Open");
    QAction *saveAction = fileMenu->addAction("Save");
    QAction *exitAction = fileMenu->addAction("Exit");
    connect(newAction, &QAction::triggered, glWidget, &GLWidget::clearScene);
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