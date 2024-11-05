#pragma once
#include <QMainWindow>
#include "app/widgets/GLWidget.h"
#include "source/headers/Mesh.h"

class MainWindow : public QMainWindow {
 Q_OBJECT

 public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

 private:
    GLWidget *glWidget;
};