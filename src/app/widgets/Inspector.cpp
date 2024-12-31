#include "Inspector.h"
#include "stb_image.h"

Inspector::Inspector(QWidget *parent) 
    : QWidget(parent),
      toolNameLabel(new QLabel("Tool: None")),
      toolDescriptionLabel(new QLabel("Description: Not available")),
      currentNode(nullptr),
      posXSpinBox(new QDoubleSpinBox), posYSpinBox(new QDoubleSpinBox), posZSpinBox(new QDoubleSpinBox),
      rotXSpinBox(new QDoubleSpinBox), rotYSpinBox(new QDoubleSpinBox), rotZSpinBox(new QDoubleSpinBox),
      scaleXSpinBox(new QDoubleSpinBox), scaleYSpinBox(new QDoubleSpinBox), scaleZSpinBox(new QDoubleSpinBox) {

    
  QVBoxLayout *mainLayout = new QVBoxLayout(this);

  // Container for scrollable content
  QScrollArea *scrollArea = new QScrollArea(this);
  QWidget *scrollWidget = new QWidget(scrollArea);
  QVBoxLayout *scrollLayout = new QVBoxLayout(scrollWidget);

  // Transform section
  initTransformWidget(scrollLayout);

  // Material section
  initMaterialWidget(scrollLayout);

  // Light section
  initLightWidget(scrollLayout);

  // Tool info
  scrollLayout->addWidget(toolNameLabel);
  scrollLayout->addWidget(toolDescriptionLabel);

  // Set the layout of the scrollable widget
  scrollWidget->setLayout(scrollLayout);

  // Set the scroll area properties
  scrollArea->setWidget(scrollWidget);
  scrollArea->setWidgetResizable(true);

  mainLayout->addWidget(scrollArea);

  setLayout(mainLayout);
}

void Inspector::initTransformWidget(QVBoxLayout *layout) {

    transformButton = new QToolButton();
    transformButton->setText("Transform");
    transformButton->setArrowType(Qt::DownArrow);
    transformButton->setCheckable(true);
    transformButton->setChecked(true);
    transformButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    connect(transformButton, &QToolButton::toggled, this, &Inspector::toggleTransform);

    transformLayout = new QVBoxLayout();

    // Position
    positionGroup = new QGroupBox("Position");
    positionGroup->setStyleSheet("QGroupBox { color: grey; }");
    QHBoxLayout *positionLayout = new QHBoxLayout();
    positionLayout->addWidget(posXSpinBox);
    positionLayout->addWidget(posYSpinBox);
    positionLayout->addWidget(posZSpinBox);
    positionGroup->setLayout(positionLayout);
    transformLayout->addWidget(positionGroup);

    // Rotation
    rotationGroup = new QGroupBox("Rotation");
    rotationGroup->setStyleSheet("QGroupBox { color: grey; }");
    QHBoxLayout *rotationLayout = new QHBoxLayout();
    rotationLayout->addWidget(rotXSpinBox);
    rotationLayout->addWidget(rotYSpinBox);
    rotationLayout->addWidget(rotZSpinBox);
    rotationGroup->setLayout(rotationLayout);
    transformLayout->addWidget(rotationGroup);

    // Scale
    scaleGroup = new QGroupBox("Scale");
    scaleGroup->setStyleSheet("QGroupBox { color: grey; }");
    QHBoxLayout *scaleLayout = new QHBoxLayout();
    scaleLayout->addWidget(scaleXSpinBox);
    scaleLayout->addWidget(scaleYSpinBox);
    scaleLayout->addWidget(scaleZSpinBox);
    scaleGroup->setLayout(scaleLayout);
    transformLayout->addWidget(scaleGroup);

    // Set ranges for each spin box 
    posXSpinBox->setRange(-1000.0, 1000.0);
    posYSpinBox->setRange(-1000.0, 1000.0);
    posZSpinBox->setRange(-1000.0, 1000.0);

    rotXSpinBox->setRange(-360.0, 360.0);
    rotYSpinBox->setRange(-360.0, 360.0);
    rotZSpinBox->setRange(-360.0, 360.0);

    scaleXSpinBox->setRange(0.0, 100.0);
    scaleYSpinBox->setRange(0.0, 100.0);
    scaleZSpinBox->setRange(0.0, 100.0);

    // Set step increments for each spin box
    posXSpinBox->setSingleStep(0.1);
    posYSpinBox->setSingleStep(0.1);
    posZSpinBox->setSingleStep(0.1);

    rotXSpinBox->setSingleStep(0.1);
    rotYSpinBox->setSingleStep(0.1);
    rotZSpinBox->setSingleStep(0.1);

    scaleXSpinBox->setSingleStep(0.1);
    scaleYSpinBox->setSingleStep(0.1);
    scaleZSpinBox->setSingleStep(0.1);

    // Connect signals to corresponding slots
    connect(posXSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &Inspector::onPositionChanged);
    connect(posYSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &Inspector::onPositionChanged);
    connect(posZSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &Inspector::onPositionChanged);

    connect(rotXSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &Inspector::onRotationChanged);
    connect(rotYSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &Inspector::onRotationChanged);
    connect(rotZSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &Inspector::onRotationChanged);

    connect(scaleXSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &Inspector::onScaleChanged);
    connect(scaleYSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &Inspector::onScaleChanged);
    connect(scaleZSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &Inspector::onScaleChanged);

    // Add the group to the layout
    layout->addWidget(transformButton);
    layout->addLayout(transformLayout);
}


