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

void GLWidget::drawGrid(float gridSize, int divisions)
{
    // Activer les fonctionnalités OpenGL nécessaires
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Couleur et largeur des lignes
    QColor mainColor(255, 255, 255, 128);   // Lignes principales
    QColor subColor(255, 255, 255, 64);     // Lignes secondaires

    float step = gridSize / divisions;

    glBegin(GL_LINES);

    // Lignes de la grille principale
    for (int i = -divisions; i <= divisions; ++i) {
        if (i % 10 == 0) {
            glColor4f(mainColor.redF(), mainColor.greenF(), mainColor.blueF(), mainColor.alphaF());
        } else {
            glColor4f(subColor.redF(), subColor.greenF(), subColor.blueF(), subColor.alphaF());
        }

        // Lignes verticales
        glVertex3f(i * step, 0, -gridSize);
        glVertex3f(i * step, 0, gridSize);

        // Lignes horizontales
        glVertex3f(-gridSize, 0, i * step);
        glVertex3f(gridSize, 0, i * step);
    }

    glEnd();

    glDisable(GL_BLEND);
}


void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);  // Fond noir

    
    // Create a new shader program
    m_program = new QOpenGLShaderProgram;

    initShaders(m_program, "./src/shaders/vertex_shader.glsl", "./src/shaders/fragment_shader.glsl");
    
    //m_rootNode = new Node("RootNode");
    m_mesh = new Model("models/swamp-location/source/map_1.obj");
    //m_mesh = new Model("models/indoor plant_02_+2.fbx");
    //m_mesh = new Model("models/knightsword/source/KnightSword.fbx");
    //m_rootNode->setModel(m_mesh);
    m_camera = new Camera("MainCamera");

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
    glEnable(GL_CULL_FACE);


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

    // light
    m_program->setUniformValue("ligth_position", m_camera->transform.position);
    m_program->setUniformValue("ligth_direction", m_camera->getFront());

    //m_rootNode->getModel()->Draw(m_program->programId());
    m_mesh->Draw(m_program->programId());
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