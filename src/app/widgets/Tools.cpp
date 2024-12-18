#include "Tools.h"

ToolBar::ToolBar(QWidget *parent) : QWidget(parent) {
    setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);

}

void ToolBar::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPoint topLeft = event->rect().topLeft();
    QColor highlightedBack(86, 128, 194);
    QColor back(49, 49, 49);

    QRect toolBoxHandle = QRect(topLeft, QSize(40, 7));
    painter.fillRect(toolBoxHandle, back);


    for (Tool tool : tools) {
        QRect toolRect = QRect(topLeft, QSize(40, 40));
        QPainterPath path;
        path.addRoundedRect(toolRect, 2, 2);
        painter.fillPath(path, back);

        QRect iconRect = toolRect.adjusted(5, 5, -5, -5);
        tool.getIcon().paint(&painter, iconRect);

        topLeft += QPoint(0, 40);
    }

}

Tool ToolBar::getToolAt(const QPoint &pos) const {
    int index = pos.y() / 40;
    if (index >= 0 && index < tools.size()) {
        return tools[index];
    }
    return Tool(); // Return a default Tool if the index is out of range
}

void ToolBar::mousePressEvent(QMouseEvent *event) {
    std::cout << "Tool pressed: " << getToolAt(event->pos()).getName().toStdString() << std::endl;
    
}
