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

#include <iostream>

class MainWindow : public QMainWindow {
 Q_OBJECT

public:
   MainWindow(QWidget *parent = nullptr);

private:
   void InitMenuBar();
   void InitTools();


   GLWidget *glWidget { nullptr };
   ToolBar *toolBar { nullptr };
   Hierarchy *hierarchy { nullptr };
   Inspector *inspector {nullptr};
};