#include "GLWidget.h"

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent), m_contextMenu(new Context(this))
{
    setFocusPolicy(Qt::StrongFocus);  // Permit to receive key events
    
    // Scene Hierachy
    m_rootNode = new Node("Root");
    
    // Context menu
    connect(m_contextMenu, &Context::actionTriggered, this, [this](const ActionType &actionType) {
        doAction(actionType);
    });

}

GLWidget::~GLWidget()
{
    if (m_program == nullptr)
        return;
    makeCurrent();
    if (m_program) delete m_program;
    if (m_programGridOverlay) delete m_programGridOverlay;
    if (m_selectionProgram) delete m_selectionProgram;
    if (m_gridOverlay) delete m_gridOverlay;
    if (m_rootNode) delete m_rootNode;
    if (m_camera) delete m_camera;
    if (m_contextMenu) delete m_contextMenu;
    if (m_selectionFBO) delete m_selectionFBO;
    doneCurrent();
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(800, 600);
}

void GLWidget::initShaders(QOpenGLShaderProgram *program, QString vertex_shader, QString fragment_shader)
{
    if (!program->addShaderFromSourceFile(QOpenGLShader::Vertex, vertex_shader.toStdString().c_str())) {
        qWarning() << "Error when compiling the vertex shader:" << program->log();
    }

    if (!program->addShaderFromSourceFile(QOpenGLShader::Fragment, fragment_shader.toStdString().c_str())) {
        qWarning() << "Error when compiling the fragment shader:" << program->log();
    }

    if (!program->link()) {
        qWarning() << "Error when linking the shader program:" << program->log();
    }
}


void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    // Create the principal shader program
    m_program = new QOpenGLShaderProgram;
    initShaders(m_program, "./src/shaders/vertex_shader.glsl", "./src/shaders/fragment_shader.glsl");

    // Create the selection shader program
    m_selectionProgram = new QOpenGLShaderProgram;
    initShaders(m_selectionProgram, "./src/shaders/vertex_shader_selection.glsl", "./src/shaders/fragment_shader_selection.glsl");
    initializeSelectionBuffer();
    renderSelectionBuffer();


    // Create the grid overlay shader program
    m_programGridOverlay = new QOpenGLShaderProgram;
    initShaders(m_programGridOverlay, "./src/shaders/vertex_shader_grid_overlay.glsl", "./src/shaders/fragment_shader_grid_overlay.glsl");
    m_gridOverlay = new GridOverlay(m_programGridOverlay);

    m_camera = new Camera("MainCamera");
    m_camera->setAspect(static_cast<float>(width()) / static_cast<float>(height()));

    // Model* map = new Model("Map", "models/swamp-location/source/map_1.obj"); map->transform.rotate(QVector3D(90, 0, 0));
    // Model* cube = new Model("Cube", "models/cube.obj");
    // Model* dragon = new Model("Dragon", "models/fbx/Dragon 2.5_fbx.fbx");
    // m_rootNode->addChild(map);
    // m_rootNode->addChild(cube);
    // m_rootNode->addChild(dragon);

    // Test selection 
    createCube();
    createSphere();
    m_rootNode->getChildren().first()->transform.translate(QVector3D(0, 0, 2));


    emit updateNode(m_rootNode);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);

    
    // light
    m_program->bind();
    m_program->setUniformValue("light_position", m_camera->transform.position);
    m_program->setUniformValue("light_direction", m_camera->getFront());
    m_program->release();

    m_programGridOverlay->bind();
    m_programGridOverlay->setUniformValue("projection", m_camera->getProjectionMatrix());
    m_programGridOverlay->setUniformValue("view", m_camera->getViewMatrix());
    m_gridOverlay->draw();
    m_programGridOverlay->release();

    m_program->bind();
    m_program->setUniformValue("projection", m_camera->getProjectionMatrix());
    m_program->setUniformValue("view", m_camera->getViewMatrix());
    m_rootNode->draw(m_program);
    m_program->release();

    update();
}


void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);  // Viewport
    m_camera->setAspect(static_cast<float>(w) / static_cast<float>(h));
    initializeSelectionBuffer();  // Resize the selection buffer
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    // std::cout << "Key pressed: " << event->key() << std::endl;
    switch (event->key()) {
    case Qt::Key_Delete :
        m_rootNode->deleteSelectedNodes();
        emit updateNode(m_rootNode);
        break;
    default:
        break;
    }
    m_camera->keyPressEvent(event);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    // std::cout << "Mouse pressed: " << event->button() << std::endl;
    m_camera->mousePressEvent(event);
    if (event->buttons() & Qt::LeftButton) {
        QPoint pos = event->pos();
        std::cout << "Mouse pressed: " << pos.x() << ", " << pos.y() << std::endl;

        makeCurrent();
        renderSelectionBuffer();

        int id = getObjectIdAtMouse(pos);
        doneCurrent();

        if (id != 0) { // If the id is not 0, a node has been selected
            m_rootNode->deselectAll();
            Node* node = m_rootNode->getNodeById(id);
            if (node) {
                std::cout << "Node selected: " << node->getName().toStdString() << std::endl;
                node->setSelected();
            }
        } else {
            m_rootNode->deselectAll();
        }
        
    }

}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    // std::cout << "Mouse moved: " << event->pos().x() << ", " << event->pos().y() << std::endl;
    m_camera->mouseMoveEvent(event);
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    // std::cout << "Wheel moved: " << event->angleDelta().y() << std::endl;
    m_camera->wheelEvent(event);

}

