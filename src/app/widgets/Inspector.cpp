#include "Inspector.h"

Inspector::Inspector(QWidget *parent) 
    : QWidget(parent),
      toolNameLabel(new QLabel("Tool: None")),
      toolDescriptionLabel(new QLabel("Description: Not available")) {
    

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(toolNameLabel);
    layout->addWidget(toolDescriptionLabel);

    setLayout(layout);
}

void Inspector::setToolInfo(const QString &toolName, const QString &toolDescription) {
    toolNameLabel->setText("Tool: " + toolName);
    toolDescriptionLabel->setText("Description: " + toolDescription);
}
