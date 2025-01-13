#include "GLWidget.h"


GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)/* , m_contextMenu(new Context(this)), m_currentNode(nullptr) */
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
    std::cout << "GLWidget destructor" << std::endl;
    makeCurrent();
    std::cout << "makeCurrent" << std::endl;
    if (m_shaderSelected) delete m_shaderSelected;
    std::cout << "m_shaderSelected" << std::endl;
    if (m_shaderSolid) delete m_shaderSolid;
    std::cout << "m_shaderSolid" << std::endl;
    if (m_shaderTexture) delete m_shaderTexture;
    std::cout << "m_shaderTexture" << std::endl;
    if (m_shaderRender) delete m_shaderRender;
    std::cout << "m_shaderRender" << std::endl;
    if (m_shaderShadowMap) delete m_shaderShadowMap;
    std::cout << "m_shaderShadowMap" << std::endl;

    if (m_shaderGridOverlayProgram) delete m_shaderGridOverlayProgram;
    std::cout << "m_shaderGridOverlayProgram" << std::endl;
    if (m_shaderSelectionProgram) delete m_shaderSelectionProgram;
    std::cout << "m_shaderSelectionProgram" << std::endl;
    if (m_gizmoProgram) delete m_gizmoProgram;
    std::cout << "m_gizmoProgram" << std::endl;

    if (m_gridOverlay) delete m_gridOverlay;
    std::cout << "m_gridOverlay" << std::endl;

    if (m_rootNode) delete m_rootNode;
    std::cout << "m_rootNode" << std::endl;
    if (m_currentNode) delete m_currentNode;
    std::cout << "m_currentNode" << std::endl;

    if (m_camera) delete m_camera;
    std::cout << "m_camera" << std::endl;
    if (m_contextMenu) delete m_contextMenu;
    std::cout << "m_contextMenu" << std::endl;
    
    if (m_gizmo) delete m_gizmo;
    std::cout << "m_gizmo" << std::endl;

    if (m_glPainter) delete m_glPainter;
    std::cout << "m_glPainter" << std::endl;
    
    if (m_selectionFBO) delete m_selectionFBO;
    std::cout << "m_selectionFBO" << std::endl;
    doneCurrent();
    std::cout << "doneCurrent" << std::endl;
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

    // Create the main shader program
    m_shaderSolid = new Shader("./src/shaders/vertex_shader_solid.glsl", "./src/shaders/fragment_shader_solid.glsl");
    m_shaderTexture = new Shader("./src/shaders/vertex_shader_texture.glsl", "./src/shaders/fragment_shader_texture.glsl");
    m_shaderRender = new Shader("./src/shaders/vertex_shader_rendered.glsl", "./src/shaders/fragment_shader_rendered.glsl");

    m_shaderSelected = m_shaderSolid;
    m_shading = ShadingMode::SHADER_SOLID;

    // Create the shadowmap shader program
    m_shaderShadowMap = new Shader("./src/shaders/vertex_shader_shadowmap.glsl", "./src/shaders/fragment_shader_shadowmap.glsl");

    // Create the selection shader program
    m_shaderSelectionProgram = new Shader("./src/shaders/selection/vertex_shader_selection.glsl", "./src/shaders/selection/fragment_shader_selection.glsl");
    initializeSelectionBuffer();

    // Create the grid overlay shader program
    m_shaderGridOverlayProgram = new Shader("./src/shaders/grid/vertex_shader_grid_overlay.glsl", "./src/shaders/grid/fragment_shader_grid_overlay.glsl");
    m_gridOverlay = new GridOverlay(m_shaderGridOverlayProgram);

    // Gizmo
    m_gizmoProgram = new Shader("./src/shaders/gizmo/vertex_shader_gizmo.glsl", "./src/shaders/gizmo/fragment_shader_gizmo.glsl");
    m_gizmo = new Gizmo(m_gizmoProgram);

    m_camera = new Camera("MainCamera");
    m_camera->setAspect(static_cast<float>(width()) / static_cast<float>(height()));

    // Test selection 
    createCube();
    createSphere();
    m_rootNode->getChildren().first()->transform.translate(QVector3D(0, 0, 2));

    QString fileName = "models/fbx/Dragon 2.5_fbx.fbx";
    QString modelName = QFileInfo(fileName).baseName();
    makeCurrent();
    m_rootNode->addChild(new Model(modelName, fileName));
    doneCurrent();

    // Test light
    makeCurrent();
    Light* light = new Light("Light", Light::DIRECTIONAL);
    light->transform.translate(QVector3D(0.0f, 10.0f, 10.0f));
    m_rootNode->addChild(light);
    light->renderShadowMap(m_shaderShadowMap, m_rootNode);
    doneCurrent();


    emit updateNode(m_rootNode);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    // glDepthFunc(GL_LEQUAL);
    // glEnable(GL_CULL_FACE);
    glDisable(GL_CULL_FACE);

     
    // light
    if (m_shading != ShadingMode::SHADER_RENDER) {
        m_shaderSelected->bind();
        m_shaderSelected->setUniformValue("light_position", m_camera->transform.position);
        m_shaderSelected->setUniformValue("light_direction", m_camera->getFront());
        m_shaderSelected->release();
    }

    m_shaderGridOverlayProgram->bind();
    m_shaderGridOverlayProgram->setUniformValue("projection", m_camera->getProjectionMatrix());
    m_shaderGridOverlayProgram->setUniformValue("view", m_camera->getViewMatrix());
    m_gridOverlay->draw();
    m_shaderGridOverlayProgram->release();

    m_shaderSelected->bind();
    m_shaderSelected->setUniformValue("projection", m_camera->getProjectionMatrix());
    m_shaderSelected->setUniformValue("view", m_camera->getViewMatrix());
    m_rootNode->draw(m_shaderSelected);
    m_shaderSelected->release();

    // Gizmo needs to be rendered at the end
    if (m_currentNode && m_gizmo->getMode() != TransformMode::None) {
        m_gizmoProgram->bind();
        m_gizmoProgram->setUniformValue("projection", m_camera->getProjectionMatrix());
        m_gizmoProgram->setUniformValue("view", m_camera->getViewMatrix());
        m_gizmoProgram->setUniformValue("modelModel", m_currentNode->transform.getModelMatrix());
        m_gizmo->draw();
        m_gizmoProgram->release();
    }


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
    // std::cout << "Key pressed: " << event->key() << std::endl;

    // Camera input
    m_camera->keyPressEvent(event);

    m_nbTotalActiveTransform.store(0);

    if (event->key() == Qt::Key_Shift) m_isShitKeyPressed = true;
    
    // Key input
    switch (event->key()) 
    {
    case Qt::Key_Delete : // Delete 
        m_rootNode->deleteSelectedNodes();
        emit updateNode(m_rootNode);
        break;
    case Qt::Key_S: // Scale
        if (m_currentNode) {
            m_currentNode->transform = m_lastNodeTransform = m_initialNodeTransform;
            m_transformMode = m_transformMode == TransformMode::Scale ? TransformMode::None : TransformMode::Scale;
            if (m_isEditMode) scaleVerticesSelected();
            else {
                m_gizmo->setMode(m_transformMode);
                scaleNodeSelected();
            }
        }
        break;
    case Qt::Key_R: // Rotate
        if (m_currentNode) {
            m_currentNode->transform = m_lastNodeTransform = m_initialNodeTransform;
            m_transformMode = m_transformMode == TransformMode::Rotate ? TransformMode::None : TransformMode::Rotate;
            if (m_isEditMode) rotateVerticesSelected();
            else {
                m_gizmo->setMode(m_transformMode);
                rotateNodeSelected();
            }
        }
        break;
    case Qt::Key_G: // Grab (translate)
        if (m_currentNode) {
            qDebug() << "Grab" << m_currentNode->getName();
            m_currentNode->transform = m_lastNodeTransform = m_initialNodeTransform;
            m_transformMode = m_transformMode == TransformMode::Translate ? TransformMode::None : TransformMode::Translate;
            if (m_isEditMode) grabVerticesSelected();
            else {
                m_gizmo->setMode(m_transformMode);
                grabNodeSelected();
            }
        }
        break;
    case Qt::Key_U: // Unify
        if (m_currentNode) {
            Mesh* mesh = dynamic_cast<Mesh*>(m_currentNode);
            if (mesh) {
                mesh->unifySharedVertices();
            }
        }
        break;
    case Qt::Key_E:
        std::cout << "Edit Mode" << std::endl;
        if (m_currentNode) {
            if (m_currentNode->isEditMode()) {
                m_currentNode->setEditMode();
                m_isEditMode = false;
                m_currentNode->setSelected();
            } else {
                m_rootNode->disableAllEditMode();
                m_currentNode->setEditMode();
                m_isEditMode = true;
                m_currentNode->setSelected();
                Mesh *mesh = dynamic_cast<Mesh*>(m_currentNode);
                if (mesh) {
                    mesh->deselectAllVertices();
                }
            }
        }
        break;
    case Qt::Key_T:
        if (m_currentNode) {
            Mesh *mesh = dynamic_cast<Mesh*>(m_currentNode);
            if (mesh) mesh->TaubinSmooth();
        }
        break;

    default:
        break;
    }

}

