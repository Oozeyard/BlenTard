#include "GLWidget.h"


GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent), m_contextMenu(new Context(this)), m_currentNode(nullptr)
{
    setFocusPolicy(Qt::StrongFocus);  // Permit to receive key events
    // QWidget::setMouseTracking(true);  // Permit to receive mouse move events

    // Scene Hierachy
    m_rootNode = new Node("Root");
    
    // Context menu
    connect(m_contextMenu, &Context::actionTriggered, this, [this](const ActionType &actionType) {
        doAction(actionType);
    });

    // Painter
    m_glPainter = new GLPainter(this);
}

GLWidget::~GLWidget()
{
    makeCurrent();
    delete m_shaderProgram;
    delete m_shaderGridOverlayProgram;
    delete m_shaderSelectionProgram;
    delete m_gridOverlay;
    delete m_rootNode;
    delete m_camera;
    delete m_contextMenu;
    delete m_gizmo;
    delete m_selectionFBO;
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


    // Gizmo
    m_gizmoProgram = new Shader("./src/shaders/vertex_shader_gizmo.glsl", "./src/shaders/fragment_shader_gizmo.glsl");
    m_gizmo = new Gizmo(m_gizmoProgram);

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

    // Gizmo needs to be rendered at the end
    m_gizmoProgram->bind();
    m_gizmoProgram->setUniformValue("projection", m_camera->getProjectionMatrix());
    m_gizmoProgram->setUniformValue("view", m_camera->getViewMatrix());
    if (m_currentNode) m_gizmoProgram->setUniformValue("modelModel", m_currentNode->transform.getModelMatrix());
    m_gizmo->draw();
    m_gizmoProgram->release();

    update();
}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);  // Viewport
    m_camera->setAspect(static_cast<float>(w) / static_cast<float>(h));
    initializeSelectionBuffer();  // Resize the selection buffer
    m_glPainter->resizePainter(w, h);
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    std::cout << "Key pressed: " << event->key() << std::endl;
    // Camera input
    m_camera->keyPressEvent(event);

    m_nbTotalActiveTransform.store(0);
    
    // Key input
    switch (event->key()) 
    {
    case Qt::Key_Delete : // Delete 
        m_rootNode->deleteSelectedNodes();
        emit updateNode(m_rootNode);
        break;
    case Qt::Key_S: // Scale
        if (m_currentNode) {
            m_currentNode->transform = m_currentNodeTransform;
            m_transformMode = m_transformMode == TransformMode::Scale ? TransformMode::None : TransformMode::Scale;
            m_gizmo->setMode(m_transformMode);
            scaleNodeSelected();
        }
        break;
    case Qt::Key_R: // Rotate
        if (m_currentNode) {
            m_currentNode->transform = m_currentNodeTransform;
            m_transformMode = m_transformMode == TransformMode::Rotate ? TransformMode::None : TransformMode::Rotate;
            m_gizmo->setMode(m_transformMode);
            rotateNodeSelected();
        }
        break;
    case Qt::Key_G: // Grab (translate)
        if (m_currentNode) {
            m_currentNode->transform = m_currentNodeTransform;
            m_transformMode = m_transformMode == TransformMode::Translate ? TransformMode::None : TransformMode::Translate;
            m_gizmo->setMode(m_transformMode);
            grabNodeSelected();
        }
        break;
    case Qt::Key_E:
        std::cout << "Edit Mode" << std::endl;
        if (m_currentNode) {
            if (m_currentNode->isEditMode()) {
                m_currentNode->setEditMode();
            } else {
                m_rootNode->disableAllEditMode();
                m_currentNode->setEditMode();
            }
        }
        break;

    default:
        break;
    }

}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    // Camera input
    m_camera->mousePressEvent(event);

    // If a transformation is in progress, do not process the mouse event
    if (m_transformMode != TransformMode::None) {
        m_nbTotalActiveTransform.store(m_activeTransforms);
        return;
    }

    // Selection node
    if (event->buttons() & Qt::LeftButton && !m_isEditMode) {

        // Reset gizmo
        m_gizmo->setMode(TransformMode::None);
        
    }

    // Edit mode
    if (event->buttons() & Qt::LeftButton && m_isEditMode) {
        std::cout << "Edit Mode" << std::endl;

        // Reset gizmo
        m_gizmo->setMode(TransformMode::None);

        // TODO Selection Vertices
        /*makeCurrent();
        renderVerticesSelectionBuffer();

        int id = getObjectIdAtMouse(pos);
        doneCurrent();

        if (id != 0) { // If the id is not 0, a node has been selected
            m_rootNode->deselectAll();
            Node* node = m_rootNode->getNodeById(id);
            if (node) {
                std::cout << "Node selected: " << node->getName().toStdString() << std::endl;
                node->setSelected();
                m_currentNode = node;
                m_currentNodeTransform = node->transform;
                emit NodeSelected(node);
            }
        } else {
            m_rootNode->deselectAll();
            emit NodeSelected(nullptr);
        }*/

    }

}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    // Camera input
    m_camera->mouseMoveEvent(event);

}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{

    // If a transformation is in progress, do not process the mouse event
    if (m_transformMode != TransformMode::None) {
        m_nbTotalActiveTransform.store(m_activeTransforms);
        return;
    }

    if (event->button() == Qt::LeftButton) {
        m_selectedRect = m_glPainter->getSelectionRect();

        makeCurrent();
        renderNodeSelectionBuffer();

        QVector<int> ids = getObjectsIdInRect(m_selectedRect);
        doneCurrent();

        qDebug() << "Selected ids: " << ids;

        if (!ids.isEmpty()) { // If the id is not 0, a node has been selected
            m_rootNode->deselectAll();
            for (int id : ids) {
                Node* node = m_rootNode->getNodeById(id);
                if (node) {
                    // std::cout << "Node selected: " << node->getName().toStdString() << std::endl;
                    node->setSelected();
                    m_currentNode = node;
                    m_currentNodeTransform = node->transform;
                    emit NodeSelected(node);
                }
            }
        } else {
            m_rootNode->deselectAll();
            emit NodeSelected(nullptr);
        }

    }
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    // Camera input
    m_camera->wheelEvent(event);

}

