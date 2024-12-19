#pragma once

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QRect>
#include <QPoint>
#include <QDebug>
#include <QCoreApplication>

#include <iostream>

class GLPainter : public QWidget {
    Q_OBJECT
public:
    explicit GLPainter(QWidget *parent = nullptr);

    QRect getSelectionRect() {return QRect(m_selectionStart, m_selectionEnd).normalized();}

    void resizePainter(int width, int height);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    bool m_isSelectionRect { false };
    QPoint m_selectionStart;
    QPoint m_selectionEnd;
};