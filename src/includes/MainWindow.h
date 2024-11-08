#pragma once

#include <QMainWindow>
#include "GLWidget.h"

class MainWindow : public QMainWindow 
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    GLWidget *glWidget = new GLWidget();
};