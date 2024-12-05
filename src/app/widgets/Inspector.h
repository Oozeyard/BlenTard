#pragma once

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QString>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QToolButton>
#include <QPushButton>
#include <QListWidget>
#include <QColorDialog>
#include <QFileDialog>
#include <QScrollArea>

#include "Tools.h"
#include "Node.h"
#include "Hierarchy.h"
#include "Utils.h"
#include "Mesh.h"

class Inspector : public QWidget {
    Q_OBJECT

public:
    explicit Inspector(QWidget *parent = nullptr);

signals:
    void keyPressed(QKeyEvent *event);

public slots:
    void setToolInfo(Tool *tool);

    void updateTransform(Node *node);
    void onPositionChanged();
    void onRotationChanged();
    void onScaleChanged();

private:
    void keyPressEvent(QKeyEvent *event) override;

    Node *convertToMesh(Node* node);

    QLabel *toolNameLabel;
    QLabel *toolDescriptionLabel;
    
    Node *currentNode;
    Hierarchy *hierarchy;

    QVBoxLayout *layout;

    // Transform widgets
    void initTransformWidget(QVBoxLayout *layout);

    QVBoxLayout *transformLayout;

    QGroupBox *positionGroup, *rotationGroup, *scaleGroup;

    QDoubleSpinBox *posXSpinBox, *posYSpinBox, *posZSpinBox;
    QDoubleSpinBox *rotXSpinBox, *rotYSpinBox, *rotZSpinBox;
    QDoubleSpinBox *scaleXSpinBox, *scaleYSpinBox, *scaleZSpinBox;

    void toggleTransform(bool checked);

    // Material widgets
    void initMaterialWidget(QVBoxLayout *layout);
    QGroupBox *materialGroup;
    QDoubleSpinBox *shininessSpinBox, *metalnessSpinBox, *roughnessSpinBox;
    QPushButton *albedoColorButton, *specularColorButton, *emissiveColorButton;

    QListWidget *texturesListWidget;
    QLabel *texturesLabel;
    QPushButton *addTextureButton, *removeTextureButton;
    
    void onAlbedoColorButtonClicked();
    void onSpecularColorButtonClicked();
    void onEmissiveColorButtonClicked();
    void onShininessChanged(double value);
    void onMetalnessChanged(double value);
    void onRoughnessChanged(double value);
    void onAddTexture();
    void onRemoveTexture();

    void toggleMaterial(bool checked);
};