void Inspector::toggleTransform(bool checked) {
    positionGroup->setVisible(checked);
    rotationGroup->setVisible(checked);
    scaleGroup->setVisible(checked);
}


void Inspector::initMaterialWidget(QVBoxLayout *layout) {

    materialButton = new QToolButton();
    materialButton->setText("Material");
    materialButton->setArrowType(Qt::DownArrow);
    materialButton->setCheckable(true);
    materialButton->setChecked(true);
    materialButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    connect(materialButton, &QToolButton::toggled, this, &Inspector::toggleMaterial);

    materialLayout = new QVBoxLayout();

    // Albedo color
    albedoColorButton = new QPushButton("Set Albedo");
    connect(albedoColorButton, &QPushButton::clicked, this, &Inspector::onAlbedoColorButtonClicked);
    materialLayout->addWidget(albedoColorButton);

    // Specular color
    specularColorButton = new QPushButton("Set Specular");
    connect(specularColorButton, &QPushButton::clicked, this, &Inspector::onSpecularColorButtonClicked);
    materialLayout->addWidget(specularColorButton);

    // Emissive color
    emissiveColorButton = new QPushButton("Set Emissive");
    connect(emissiveColorButton, &QPushButton::clicked, this, &Inspector::onEmissiveColorButtonClicked);
    materialLayout->addWidget(emissiveColorButton);

    // Shininess
    shininessSpinBox = new QDoubleSpinBox();
    shininessSpinBox->setRange(0.0, 128.0);
    shininessSpinBox->setSingleStep(0.1);
    shininessSpinBox->setPrefix("Shininess: ");
    connect(shininessSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &Inspector::onShininessChanged);
    materialLayout->addWidget(shininessSpinBox);

    // Metalness
    metalnessSpinBox = new QDoubleSpinBox();
    metalnessSpinBox->setRange(0.0, 1.0);
    metalnessSpinBox->setSingleStep(0.01);
    metalnessSpinBox->setPrefix("Metalness: ");
    connect(metalnessSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &Inspector::onMetalnessChanged);
    materialLayout->addWidget(metalnessSpinBox);

    // Roughness
    roughnessSpinBox = new QDoubleSpinBox();
    roughnessSpinBox->setRange(0.0, 1.0);
    roughnessSpinBox->setSingleStep(0.01);
    roughnessSpinBox->setPrefix("Roughness: ");
    connect(roughnessSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &Inspector::onRoughnessChanged);
    materialLayout->addWidget(roughnessSpinBox);

    // Textures
    texturesListWidget = new QListWidget();
    texturesLabel = new QLabel("Textures:");
    materialLayout->addWidget(texturesLabel);
    materialLayout->addWidget(texturesListWidget);

    addTextureButton = new QPushButton("Add Texture");
    connect(addTextureButton, &QPushButton::clicked, this, &Inspector::onAddTexture);
    materialLayout->addWidget(addTextureButton);

    removeTextureButton = new QPushButton("Remove Selected Texture");
    connect(removeTextureButton, &QPushButton::clicked, this, &Inspector::onRemoveTexture);
    materialLayout->addWidget(removeTextureButton);

    layout->addWidget(materialButton);
    layout->addLayout(materialLayout);

}