void GLWidget::contextMenuEvent(QContextMenuEvent *event) {
    m_contextMenu->showContextMenu(event->globalPos());
}

void GLWidget::activateTool(Tool* tool) {
    switch (tool->type)
    {
    case MOVE:
        
        break;
    case SELECT:
        break;
    case RECTANGLE_SELECT:
        break;
    case CIRCLE_SELECT:
        break;
    case VERTEX_SELECT:
        break;
    case FACE_SELECT:
        break;
    case ORTHOGRAPHIC:
        m_camera->setOrthographic();
        break;
    default:
        break;
    }
}
    
void GLWidget::doAction(const ActionType &actionType) {
    switch (actionType)
    {
    case ADD_CUBE: 
        createCube();
        break;
    case ADD_SPHERE:
        createSphere();
        break;
    case ADD_CUSTOM_MODEL: 
        loadCustomModel();
        break;
    case DELETE:
        m_rootNode->deleteSelectedNodes();
        break;
    default:
        break;
    }

    emit updateNode(m_rootNode);
}

void GLWidget::createCube() 
{
    makeCurrent();
    Model* model = new Model("Cube", "models/cube.obj");
    m_rootNode->addChild(static_cast<Mesh*>(model->getChildren().first()));
    doneCurrent();
}
void GLWidget::createSphere() 
{
    makeCurrent();
    Model* model = new Model("Sphere", "models/sphere.obj");
    Mesh* sphere = static_cast<Mesh*>(model->getChildren().first());
    sphere->transform.rotate(QVector3D(90, 0, 0));
    m_rootNode->addChild(sphere);
    doneCurrent();
}
void GLWidget:: loadCustomModel() 
{
    QString fileName = QFileDialog::getOpenFileName(nullptr, "Open Model", QDir::currentPath(), "Model Files (*.obj *.fbx)");
    if (fileName.isEmpty()) return;
    QString modelName = QFileInfo(fileName).baseName();
    makeCurrent();
    m_rootNode->addChild(new Model(modelName, fileName));
    doneCurrent();
}

void GLWidget::initializeSelectionBuffer() 
{
    QSize size(width(), height());
    m_selectionFBO = new QOpenGLFramebufferObject(size, QOpenGLFramebufferObject::CombinedDepthStencil);

    m_selectionTexture = m_selectionFBO->texture(); // Get the texture from the FBO

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        qWarning() << "Framebuffer not complete!";
    }
}


void GLWidget::renderSelectionBuffer() 
{
    m_selectionFBO->bind(); // Bind the framebuffer

    glEnable(GL_DEPTH_TEST); // Enable depth testing
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Clear the color buffer with black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color and depth buffer

    m_selectionProgram->bind();

    m_selectionProgram->setUniformValue("projection", m_camera->getProjectionMatrix());
    m_selectionProgram->setUniformValue("view", m_camera->getViewMatrix());

    for (Node* child : m_rootNode->getChildren()) {
        Model* model = dynamic_cast<Model*>(child);
        if (model) {
            for (Node* meshNode : model->getChildren()) {
                Mesh* mesh = dynamic_cast<Mesh*>(meshNode);
                if (mesh) {
                    mesh->draw(m_selectionProgram); // Render with unique color
                }
            }
        } else {
            Mesh* mesh = dynamic_cast<Mesh*>(child);
            if (mesh) {
                mesh->draw(m_selectionProgram); // Render with unique color
            }
        }
    }

    m_selectionProgram->release();
    m_selectionFBO->release();

    glDisable(GL_DEPTH_TEST); // Disable depth testing
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);  // Set the clear color back to the default

}

int GLWidget::getObjectIdAtMouse(const QPoint& mousePosition) 
{
    int x = mousePosition.x();
    int y = height() - mousePosition.y(); // Invert the y coordinate

    unsigned char pixel[3]; // Store the pixel color
    m_selectionFBO->bind();
    glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
    m_selectionFBO->release();

    return colorToID(pixel[0], pixel[1], pixel[2]);
}