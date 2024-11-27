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
    makeCurrent();
    if (m_shaderProgram) delete m_shaderProgram;
    if (m_shaderGridOverlayProgram) delete m_shaderGridOverlayProgram;
    if (m_shaderSelectionProgram) delete m_shaderSelectionProgram;
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



void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    // Create the principal shader program
    m_shaderProgram = new Shader("./src/shaders/vertex_shader.glsl", "./src/shaders/fragment_shader.glsl");

    // Create the selection shader program
    m_shaderSelectionProgram = new Shader("./src/shaders/vertex_shader_selection.glsl", "./src/shaders/fragment_shader_selection.glsl");
    initializeSelectionBuffer();


    // Create the grid overlay shader program
    m_shaderGridOverlayProgram = new Shader("./src/shaders/vertex_shader_grid_overlay.glsl", "./src/shaders/fragment_shader_grid_overlay.glsl");
    m_gridOverlay = new GridOverlay(m_shaderGridOverlayProgram);


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
    m_shaderProgram->bind();
    m_shaderProgram->setUniformValue("light_position", m_camera->transform.position);
    m_shaderProgram->setUniformValue("light_direction", m_camera->getFront());
    m_shaderProgram->release();

    m_shaderGridOverlayProgram->bind();
    m_shaderGridOverlayProgram->setUniformValue("projection", m_camera->getProjectionMatrix());
    m_shaderGridOverlayProgram->setUniformValue("view", m_camera->getViewMatrix());
    m_gridOverlay->draw();
    m_shaderGridOverlayProgram->release();

    m_shaderProgram->bind();
    m_shaderProgram->setUniformValue("projection", m_camera->getProjectionMatrix());
    m_shaderProgram->setUniformValue("view", m_camera->getViewMatrix());
    m_rootNode->draw(m_shaderProgram);
    m_shaderProgram->release();

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
    case Qt::Key_G:
        m_isGrabbing = !m_isGrabbing;
        grabNodeSelected();
        break;
    case Qt::Key_X:
        break;
    case Qt::Key_Y:
        break;
    case Qt::Key_Z:
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
                emit NodeSelected(node);
            }
        } else {
            m_rootNode->deselectAll();
            emit NodeSelected(nullptr);
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
    if (m_isGrabbing) return;
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

    m_shaderSelectionProgram->bind();

    m_shaderSelectionProgram->setUniformValue("projection", m_camera->getProjectionMatrix());
    m_shaderSelectionProgram->setUniformValue("view", m_camera->getViewMatrix());

    for (Node* child : m_rootNode->getChildren()) {
        Model* model = dynamic_cast<Model*>(child);
        if (model) {
            for (Node* meshNode : model->getChildren()) {
                Mesh* mesh = dynamic_cast<Mesh*>(meshNode);
                if (mesh) {
                    mesh->draw(m_shaderSelectionProgram); // Render with unique color
                }
            }
        } else {
            Mesh* mesh = dynamic_cast<Mesh*>(child);
            if (mesh) {
                mesh->draw(m_shaderSelectionProgram); // Render with unique color
            }
        }
    }

    m_shaderSelectionProgram->release();
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

void GLWidget::grabNodeSelected() 
{
    QVector<Node*> nodes = m_rootNode->getSelectedNode();
    if (nodes.isEmpty()) {
        m_isGrabbing = true;
        return;
    }
    
    // Get last selected node
    Node* lastSelected = nodes.last();
    QVector3D initialPosition = lastSelected->transform.position;

    QVector3D planePoint = initialPosition;
    QVector3D rayOrigin, rayDirection;
    QVector3D planeNormal = -m_camera->getFront();
    QPoint localPos = mapFromGlobal(QCursor::pos());
    mouseToRay(localPos, rayOrigin, rayDirection);

    QVector3D previousIntersection = rayPlaneIntersection(rayOrigin, rayDirection, planePoint, planeNormal);

    while (!((QApplication::mouseButtons() & Qt::LeftButton)) && m_isGrabbing) {
        QCoreApplication::processEvents();
        QVector3D planeNormal = -m_camera->getFront();

        QPoint localPos = mapFromGlobal(QCursor::pos());
        
        mouseToRay(localPos, rayOrigin, rayDirection);
        QVector3D currentIntersection = rayPlaneIntersection(rayOrigin, rayDirection, planePoint, planeNormal);

        QVector3D delta = currentIntersection - previousIntersection;
        
        lastSelected->transform.translate(delta);

        previousIntersection = currentIntersection;

        // Undo translation
        if (QApplication::mouseButtons() & Qt::RightButton) {
            lastSelected->transform.position = initialPosition;
            std::cout << "Translation canceled" << std::endl;
            break;
        }
    }
    m_isGrabbing = false;

}

void GLWidget::mouseToRay(const QPoint& mousePos, QVector3D& rayOrigin, QVector3D& rayDirection)
{
    rayOrigin = m_camera->transform.position;

    float x = (2.0f * mousePos.x()) / width() - 1.0f;
    float y = 1.0f - (2.0f * mousePos.y()) / height();
    float z = 1.0f;
    QVector3D ray_nds(x, y, z);

    QVector4D ray_clip(ray_nds.x(), ray_nds.y(), -1.0, 1.0);
    QVector4D ray_eye = m_camera->getProjectionMatrix().inverted() * ray_clip;
    ray_eye.setZ(-1.0);
    ray_eye.setW(0.0);

    QVector4D ray_world = m_camera->getViewMatrix().inverted() * ray_eye;
    rayDirection = ray_world.toVector3D().normalized();
}

QVector3D GLWidget::rayPlaneIntersection(const QVector3D& rayOrigin, const QVector3D& rayDirection, const QVector3D& planeOrigin, const QVector3D& planeNormal)
{
    float t = QVector3D::dotProduct(planeNormal, rayOrigin - planeOrigin) / QVector3D::dotProduct(planeNormal, -rayDirection);
    return rayOrigin + t * rayDirection;
}