void GLWidget::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Shift) m_isShitKeyPressed = false;
}

void GLWidget::keyPressed(QKeyEvent *event) {
    keyPressEvent(event);
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
       
        /*

        if (id != 0) { // If the id is not 0, a node has been selected
            m_rootNode->deselectAll();
            Node* node = m_rootNode->getNodeById(id);
            if (node) {
                std::cout << "Node selected: " << node->getName().toStdString() << std::endl;
                node->setSelected();
                m_currentNode = node;
                m_initialNodeTransform = node->transform;
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
        return;
    }

    if (event->button() == Qt::LeftButton && !m_isEditMode) {
        m_selectedRect = m_glPainter->getSelectionRect();

        makeCurrent();
        renderNodeSelectionBuffer();

        // QImage image = m_selectionFBO->toImage();
        // image.save("selection_buffer.png");

        QVector<int> ids = getObjectsIdInRect(m_selectedRect);
        doneCurrent();

        qDebug() << "Selected ids: " << ids;

        if (!ids.isEmpty()) { // If the id is not 0, a node has been selected
            if (!m_isShitKeyPressed) m_rootNode->deselectAll();
            for (int id : ids) {
                Node* node = m_rootNode->getNodeById(id);
                if (node) {
                    // std::cout << "Node selected: " << node->getName().toStdString() << std::endl;
                    node->setSelected();
                    m_currentNode = node;
                    m_initialNodeTransform = node->transform;
                    m_lastNodeTransform = m_initialNodeTransform;
                    emit NodeSelected(node);
                }
            }
        } else {
            m_rootNode->deselectAll();
            emit NodeSelected(nullptr);
        }
    }

    if (event->button() == Qt::LeftButton && m_isEditMode) {
        m_selectedRect = m_glPainter->getSelectionRect();

        // Reset gizmo
        m_gizmo->setMode(TransformMode::None);

        // TODO Selection Vertices
        makeCurrent();
        renderVerticesSelectionBuffer();

        // QImage image = m_selectionFBO->toImage();
        // image.save("selection_buffer.png");

        QVector<int> ids = getObjectsIdInRect(m_selectedRect);
        doneCurrent();

        qDebug() << "Selected ids: " << ids;

        Mesh* mesh = dynamic_cast<Mesh*>(m_currentNode);
        if (mesh) {
            if (!ids.isEmpty()) {
                if (!m_isShitKeyPressed) mesh->deselectAllVertices();
                for (int id : ids) {
                    mesh->selectVertex(id - 1);
                    m_selectedVertexIndices.push_back(id - 1);
                    m_initialNodeTransform = mesh->transform;
                    m_lastNodeTransform = m_initialNodeTransform;
                }
            } else {
                mesh->deselectAllVertices();
                m_selectedVertexIndices.clear();
            }
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

void GLWidget::unselectNodes() {
    m_rootNode->deselectAll();
    m_currentNode = nullptr;
    emit NodeSelected(nullptr);
}

void GLWidget::activateTool(Tool* tool) {

    switch (tool->type)
    {
    case MOVE:
        if (m_currentNode) {
            m_gizmo->setMode(TransformMode::Translate);
            m_transformMode = TransformMode::Translate;
        }
        break;
    case ROTATE:
        if (m_currentNode) {
            m_transformMode = TransformMode::Rotate;
            m_gizmo->setMode(TransformMode::Rotate);
        }
        break;
    case SCALE:
        if (m_currentNode) {
            m_transformMode = TransformMode::Scale;
            m_gizmo->setMode(TransformMode::Scale);
        }
        break;
    case RECTANGLE_SELECT:
        break;
    case VERTEX_SELECT:
        std::cout << "Edit Mode" << std::endl;
        if (m_currentNode) {
            if (m_currentNode->isEditMode()) {
                m_currentNode->setEditMode();
                m_isEditMode = false;
                m_currentNode->setSelected();
            } else {
                m_rootNode->disableAllEditMode();
                m_currentNode->setEditMode();
                m_isEditMode = true;
                m_currentNode->setSelected();
                Mesh *mesh = dynamic_cast<Mesh*>(m_currentNode);
                if (mesh) {
                    mesh->deselectAllVertices();
                }
            }
        }
        break;
    case SHADING:
        break;
    case RENDER:
        m_shading = ShadingMode::SHADER_RENDER;
        m_shaderSelected = m_shaderRender;
        break;
    case TEXTURE:
        m_shading = ShadingMode::SHADER_TEXTURE;
        m_shaderSelected = m_shaderTexture;
        break;
    case SOLID:
        m_shading = ShadingMode::SHADER_SOLID;
        m_shaderSelected = m_shaderSolid;
        break;
    case WIRE:
        m_shading = ShadingMode::SHADER_WIRE;
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
    case ADD_SUZANNE:
        createSuzanne();
        break;
    case ADD_CUSTOM_MODEL: 
        loadCustomModel();
        break;
    case ADD_DIRECTIONAL_LIGHT:
        createLight(Light::DIRECTIONAL);
        break;
    case ADD_POINT_LIGHT:
        createLight(Light::POINT);
        break;
    case ADD_SPOT_LIGHT:
        createLight(Light::SPOT);
        break;
    case UNIFY_MESH:
    {
        Mesh* mesh = dynamic_cast<Mesh*>(m_currentNode);
        if (mesh) {
            mesh->unifySharedVertices();
        }
        break;
    }
    case SUBDIVIDE:
    {
        Mesh* mesh = dynamic_cast<Mesh*>(m_currentNode);
        if (mesh) {
            mesh->subdivide();
        }
        break;
    }
    case SUBDIVIDECATMULLCLARK:
    {
        Mesh* mesh = dynamic_cast<Mesh*>(m_currentNode);
        if (mesh) {
            mesh->subdivideCatmullClark();
        }
        break;
    }
    case SUBDIVIDELOOP:
    {
        Mesh* mesh = dynamic_cast<Mesh*>(m_currentNode);
        if (mesh) {
            mesh->subdivideLoop();
        }
        break;
    }
    case EDGE_COLLAPSE_10:
    {
        Mesh* mesh = dynamic_cast<Mesh*>(m_currentNode);
        if (mesh) {
            mesh->edgeCollapse(0.9);
        }
        break;
    }
    case EDGE_COLLAPSE_20:
    {
        Mesh* mesh = dynamic_cast<Mesh*>(m_currentNode);
        if (mesh) {
            mesh->edgeCollapse(0.8);
        }
        break;
    }
    case EDGE_COLLAPSE_50:
    {
        Mesh* mesh = dynamic_cast<Mesh*>(m_currentNode);
        if (mesh) {
            mesh->edgeCollapse(0.5);
        }
        break;
    }
    case LAPLACIAN_SMOOTH:
    {
        Mesh* mesh = dynamic_cast<Mesh*>(m_currentNode);
        if (mesh) {
            mesh->LaplacianSmooth(1, 0.5f);
        }
        break;
    }
    case TAUBIN_SMOOTH:
    {
        Mesh* mesh = dynamic_cast<Mesh*>(m_currentNode);
        if (mesh) {
            mesh->TaubinSmooth(1, 0.5f, 0.53f);
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

void GLWidget::clearScene() 
{
    makeCurrent();
    // if(m_rootNode) delete m_rootNode;
    m_rootNode = new Node("Root");
    emit updateNode(m_rootNode);
    doneCurrent();
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
void GLWidget::createSuzanne() 
{
    makeCurrent();
    Model* model = new Model("Suzanne", "models/suzanne.obj");
    Mesh* suzanne = static_cast<Mesh*>(model->getChildren().first());
    m_rootNode->addChild(suzanne);
    doneCurrent();
}
void GLWidget:: loadCustomModel() 
{
    QString fileName = QFileDialog::getOpenFileName(nullptr, "Open Model", QDir::currentPath(), "Model Files (*.obj *.fbx *.gltf *.glb)");
    if (fileName.isEmpty()) return;
    QString modelName = QFileInfo(fileName).baseName();
    makeCurrent();
    m_rootNode->addChild(new Model(modelName, fileName));
    doneCurrent();
}

void GLWidget::createLight(Light::LightType type) 
{
    makeCurrent();
    Light* light = new Light("Light", type);
    m_rootNode->addChild(light);
    light->renderShadowMap(m_shaderShadowMap, m_rootNode);
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
    // glEnable(GL_CULL_FACE); 
    // glCullFace(GL_BACK);   
    // glFrontFace(GL_CCW);  
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
                    mesh->setDrawSelectionMode(DrawSelectionMode::Objects);
                    mesh->draw(m_shaderSelectionProgram); // Render with unique color
                    mesh->setDrawSelectionMode(DrawSelectionMode::None);
                }
            }
        } else {
            Mesh* mesh = dynamic_cast<Mesh*>(child);
            if (mesh) {
                mesh->setDrawSelectionMode(DrawSelectionMode::Objects);
                mesh->draw(m_shaderSelectionProgram); // Render with unique color
                mesh->setDrawSelectionMode(DrawSelectionMode::None);
            }
        }
    }

    m_shaderSelectionProgram->release();
    m_selectionFBO->release();

    glDisable(GL_DEPTH_TEST); // Disable depth testing
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);  // Set the clear color back to the default

}

void GLWidget::renderVerticesSelectionBuffer()
{
    m_selectionFBO->bind(); // Bind the framebuffer

    glEnable(GL_DEPTH_TEST); // Enable depth testing
    glEnable(GL_CULL_FACE); 
    glCullFace(GL_BACK);   
    glFrontFace(GL_CCW);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Clear the color buffer with black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color and depth buffer

    m_shaderSelectionProgram->bind();

    m_shaderSelectionProgram->setUniformValue("projection", m_camera->getProjectionMatrix());
    m_shaderSelectionProgram->setUniformValue("view", m_camera->getViewMatrix());

    Mesh* mesh = dynamic_cast<Mesh*>(m_currentNode);
    if (mesh) {
        mesh->setDrawSelectionMode(DrawSelectionMode::UseCulling);
        mesh->draw(m_shaderSelectionProgram);

        mesh->setDrawSelectionMode(DrawSelectionMode::Vertices);
        mesh->draw(m_shaderSelectionProgram); // Render with unique color
        mesh->setDrawSelectionMode(DrawSelectionMode::None);
    }

    m_shaderSelectionProgram->release();
    m_selectionFBO->release();

    glDisable(GL_CULL_FACE); 
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
        // pixel(0, 0, 0) and pixel(255, 255, 255) ignored
        if (id != 0 && id != 16777215 && !ids.contains(id)) ids.push_back(id); 
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
        if (m_activeTransforms == m_nbTotalActiveTransform) m_lastNodeTransform.position = m_currentNode->transform.position;

        previousIntersection = currentIntersection;

        emit NodeSelected(m_currentNode);

        // Undo translation
        if (QApplication::mouseButtons() & Qt::RightButton) {
            m_lastNodeTransform.position = initialPosition;
            // std::cout << "Translation canceled" << std::endl;
            break;
        }
    }

    if (m_activeTransforms == m_nbTotalActiveTransform) m_currentNode->transform = m_initialNodeTransform = m_lastNodeTransform;
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
        if (m_activeTransforms == m_nbTotalActiveTransform) m_lastNodeTransform.scale = m_currentNode->transform.scale;

        emit NodeSelected(m_currentNode);
        
        // Undo scaling
        if (QApplication::mouseButtons() & Qt::RightButton) {
            m_lastNodeTransform.scale = initialScale;
            // std::cout << "Scaling canceled" << std::endl;
            break;
        }

    }

    if (m_activeTransforms == m_nbTotalActiveTransform) m_currentNode->transform = m_initialNodeTransform = m_lastNodeTransform;
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
        if (m_activeTransforms == m_nbTotalActiveTransform) m_lastNodeTransform.setRotationEuler(m_currentNode->transform.rotationEuler);

        initialMousePos = currentMousePos;

        emit NodeSelected(m_currentNode);

        // Undo rotation
        if (QApplication::mouseButtons() & Qt::RightButton) {
            m_lastNodeTransform.setRotationEuler(initialRotation);
            // std::cout << "Rotation canceled" << std::endl;
            break;
        }
    }

    if (m_activeTransforms == m_nbTotalActiveTransform) m_currentNode->transform = m_initialNodeTransform = m_lastNodeTransform;
    emit NodeSelected(m_currentNode);

    m_activeTransforms--; 
    if (m_activeTransforms == 0) {  // Check if all transformations are done
        m_transformMode = TransformMode::None;
        m_gizmo->setMode(TransformMode::None);
    }
}

void GLWidget::grabVerticesSelected()
{
    if (m_selectedVertexIndices.isEmpty()) return;
    m_activeTransforms++;

    Mesh* mesh = dynamic_cast<Mesh*>(m_currentNode);

    QVector<QVector3D> initialPositions;
    for (int index : m_selectedVertexIndices) initialPositions.append(mesh->getVertices()[index].position);

    QVector<QVector3D> verticesPositions = initialPositions;
    
    QVector3D planePoint = initialPositions.last();
    QVector3D rayOrigin, rayDirection;
    QVector3D planeNormal = -m_camera->getFront();
    QPoint localPos = mapFromGlobal(QCursor::pos());
    mouseToRay(localPos, rayOrigin, rayDirection, m_camera, width(), height());

    QVector3D previousIntersection = rayPlaneIntersection(rayOrigin, rayDirection, planePoint, planeNormal);

    int nbVertices = m_selectedVertexIndices.size();
    
    while (!(QApplication::mouseButtons() & Qt::LeftButton)) {
        QCoreApplication::processEvents();

        if (m_transformMode != TransformMode::Translate || m_activeTransforms < m_nbTotalActiveTransform) {
            // m_lastNodeTransform.position = initialPositions.last();
            for (int i = 0; i < nbVertices; ++i) {
                mesh->updateVertexPosition(m_selectedVertexIndices[i], initialPositions[i]);
            }
            
            m_activeTransforms--; 
            if (m_activeTransforms == 0) {  // Check if all transformations are done
                m_transformMode = TransformMode::None;
            }

            return;
        }
        
        QVector3D planeNormal = -m_camera->getFront();
        QPoint localPos = mapFromGlobal(QCursor::pos());
        mouseToRay(localPos, rayOrigin, rayDirection, m_camera, width(), height());
        QVector3D currentIntersection = rayPlaneIntersection(rayOrigin, rayDirection, planePoint, planeNormal);
        QVector3D delta = currentIntersection - previousIntersection;

        for (int i = 0; i < nbVertices; ++i) {
            verticesPositions[i] += delta;
            mesh->updateVertexPosition(m_selectedVertexIndices[i], verticesPositions[i]);
        }

        update();
        if (m_activeTransforms == m_nbTotalActiveTransform) m_lastNodeTransform.position = initialPositions.last();

        previousIntersection = currentIntersection;

        // Undo translation
        if (QApplication::mouseButtons() & Qt::RightButton) {
            // m_lastNodeTransform.position = initialPositions.last();
            for (int i = 0; i < nbVertices; ++i) {
                mesh->updateVertexPosition(m_selectedVertexIndices[i], initialPositions[i]);
            }
            // std::cout << "Translation canceled" << std::endl;
            
            m_activeTransforms--; 
            if (m_activeTransforms == 0) {  // Check if all transformations are done
                m_transformMode = TransformMode::None;
            }

            return;
        }
    }

    if (m_activeTransforms == m_nbTotalActiveTransform) {
        // mesh->updateVertexPosition(m_selectedVertexIndices.last(), m_lastNodeTransform.position);
        // m_initialNodeTransform.position = m_lastNodeTransform.position;
        for (int i = 0; i < nbVertices; ++i) {
            mesh->updateVertexPosition(m_selectedVertexIndices[i], verticesPositions[i]);
        }
    }

    m_activeTransforms--; 
    if (m_activeTransforms == 0) {  // Check if all transformations are done
        m_transformMode = TransformMode::None;
    }

}

void GLWidget::scaleVerticesSelected()
{
    if (m_selectedVertexIndices.size() < 1) return;
    m_activeTransforms++;

    Mesh* mesh = dynamic_cast<Mesh*>(m_currentNode);

    QVector<QVector3D> initialPositions;
    for (int index : m_selectedVertexIndices) initialPositions.append(mesh->getVertices()[index].position);

    QVector3D midlePoint = getMidlePoint(initialPositions);
    
    QVector<QVector3D> verticesPositions = initialPositions;

    QPoint initialMousePos = mapFromGlobal(QCursor::pos());
    float scaleFactor = 1.0f;

    int nbVertices = m_selectedVertexIndices.size();

    while (!(QApplication::mouseButtons() & Qt::LeftButton)) {
        QCoreApplication::processEvents();

        if (m_transformMode != TransformMode::Scale || m_activeTransforms < m_nbTotalActiveTransform) {
            // m_lastNodeTransform.position = initialPositions.last();
            for (int i = 0; i < nbVertices; ++i) {
                mesh->updateVertexPosition(m_selectedVertexIndices[i], initialPositions[i]);
            }
            
            m_activeTransforms--; 
            if (m_activeTransforms == 0) {  // Check if all transformations are done
                m_transformMode = TransformMode::None;
            }

            return;
        }
        
        QPoint currentMousePos = mapFromGlobal(QCursor::pos());
        int deltaMouse = currentMousePos.y() - initialMousePos.y();

        scaleFactor = qMax(0.01f, 1.0f + deltaMouse * 0.001f);

        for (int i = 0; i < nbVertices; ++i) {
            QVector3D delta = verticesPositions[i] - midlePoint;
            verticesPositions[i] = midlePoint + delta * scaleFactor;
            mesh->updateVertexPosition(m_selectedVertexIndices[i], verticesPositions[i]);
        }

        update();
        if (m_activeTransforms == m_nbTotalActiveTransform) m_lastNodeTransform.position = initialPositions.last();

        // Undo translation
        if (QApplication::mouseButtons() & Qt::RightButton) {
            // m_lastNodeTransform.position = initialPositions.last();
            for (int i = 0; i < nbVertices; ++i) {
                mesh->updateVertexPosition(m_selectedVertexIndices[i], initialPositions[i]);
            }
            // std::cout << "Translation canceled" << std::endl;
            
            m_activeTransforms--; 
            if (m_activeTransforms == 0) {  // Check if all transformations are done
                m_transformMode = TransformMode::None;
            }

            return;
        }
    }

    if (m_activeTransforms == m_nbTotalActiveTransform) {
        // mesh->updateVertexPosition(m_selectedVertexIndices.last(), m_lastNodeTransform.position);
        // m_initialNodeTransform.position = m_lastNodeTransform.position;
        for (int i = 0; i < nbVertices; ++i) {
            mesh->updateVertexPosition(m_selectedVertexIndices[i], verticesPositions[i]);
        }
    }

    m_activeTransforms--; 
    if (m_activeTransforms == 0) {  // Check if all transformations are done
        m_transformMode = TransformMode::None;
    }
}

void GLWidget::rotateVerticesSelected()
{
    if (m_selectedVertexIndices.size() < 1) return;
    m_activeTransforms++;

    Mesh* mesh = dynamic_cast<Mesh*>(m_currentNode);

    QVector<QVector3D> initialPositions;
    for (int index : m_selectedVertexIndices) initialPositions.append(mesh->getVertices()[index].position);

    QVector3D pivotPoint = getMidlePoint(initialPositions);

    QVector3D rotationAxis = -m_camera->getFront();

    QPoint initialMousePos = mapFromGlobal(QCursor::pos());
    float totalAngle = 0.0f;

    QVector<QVector3D> rotatedPositions = initialPositions;
    int nbVertices = m_selectedVertexIndices.size();

    while (!(QApplication::mouseButtons() & Qt::LeftButton)) {
        QCoreApplication::processEvents();

        if (m_transformMode != TransformMode::Rotate || m_activeTransforms < m_nbTotalActiveTransform) {
            // m_lastNodeTransform.position = initialPositions.last();
            for (int i = 0; i < nbVertices; ++i) {
                mesh->updateVertexPosition(m_selectedVertexIndices[i], initialPositions[i]);
            }
            
            m_activeTransforms--; 
            if (m_activeTransforms == 0) {  // Check if all transformations are done
                m_transformMode = TransformMode::None;
            }

            return;
        }
        
        QPoint currentMousePos = mapFromGlobal(QCursor::pos());
        int deltaX = currentMousePos.x() - initialMousePos.x();
        int deltaY = currentMousePos.y() - initialMousePos.y();

        rotationAxis = QVector3D(deltaY, -deltaX, 0.0f).normalized();

        float angleIncrement = deltaX * 0.01f;
        totalAngle += angleIncrement;

        QMatrix4x4 rotationMatrix;
        rotationMatrix.setToIdentity();
        rotationMatrix.translate(pivotPoint);
        rotationMatrix.rotate(qRadiansToDegrees(angleIncrement), rotationAxis);
        rotationMatrix.translate(-pivotPoint);

        for (int i = 0; i < nbVertices; ++i) {
            QVector4D rotatedVertex = rotationMatrix * QVector4D(initialPositions[i], 1.0);
            rotatedPositions[i] = rotatedVertex.toVector3D();
            mesh->updateVertexPosition(m_selectedVertexIndices[i], rotatedPositions[i]);
        }

        update();
        if (m_activeTransforms == m_nbTotalActiveTransform) m_lastNodeTransform.position = initialPositions.last();

        // Undo translation
        if (QApplication::mouseButtons() & Qt::RightButton) {
            // m_lastNodeTransform.position = initialPositions.last();
            for (int i = 0; i < nbVertices; ++i) {
                mesh->updateVertexPosition(m_selectedVertexIndices[i], initialPositions[i]);
            }
            // std::cout << "Translation canceled" << std::endl;
            
            m_activeTransforms--; 
            if (m_activeTransforms == 0) {  // Check if all transformations are done
                m_transformMode = TransformMode::None;
            }

            return;
        }
    }

    if (m_activeTransforms == m_nbTotalActiveTransform) {
        // mesh->updateVertexPosition(m_selectedVertexIndices.last(), m_lastNodeTransform.position);
        // m_initialNodeTransform.position = m_lastNodeTransform.position;
        for (int i = 0; i < nbVertices; ++i) {
            mesh->updateVertexPosition(m_selectedVertexIndices[i], rotatedPositions[i]);
        }
    }

    m_activeTransforms--; 
    if (m_activeTransforms == 0) {  // Check if all transformations are done
        m_transformMode = TransformMode::None;
    }
}