void Inspector::toggleMaterial(bool checked) {
    albedoColorButton->setVisible(checked);
    specularColorButton->setVisible(checked);
    emissiveColorButton->setVisible(checked);
    shininessSpinBox->setVisible(checked);
    metalnessSpinBox->setVisible(checked);
    roughnessSpinBox->setVisible(checked);
    texturesLabel->setVisible(checked);
    texturesListWidget->setVisible(checked);
    addTextureButton->setVisible(checked);
    removeTextureButton->setVisible(checked);
}

void Inspector::initLightWidget(QVBoxLayout *layout) {
    //button
    lightButton = new QToolButton();
    lightButton->setText("Light");
    lightButton->setArrowType(Qt::DownArrow);
    lightButton->setCheckable(true);
    lightButton->setChecked(true);
    lightButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
   
    // Light properties
    lightLayout = new QVBoxLayout();

    // Light type
    lightTypeComboBox = new QComboBox();
    lightTypeComboBox->addItem("Directional");
    lightTypeComboBox->addItem("Point");
    lightTypeComboBox->addItem("Spot");
    lightLayout->addWidget(lightTypeComboBox);

    // Light color
    lightColorButton = new QPushButton("Set Light Color");
    connect(lightColorButton, &QPushButton::clicked, this, &Inspector::onLightColorButtonClicked);
    lightLayout->addWidget(lightColorButton);

    // Light intensity
    lightIntensitySpinBox = new QDoubleSpinBox();
    lightIntensitySpinBox->setRange(0.0, 100.0);
    lightIntensitySpinBox->setSingleStep(0.1);
    lightIntensitySpinBox->setPrefix("Intensity: ");
    connect(lightIntensitySpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &Inspector::onLightIntensityChanged);
    lightLayout->addWidget(lightIntensitySpinBox);

    layout->addWidget(lightButton);
    layout->addLayout(lightLayout);
}

void Inspector::toggleLight(bool checked) {
    lightTypeComboBox->setVisible(checked);
    lightColorButton->setVisible(checked);
    lightIntensitySpinBox->setVisible(checked);
}

