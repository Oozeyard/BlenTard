#pragma once
#include <QMainWindow>
#include <QSplitter>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QMenuBar>
#include "app/widgets/GLWidget.h"
#include "app/widgets/Inspector.h"
#include "app/widgets/Hierarchy.h"
#include "source/headers/Mesh.h"

class MainWindow : public QMainWindow {
 Q_OBJECT

public:
   MainWindow(QWidget *parent = nullptr);
   ~MainWindow();

private:
   void InitMenuBar();
   GLWidget *glWidget;
};