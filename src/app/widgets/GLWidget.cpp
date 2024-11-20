#include "GLWidget.h"

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);  // Permit to receive key events
    
    // Scene Hierachy
    m_rootNode = new Node("Root");

}

GLWidget::~GLWidget()
{
    if (m_program == nullptr)
        return;
    makeCurrent();
    delete m_program;
    delete m_programGridOverlay;
    delete m_rootNode;
    delete m_gridOverlay;
    delete m_camera;
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

void GLWidget::initShaders(QOpenGLShaderProgram *program, std::string vertex_shader, std::string fragment_shader)
{
    if (!program->addShaderFromSourceFile(QOpenGLShader::Vertex, vertex_shader.c_str())) {
        qWarning() << "Error when compiling the vertex shader:" << program->log();
    }

    if (!program->addShaderFromSourceFile(QOpenGLShader::Fragment, fragment_shader.c_str())) {
        qWarning() << "Error when compiling the fragment shader:" << program->log();
    }

    if (!program->link()) {
        qWarning() << "Error when linking the shader program:" << program->log();
    }
}


void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);  // Fond noir

    // Create the principal shader program
    m_program = new QOpenGLShaderProgram;
    initShaders(m_program, "./src/shaders/vertex_shader.glsl", "./src/shaders/fragment_shader.glsl");

    // Create the grid overlay shader program
    m_programGridOverlay = new QOpenGLShaderProgram;
    initShaders(m_programGridOverlay, "./src/shaders/vertex_shader_grid_overlay.glsl", "./src/shaders/fragment_shader_grid_overlay.glsl");
    m_gridOverlay = new GridOverlay(m_programGridOverlay);

    m_camera = new Camera("MainCamera");
    m_camera->setAspect(static_cast<float>(width()) / static_cast<float>(height()));

    // Model* model = new Model("Map", "models/swamp-location/source/map_1.obj"); model->transform.rotate(QVector3D(90, 0, 0));
    // Model* model = new Model("Cube", "models/cube.obj");
    Model* model = new Model("Dragon", "models/fbx/Dragon 2.5_fbx.fbx");
    m_rootNode->addChild(model);

    emit rootNodeCreated(m_rootNode);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    
    // light
    m_program->bind();
    m_program->setUniformValue("ligth_position", m_camera->transform.position);
    m_program->setUniformValue("ligth_direction", m_camera->getFront());
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
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    std::cout << "Key pressed: " << event->key() << std::endl;
    m_camera->keyPressEvent(event);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    std::cout << "Mouse pressed: " << event->button() << std::endl;
    m_camera->mousePressEvent(event);

}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    std::cout << "Mouse moved: " << event->pos().x() << ", " << event->pos().y() << std::endl;
    m_camera->mouseMoveEvent(event);

}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    std::cout << "Wheel moved: " << event->angleDelta().y() << std::endl;
    m_camera->wheelEvent(event);

}