void Inspector::updateNode(Node *node) {
  // check wich class the node is
  if (convertToMesh(node)) {
    toggleTransform(true);
    updateTransform(node);

    toggleMaterial(true);
    updateMaterial(node);
    
    toggleLight(false);
    lightButton->setEnabled(false);
  }
  else if (convertToModel(node)) {
    toggleTransform(true);
    updateTransform(node);

    toggleMaterial(false);
    materialButton->setEnabled(false);
    
    toggleLight(false);
    lightButton->setEnabled(false);
  }
  else if (convertToLight(node)) {
    toggleTransform(true);
    updateTransform(node);

    toggleMaterial(false);
    materialButton->setEnabled(false);
    
    toggleLight(true);
    updateLight(node);
  }
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

void Inspector::updateMaterial(Node *node) {
  Mesh* mesh = dynamic_cast<Mesh*>(node);
  if (!mesh) return;

  // block signals to prevent wrong values
  QSignalBlocker blockerShininess(shininessSpinBox);
  QSignalBlocker blockerMetalness(metalnessSpinBox);
  QSignalBlocker blockerRoughness(roughnessSpinBox);

  shininessSpinBox->setValue(mesh->getMaterial().shininess);
  metalnessSpinBox->setValue(mesh->getMaterial().metalness);
  roughnessSpinBox->setValue(mesh->getMaterial().roughness);

  // Textures
  texturesListWidget->clear();
  for (const Texture& texture : mesh->getMaterial().textures) {
    texturesListWidget->addItem(texture.path);
  }
}

void Inspector::updateLight(Node *node) {
  Light* light = dynamic_cast<Light*>(node);
  if (!light) return;

  // block signals to prevent wrong values
  QSignalBlocker blockerIntensity(lightIntensitySpinBox);

  lightIntensitySpinBox->setValue(light->getIntensity());
  lightTypeComboBox->setCurrentIndex(static_cast<int>(light->getType()));
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

void Inspector::onAlbedoColorButtonClicked() {
    Mesh* mesh = dynamic_cast<Mesh*>(currentNode);
    if (!mesh) return;
    QColor color = QColorDialog::getColor(Qt::white, this, "Choose Albedo Color");
    if (color.isValid()) {
        mesh->getMaterial().setAlbedo(QVector3D(color.redF(), color.greenF(), color.blueF()));
        emit currentNode->nodeChanged();
    }
}

void Inspector::onSpecularColorButtonClicked() {
    Mesh* mesh = dynamic_cast<Mesh*>(currentNode);
    if (!mesh) return;
    QColor color = QColorDialog::getColor(Qt::white, this, "Choose Specular Color");
    if (color.isValid()) {
      mesh->getMaterial().setSpecular(QVector3D(color.redF(), color.greenF(), color.blueF()));
      emit currentNode->nodeChanged();
    }
}

void Inspector::onEmissiveColorButtonClicked() {
    Mesh* mesh = dynamic_cast<Mesh*>(currentNode);
    if (!mesh) return;
    QColor color = QColorDialog::getColor(Qt::white, this, "Choose Emissive Color");

    if (color.isValid()) {
        mesh->getMaterial().setEmissive(QVector3D(color.redF(), color.greenF(), color.blueF()));
        emit currentNode->nodeChanged();
    }
}

void Inspector::onAddTexture() {
    QString fileName = QFileDialog::getOpenFileName(this, "Add Texture", "", "Image Files (*.png *.jpg *.bmp)");
    Mesh* mesh = dynamic_cast<Mesh*>(currentNode);
    if (!fileName.isEmpty() && mesh) {

        // Add to material
        Texture newTexture;
        newTexture.path = fileName;
        newTexture.type = "texture_diffuse";
        newTexture.id = mesh->textureFromFile(fileName, QFileInfo(fileName).absolutePath());
        mesh->getMaterial().textures.push_back(newTexture);

        // Update UI
        texturesListWidget->addItem(fileName);
        emit currentNode->nodeChanged();
    }
}

void Inspector::onRemoveTexture() {
    QListWidgetItem *selectedItem = texturesListWidget->currentItem();
    Mesh* mesh = dynamic_cast<Mesh*>(currentNode);
    if (!selectedItem || !mesh) return;

    // Remove from material
    int index = texturesListWidget->row(selectedItem);
    mesh->getMaterial().textures.remove(index);

    // Update UI
    delete selectedItem;
    emit currentNode->nodeChanged();
}

void Inspector::onRoughnessChanged(double value) {
    Mesh* mesh = dynamic_cast<Mesh*>(currentNode);
    if (!currentNode || !mesh) return;

    mesh->getMaterial().roughness = static_cast<float>(value);
    emit currentNode->nodeChanged();
}

void Inspector::onMetalnessChanged(double value) {
    Mesh* mesh = dynamic_cast<Mesh*>(currentNode);
    if (!currentNode || !mesh) return;

    mesh->getMaterial().metalness = static_cast<float>(value);
    emit currentNode->nodeChanged();
}

void Inspector::onShininessChanged(double value) {
    Mesh* mesh = dynamic_cast<Mesh*>(currentNode);
    if (!currentNode || !mesh) return;

    mesh->getMaterial().shininess = static_cast<float>(value);
    emit currentNode->nodeChanged();
}

void Inspector::onLightColorButtonClicked() {
    Light* light = dynamic_cast<Light*>(currentNode);
    if (!light) return;
    QColor color = QColorDialog::getColor(Qt::white, this, "Choose Light Color");
    if (color.isValid()) {
        light->setColor(QVector3D(color.redF(), color.greenF(), color.blueF()));
        emit currentNode->nodeChanged();
    }
}

void Inspector::onLightIntensityChanged(double value) {
    Light* light = dynamic_cast<Light*>(currentNode);
    if (!light) return;
    light->setIntensity(static_cast<float>(value));
    emit currentNode->nodeChanged();
}

void Inspector::setToolInfo(Tool *tool) {
  toolNameLabel->setText("Tool: " + tool->getName());
  toolDescriptionLabel->setText("Description: " + tool->getToolDescription());
}

Node* Inspector::convertToMesh(Node* node) {
    Mesh* mesh = dynamic_cast<Mesh*>(node);
    if (mesh) return mesh;
    else return nullptr;
}

Node* Inspector::convertToModel(Node* node) {
    Model* model = dynamic_cast<Model*>(node);
    if (model) return model;
    else return nullptr;
}

Node* Inspector::convertToLight(Node* node) {
    Light* light = dynamic_cast<Light*>(node);
    if (light) return light;
    else return nullptr;
}

void Inspector::keyPressEvent(QKeyEvent *event) {
  emit keyPressed(event);
}