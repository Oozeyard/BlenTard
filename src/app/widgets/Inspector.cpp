#include "Inspector.h"

Inspector::Inspector(QWidget *parent) 
    : QWidget(parent),
      toolNameLabel(new QLabel("Tool: None")),
      toolDescriptionLabel(new QLabel("Description: Not available")),
      currentNode(nullptr),
      posXSpinBox(new QDoubleSpinBox), posYSpinBox(new QDoubleSpinBox), posZSpinBox(new QDoubleSpinBox),
      rotXSpinBox(new QDoubleSpinBox), rotYSpinBox(new QDoubleSpinBox), rotZSpinBox(new QDoubleSpinBox),
      scaleXSpinBox(new QDoubleSpinBox), scaleYSpinBox(new QDoubleSpinBox), scaleZSpinBox(new QDoubleSpinBox) {
    
  QVBoxLayout *layout = new QVBoxLayout(this);

  // Transform info
  // Position
  QGroupBox *positionGroup = new QGroupBox("Position");
  QHBoxLayout *positionLayout = new QHBoxLayout;
  positionLayout->addWidget(posXSpinBox);
  positionLayout->addWidget(posYSpinBox);
  positionLayout->addWidget(posZSpinBox);
  positionGroup->setLayout(positionLayout);
  layout->addWidget(positionGroup);

  // Rotation
  QGroupBox *rotationGroup = new QGroupBox("Rotation");
  QHBoxLayout *rotationLayout = new QHBoxLayout;
  rotationLayout->addWidget(rotXSpinBox);
  rotationLayout->addWidget(rotYSpinBox);
  rotationLayout->addWidget(rotZSpinBox);
  rotationGroup->setLayout(rotationLayout);
  layout->addWidget(rotationGroup);

  // Scale
  QGroupBox *scaleGroup = new QGroupBox("Scale");
  QHBoxLayout *scaleLayout = new QHBoxLayout;
  scaleLayout->addWidget(scaleXSpinBox);
  scaleLayout->addWidget(scaleYSpinBox);
  scaleLayout->addWidget(scaleZSpinBox);
  scaleGroup->setLayout(scaleLayout);
  layout->addWidget(scaleGroup);

  // Set ranges
  posXSpinBox->setRange(-1000.0, 1000.0);
  posYSpinBox->setRange(-1000.0, 1000.0);
  posZSpinBox->setRange(-1000.0, 1000.0);

  rotXSpinBox->setRange(-360.0, 360.0);
  rotYSpinBox->setRange(-360.0, 360.0);
  rotZSpinBox->setRange(-360.0, 360.0);

  scaleXSpinBox->setRange(0.0, 100.0);
  scaleYSpinBox->setRange(0.0, 100.0);
  scaleZSpinBox->setRange(0.0, 100.0);

  // Set paddings
  posXSpinBox->setSingleStep(0.1);
  posYSpinBox->setSingleStep(0.1);
  posZSpinBox->setSingleStep(0.1);

  rotXSpinBox->setSingleStep(0.1);
  rotYSpinBox->setSingleStep(0.1);
  rotZSpinBox->setSingleStep(0.1);

  scaleXSpinBox->setSingleStep(0.1);
  scaleYSpinBox->setSingleStep(0.1);
  scaleZSpinBox->setSingleStep(0.1);

  // Connect signals
  connect(posXSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &Inspector::onPositionChanged);
  connect(posYSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &Inspector::onPositionChanged);
  connect(posZSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &Inspector::onPositionChanged);

  connect(rotXSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &Inspector::onRotationChanged);
  connect(rotYSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &Inspector::onRotationChanged);
  connect(rotZSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &Inspector::onRotationChanged);

  connect(scaleXSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &Inspector::onScaleChanged);
  connect(scaleYSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &Inspector::onScaleChanged);
  connect(scaleZSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &Inspector::onScaleChanged);

  // Tool info
  layout->addWidget(toolNameLabel);
  layout->addWidget(toolDescriptionLabel);

  setLayout(layout);
}

void Inspector::updateTransform(Node *node) {

  if (!node) {

    currentNode = nullptr;

    posXSpinBox->setValue(0.0);
    posYSpinBox->setValue(0.0);
    posZSpinBox->setValue(0.0);

    rotXSpinBox->setValue(0.0);
    rotYSpinBox->setValue(0.0);
    rotZSpinBox->setValue(0.0);

    scaleXSpinBox->setValue(1.0);
    scaleYSpinBox->setValue(1.0);
    scaleZSpinBox->setValue(1.0);

    return;
  }

  std::cout << "Node selected : " << node->getName().toStdString() << std::endl;
  currentNode = node;


  const Transform &transform = node->getTransform();

  // block signals to prevent wrong values
  QSignalBlocker blockerX(posXSpinBox);
  QSignalBlocker blockerY(posYSpinBox);
  QSignalBlocker blockerZ(posZSpinBox);

  QSignalBlocker blockerRotX(rotXSpinBox);
  QSignalBlocker blockerRotY(rotYSpinBox);
  QSignalBlocker blockerRotZ(rotZSpinBox);

  QSignalBlocker blockerScaleX(scaleXSpinBox);
  QSignalBlocker blockerScaleY(scaleYSpinBox);
  QSignalBlocker blockerScaleZ(scaleZSpinBox);

  posXSpinBox->setValue(transform.position.x());
  posYSpinBox->setValue(transform.position.y());
  posZSpinBox->setValue(transform.position.z());

  rotXSpinBox->setValue(transform.rotationEuler.x());
  rotYSpinBox->setValue(transform.rotationEuler.y());
  rotZSpinBox->setValue(transform.rotationEuler.z());

  scaleXSpinBox->setValue(transform.scale.x());
  scaleYSpinBox->setValue(transform.scale.y());
  scaleZSpinBox->setValue(transform.scale.z());

}

void Inspector::onPositionChanged() {
  if (!currentNode) return;

  QVector3D position(posXSpinBox->value(), posYSpinBox->value(), posZSpinBox->value());
  currentNode->getTransform().position = position;

  emit currentNode->nodeChanged();
}

void Inspector::onRotationChanged() {
  if (!currentNode) return;

  QVector3D rotation(rotXSpinBox->value(), rotYSpinBox->value(), rotZSpinBox->value());
  currentNode->getTransform().rotationEuler = rotation;
  currentNode->getTransform().rotation = QQuaternion::fromEulerAngles(rotation);
  
  emit currentNode->nodeChanged();
}

void Inspector::onScaleChanged() {
  if (!currentNode) return;

  QVector3D scale(scaleXSpinBox->value(), scaleYSpinBox->value(), scaleZSpinBox->value());
  currentNode->getTransform().scale = scale;

  emit currentNode->nodeChanged();
}

void Inspector::setToolInfo(Tool *tool) {
  toolNameLabel->setText("Tool: " + tool->getName());
  toolDescriptionLabel->setText("Description: " + tool->getToolDescription());
}
