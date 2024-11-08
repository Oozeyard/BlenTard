#include "GLWidget.h"

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);  // Permit to receive key events
}

GLWidget::~GLWidget()
{
    if (m_program == nullptr)
        return;
    makeCurrent();
    delete m_program;
    delete m_mesh;
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

void GLWidget::drawGrid(float gridSize, float gridSpacing)
{
    glBegin(GL_LINES);
    glColor3f(0.8f, 0.8f, 0.8f);  // Light gray color for grid lines

    // Draw lines parallel to the X-axis
    for (float i = -gridSize; i <= gridSize; i += gridSpacing) {
        glVertex3f(i, 0, -gridSize);
        glVertex3f(i, 0, gridSize);
    }

    // Draw lines parallel to the Z-axis
    for (float j = -gridSize; j <= gridSize; j += gridSpacing) {
        glVertex3f(-gridSize, 0, j);
        glVertex3f(gridSize, 0, j);
    }
    glEnd();
}


void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Fond noir

    
    // Create a new shader program
    m_program = new QOpenGLShaderProgram;

    

    initShaders(m_program, "./src/shaders/vertex_shader.glsl", "./src/shaders/fragment_shader.glsl");

    m_mesh = new Mesh();
    m_camera = new Camera();

    m_model.setToIdentity();
    // m_model.rotate(65.0f, 54.0f, 84.0f, 1.0f);
    m_view.setToIdentity();
    // m_view.translate(0.0f, 0.0f, -5.0f);


    // m_program->release();

}


void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);

    /* // Couleur du triangle
    glColor3f(1.0f, 0.0f, 0.0f);  // Rouge

    // Dessiner un triangle
    glBegin(GL_TRIANGLES);
        glVertex3f(-0.5f, -0.5f, -1.0f);  // Sommet 1
        glVertex3f( 0.5f, -0.5f, -1.0f);  // Sommet 2
        glVertex3f( 0.0f,  0.5f, -1.0f);  // Sommet 3
    glEnd();

    glFlush(); */

    m_program->bind();

    drawGrid();
    m_camera->computeView(m_view, m_projection);

    m_program->setUniformValue("projection", m_projection);
    m_program->setUniformValue("view", m_view);
    m_program->setUniformValue("model", m_model);

    m_mesh->draw(m_program);
    update();

    m_program->release();

}


void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);  // Viewport

    m_projection = m_camera->getProjectionMatrix();
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    std::cout << "Key pressed: " << event->key() << std::endl;

    if (event->key() == Qt::Key_Escape) {
        std::cout << "Escape key pressed" << std::endl;
        close();  // Fermer la fenêtre si la touche Échap est pressée
    }
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    // std::cout << "Mouse pressed: " << event->button() << std::endl;
    m_camera->mousePressEvent(event);
    // update();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    // std::cout << "Mouse moved: " << event->pos().x() << ", " << event->pos().y() << std::endl;
    m_camera->mouseMoveEvent(event);
    // update();
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    // std::cout << "Wheel moved: " << event->angleDelta().y() << std::endl;
    m_camera->wheelEvent(event);
    // update();

}