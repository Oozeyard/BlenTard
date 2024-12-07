#pragma once

#include <QWidget>
#include <QMenu>
#include <QVector>
#include <QString>
#include <QIcon>
#include <QPainter>
#include <QPaintEvent>
#include <QPainterPath>

#include <iostream>

enum ToolType {
    NONE,
    MOVE,
    SELECT,
    RECTANGLE_SELECT,
    CIRCLE_SELECT,
    VERTEX_SELECT,
    FACE_SELECT,
    ORTHOGRAPHIC
};

class Tool {

public:
    Tool(QString name, QIcon icon, ToolType type) : type(type), name(name), icon(icon) {}
    Tool() {}

    void addSubTool(Tool tool) {subTools.push_back(tool);}
    
    void setName(QString name) {this->name = name;}
    void setIcon(QIcon icon) {this->icon = icon;}

    QString getName() {return name;}
    QIcon getIcon() {return icon;}
    QVector<Tool> getSubTools() {return subTools;}

    void setSubToolsVisibility() {isSubToolsVisible = !isSubToolsVisible;}
    bool getSubToolsVisibility() {return isSubToolsVisible;}

    QString getToolDescription() {return toolDescription;}

    ToolType type;
private:
    QString name;
    QIcon icon { QIcon() };
    QVector<Tool> subTools;
    bool isSubToolsVisible = false;
    QString toolDescription = "Not available";
};

class ToolBar : public QWidget {
    Q_OBJECT

public:
    explicit ToolBar(QWidget *parent = nullptr);
    void addTool(Tool tool) {tools.push_back(tool);}
    int GetToolsSize() {return tools.size();}
    void addSeparator();

    Tool* getSelectedTool() {return selectedTool;}
    void setSelectedTool(Tool *tool);

protected:
    // Events
    void mousePressEvent(QMouseEvent *event) override;
    //void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override; // paint own widget

private:
    QVector<Tool> tools;
    Tool *selectedTool = nullptr;

signals:
    void toolSelected(Tool *tool);
};