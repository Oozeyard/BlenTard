#pragma once

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QString>
#include "Tools.h"

class Inspector : public QWidget {
    Q_OBJECT

public:
    explicit Inspector(QWidget *parent = nullptr, ToolBar *toolBar = nullptr);

    void setToolInfo(Tool *tool);

private:
    QLabel *toolNameLabel;
    QLabel *toolDescriptionLabel;
};