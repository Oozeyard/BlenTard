#pragma once

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QString>

class Inspector : public QWidget {
    Q_OBJECT

public:
    explicit Inspector(QWidget *parent = nullptr);

    void setToolInfo(const QString &toolName, const QString &toolDescription);

private:
    QLabel *toolNameLabel;
    QLabel *toolDescriptionLabel;
};