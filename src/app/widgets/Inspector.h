#pragma once

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QString>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include "Tools.h"
#include "Node.h"
#include "Hierarchy.h"

class Inspector : public QWidget {
    Q_OBJECT

public:
    explicit Inspector(QWidget *parent = nullptr);

public slots:
    void setToolInfo(Tool *tool);

    void updateTransform(Node *node);
    void onPositionChanged();
    void onRotationChanged();
    void onScaleChanged();

private:
    QLabel *toolNameLabel;
    QLabel *toolDescriptionLabel;

    QDoubleSpinBox *posXSpinBox;
    QDoubleSpinBox *posYSpinBox;
    QDoubleSpinBox *posZSpinBox;

    QDoubleSpinBox *rotXSpinBox;
    QDoubleSpinBox *rotYSpinBox;
    QDoubleSpinBox *rotZSpinBox;

    QDoubleSpinBox *scaleXSpinBox;
    QDoubleSpinBox *scaleYSpinBox;
    QDoubleSpinBox *scaleZSpinBox;

    Node *currentNode;
    Hierarchy *hierarchy;
};