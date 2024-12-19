#include "GLPainter.h"

GLPainter::GLPainter(QWidget *parent) : QWidget(parent) 
{
    setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    if (parent) {
        setGeometry(parent->geometry());
    }
}

void GLPainter::resizePainter(int width, int height)
{
    setGeometry(QRect(0, 0, width, height));
}

void GLPainter::paintEvent(QPaintEvent *event) 
{
    Q_UNUSED(event);
    
    if (m_isSelectionRect) {
        QPainter painter(this);

        painter.setRenderHint(QPainter::Antialiasing);

        // Style du rectangle
        painter.setPen(QPen(Qt::blue, 2, Qt::DashLine));
        painter.setBrush(QColor(0, 0, 255, 50));  // Remplissage semi-transparent

        QRect selectionRect = QRect(m_selectionStart, m_selectionEnd).normalized();
        // qDebug() << selectionRect;
        painter.drawRect(selectionRect);
        painter.end();
    }

    update();
}

void GLPainter::mousePressEvent(QMouseEvent *event) 
{
    if (event->buttons() & Qt::LeftButton) {
        m_isSelectionRect = true;
        m_selectionStart = event->pos();  
        m_selectionEnd = m_selectionStart;
    }

    if (parentWidget()) {
        QCoreApplication::sendEvent(parentWidget(), event);
    }
}

void GLPainter::mouseMoveEvent(QMouseEvent *event) 
{
    if (m_isSelectionRect) {
        m_selectionEnd = event->pos();
        update();
    }

    if (parentWidget()) {
        QCoreApplication::sendEvent(parentWidget(), event);
    }
}

void GLPainter::mouseReleaseEvent(QMouseEvent *event) 
{
    if (event->button() == Qt::LeftButton && m_isSelectionRect) {
        m_isSelectionRect = false;
    }

    if (parentWidget()) {
        QCoreApplication::sendEvent(parentWidget(), event);
    }
}
    