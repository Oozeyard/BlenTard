#pragma once

#include <QWidget>
#include <QMenu>
#include <QVector>
#include <QString>
#include <QIcon>
#include <QPainter>
#include <QPaintEvent>
#include <QPainterPath>

class Tool {

public:
    Tool(QString name, QIcon icon) : name(name), icon(icon) {}
    Tool() {}

    void addSubTool(Tool tool) {subTools.push_back(tool);}
    
    void setName(QString name) {this->name = name;}
    void setIcon(QIcon icon) {this->icon = icon;}

    QString getName() {return name;}
    QIcon getIcon() {return icon;}

private:
    QString name;
    QIcon icon;
    QVector<Tool> subTools;
};

class ToolBar : public QWidget {
    Q_OBJECT

public:
    explicit ToolBar(QWidget *parent = nullptr);
    void addTool(Tool tool) {tools.push_back(tool);}
    void addSeparator();

    // Events
    //void mousePressEvent(QMouseEvent *event) override;
    //void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override; // paint own widget

private:
    QVector<Tool> tools;
};