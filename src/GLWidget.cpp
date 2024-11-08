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

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Fond noir

    
    // Create a new shader program
    m_program = new QOpenGLShaderProgram;

    initShaders(m_program, "./shaders/vertex_shader.glsl", "./shaders/fragment_shader.glsl");

    m_mesh = new Mesh();

    m_model.setToIdentity();
    m_model.rotate(65.0f, 54.0f, 84.0f, 1.0f);
    m_view.setToIdentity();
    m_view.translate(0.0f, 0.0f, -5.0f);


    
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

    m_program->setUniformValue("projection", m_projection);
    m_program->setUniformValue("view", m_view);
    m_program->setUniformValue("model", m_model);

    m_mesh->draw(m_program);

    m_program->release();

}


void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);  // Viewport

    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane, far plane, field of view
    const qreal zNear = 0.01, zFar = 100.0, fov = 45.0;

    m_projection.setToIdentity();
    m_projection.perspective(fov, aspect, zNear, zFar);
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
    std::cout << "Mouse pressed: " << event->button() << std::endl;
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    std::cout << "Mouse moved: " << event->pos().x() << ", " << event->pos().y() << std::endl;
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    std::cout << "Wheel moved: " << event->angleDelta().y() << std::endl;
    if (event->angleDelta().y() > 0) {
        // Zoom avant
    } else {
        // Zoom arrière
    }
}