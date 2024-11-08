#pragma once
#include <QMainWindow>
#include <QSplitter>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QMenuBar>
#include <QStackedLayout>
#include "GLWidget.h"
#include "Inspector.h"
#include "Hierarchy.h"
#include "Tools.h"
#include "Mesh.h"

class MainWindow : public QMainWindow {
 Q_OBJECT

public:
   MainWindow(QWidget *parent = nullptr);
   ~MainWindow();

private:
   void InitMenuBar();
   GLWidget *glWidget;
};