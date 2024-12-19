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

        if (!tool.getSubTools().isEmpty()) {
            QPainterPath p;
            p.moveTo(toolRect.bottomRight());
            p.lineTo(toolRect.bottomRight() + QPoint(0, -7));
            p.lineTo(toolRect.bottomRight() + QPoint(-7, 0));
            painter.fillPath(p, Qt::white);
        }

        if (tool.getSubToolsVisibility()) {
            QPoint subToolTopLeft = toolRect.topRight() + QPoint(10, 0); // Décalage horizontal de 10px
            for (Tool subTool : tool.getSubTools()) {
                QRect subToolRect = QRect(subToolTopLeft, QSize(40, 40));
                QPainterPath subPath;
                subPath.addRoundedRect(subToolRect, 2, 2);
                painter.fillPath(subPath, back);

                QRect subIconRect = subToolRect.adjusted(5, 5, -5, -5);
                subTool.getIcon().paint(&painter, subIconRect);

                subToolTopLeft += QPoint(0, 40); // Décalage vertical pour les sous-outils
            }
        }

       topLeft += QPoint(0, 40);
    }
    painter.end();

}

void ToolBar::mousePressEvent(QMouseEvent *event) {
    QPoint clickPos = event->pos();

    QPoint topLeft(0, 0);
    QSize toolSize(40, 40); // tool size
    QSize subToolSize(40, 40); // subtool size

    for (Tool& tool : tools) {
        QRect toolRect(topLeft, toolSize);

        // check if clicked on tool
        if (toolRect.contains(clickPos)) {
            std::cout << "Clicked on tool: " << tool.getName().toStdString() << std::endl;

            // remove subtools visibility of previously selected tool
            if (selectedTool != nullptr && selectedTool != &tool) {
                selectedTool->setSubToolsVisibility();
            }

            // Check if tool has subtools
            if (!tool.getSubTools().isEmpty()) {
                tool.setSubToolsVisibility();
            }

            setSelectedTool(&tool);

            update();
            return;
        }

        // Show subtools
        if (tool.getSubToolsVisibility()) {
            QPoint subToolTopLeft = toolRect.topRight() + QPoint(10, 0); // 10px horizontal offset
            for (Tool& subTool : tool.getSubTools()) {
                QRect subToolRect(subToolTopLeft, subToolSize);

                if (subToolRect.contains(clickPos)) {
                    std::cout << "Clicked on subtool: " << subTool.getName().toStdString() << std::endl;

                    // remove subtools visibility of previously selected tool
                    if (selectedTool != nullptr && selectedTool != &subTool) {
                        selectedTool->setSubToolsVisibility();
                    }

                    setSelectedTool(&subTool);

                    update();
                    return;
                }

                subToolTopLeft += QPoint(0, 40); // check next subtool
            }
        }


        topLeft += QPoint(0, 40); // check next tool
    }

    QCoreApplication::sendEvent(parent(), event);
}

void ToolBar::setSelectedTool(Tool *tool) {
    selectedTool = tool;
    if (tool != nullptr) emit toolSelected(tool);
}