void GLWidget::contextMenuEvent(QContextMenuEvent *event) {
    // If a transformation is in progress, do not show the context menu
    if (m_transformMode != TransformMode::None) return;

    // Show the context menu
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
    case SUBDIVIDE:
    {
        Mesh* mesh = dynamic_cast<Mesh*>(m_currentNode);
        if (mesh) {
            mesh->subdivide();
        }
        break;
    }
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


void GLWidget::renderNodeSelectionBuffer() 
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

QVector<int> GLWidget::getObjectsIdInRect(const QRect &rect)
{
    QRect validRect = rect.intersected(QRect(0, 0, width(), height()));
    if (validRect.isEmpty() || validRect.width() == 0 || validRect.height() == 0) { // Invalid rect
        return QVector<int>(); 
    }
    QVector<int> ids;

    QPoint topLeft = validRect.topLeft();
    int x = topLeft.x();
    int y = height() - topLeft.y(); // Invert the y coordinate
    int widthRect = validRect.width();
    int heightRect = validRect.height();

    widthRect = qMax(0, qMin(widthRect, width() - x));
    heightRect = qMax(0, qMin(heightRect, y));

    QVector<unsigned char> pixels(widthRect * heightRect * 3, 0);

    m_selectionFBO->bind();
    glFlush();
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(x, y - heightRect + 1, widthRect, heightRect, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
    m_selectionFBO->release();

    for (int i = 0; i < pixels.size(); i += 3) {
        int id = colorToID(pixels[i], pixels[i + 1], pixels[i + 2]);
        if (id != 0 && !ids.contains(id)) ids.push_back(id);
    }

    return ids;
}


void GLWidget::grabNodeSelected() 
{
    if (!m_currentNode) return;
    m_activeTransforms++;

    QVector3D initialPosition = m_currentNode->transform.position;

    QVector3D planePoint = initialPosition;
    QVector3D rayOrigin, rayDirection;
    QVector3D planeNormal = -m_camera->getFront();
    QPoint localPos = mapFromGlobal(QCursor::pos());
    mouseToRay(localPos, rayOrigin, rayDirection, m_camera, width(), height());

    QVector3D previousIntersection = rayPlaneIntersection(rayOrigin, rayDirection, planePoint, planeNormal);

    while (!(QApplication::mouseButtons() & Qt::LeftButton)) {
        QCoreApplication::processEvents();

        if (m_transformMode != TransformMode::Translate || m_activeTransforms < m_nbTotalActiveTransform) {
            m_lastNodeTransform.position = initialPosition;
            break;
        }
        
        QVector3D planeNormal = -m_camera->getFront();
        QPoint localPos = mapFromGlobal(QCursor::pos());
        mouseToRay(localPos, rayOrigin, rayDirection, m_camera, width(), height());
        QVector3D currentIntersection = rayPlaneIntersection(rayOrigin, rayDirection, planePoint, planeNormal);
        QVector3D delta = currentIntersection - previousIntersection;

        m_currentNode->transform.translate(delta);
        if (m_activeTransforms == m_nbTotalActiveTransform) m_lastNodeTransform = m_currentNode->transform;

        previousIntersection = currentIntersection;

        emit NodeSelected(m_currentNode);

        // Undo translation
        if (QApplication::mouseButtons() & Qt::RightButton) {
            m_currentNode->transform.position = initialPosition;
            m_lastNodeTransform.position = initialPosition;
            // std::cout << "Translation canceled" << std::endl;
            break;
        }
    }

    m_currentNode->transform = m_currentNodeTransform = m_lastNodeTransform;
    emit NodeSelected(m_currentNode);

    m_activeTransforms--; 
    if (m_activeTransforms == 0) {  // Check if all transformations are done
        m_transformMode = TransformMode::None;
        m_gizmo->setMode(TransformMode::None);
    }
}


void GLWidget::scaleNodeSelected() 
{
    if (!m_currentNode) return;
    m_activeTransforms++;

    QVector3D initialScale = m_currentNode->transform.scale;
    
    QVector3D initialPos = m_currentNode->transform.position;
    QVector3D screenPos = worldToScreen(initialPos, m_camera, width(), height());
    
    QPoint initialMousePos = mapFromGlobal(QCursor::pos());
    float initialDistance = (QVector2D(initialMousePos) - QVector2D(screenPos.x(), screenPos.y())).length();

    while (!(QApplication::mouseButtons() & Qt::LeftButton)) {
        QCoreApplication::processEvents();

        if (m_transformMode != TransformMode::Scale || m_activeTransforms < m_nbTotalActiveTransform) {
            m_lastNodeTransform.scale = initialScale;
            break;
        }

        QPoint currentMousePos = mapFromGlobal(QCursor::pos());
        float currentDistance = (QVector2D(currentMousePos) - QVector2D(screenPos.x(), screenPos.y())).length();
        float scaleFactor = currentDistance / initialDistance;
        
        m_currentNode->transform.scale = initialScale * scaleFactor;
        if (m_activeTransforms == m_nbTotalActiveTransform) m_lastNodeTransform = m_currentNode->transform;

        emit NodeSelected(m_currentNode);
        
        // Undo scaling
        if (QApplication::mouseButtons() & Qt::RightButton) {
            m_currentNode->transform.scale = initialScale;
            m_lastNodeTransform.scale = initialScale;
            // std::cout << "Scaling canceled" << std::endl;
            break;
        }

    }

    m_currentNode->transform = m_currentNodeTransform = m_lastNodeTransform;
    emit NodeSelected(m_currentNode);

    m_activeTransforms--; 
    if (m_activeTransforms == 0) {  // Check if all transformations are done
        m_transformMode = TransformMode::None;
        m_gizmo->setMode(TransformMode::None);
    }
}

void GLWidget::rotateNodeSelected() 
{
    if (!m_currentNode) return;
    m_activeTransforms++;

    QVector3D initialRotation = m_currentNode->transform.rotationEuler;
    QPoint initialMousePos = mapFromGlobal(QCursor::pos());

    while (!(QApplication::mouseButtons() & Qt::LeftButton)) {
        QCoreApplication::processEvents();

        if (m_transformMode != TransformMode::Rotate || m_activeTransforms < m_nbTotalActiveTransform) {
            m_lastNodeTransform.setRotationEuler(initialRotation);
            break;
        }

        QPoint currentMousePos = mapFromGlobal(QCursor::pos());
        float deltaX = currentMousePos.x() - initialMousePos.x();
        float deltaY = currentMousePos.y() - initialMousePos.y();

        m_currentNode->transform.rotate(QVector3D(deltaY, deltaX, 0));
        if (m_activeTransforms == m_nbTotalActiveTransform) m_lastNodeTransform = m_currentNode->transform;

        initialMousePos = currentMousePos;

        emit NodeSelected(m_currentNode);

        // Undo rotation
        if (QApplication::mouseButtons() & Qt::RightButton) {
            m_currentNode->transform.setRotationEuler(initialRotation);
            m_lastNodeTransform.setRotationEuler(initialRotation);
            // std::cout << "Rotation canceled" << std::endl;
            break;
        }
    }

    m_currentNode->transform = m_currentNodeTransform = m_lastNodeTransform;
    emit NodeSelected(m_currentNode);

    m_activeTransforms--; 
    if (m_activeTransforms == 0) {  // Check if all transformations are done
        m_transformMode = TransformMode::None;
        m_gizmo->setMode(TransformMode::None);
    }
}
