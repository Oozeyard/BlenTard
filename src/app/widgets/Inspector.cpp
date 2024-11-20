#include "Inspector.h"

Inspector::Inspector(QWidget *parent, ToolBar *toolBar) 
    : QWidget(parent),
      toolNameLabel(new QLabel("Tool: None")),
      toolDescriptionLabel(new QLabel("Description: Not available")) {
    

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(toolNameLabel);
    layout->addWidget(toolDescriptionLabel);

    setLayout(layout);

    connect(toolBar, &ToolBar::toolSelected, this, &Inspector::setToolInfo);
}

void Inspector::setToolInfo(Tool *tool) {
  std::cout << "Setting tool info" << std::endl;
  toolNameLabel->setText("Tool: " + tool->getName());
  toolDescriptionLabel->setText("Description: " + tool->getToolDescription());
}
