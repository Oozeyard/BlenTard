#include "app/MainWindow.h"
#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // Load stylesheet
    QFile file("MaterialDark.qss");
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(file.readAll());
        app.setStyleSheet(styleSheet);
        file.close();
    } else {
        qWarning() << "Failed to load stylesheet";
    }

    // Create main window
    MainWindow window;
    window.show();
    return app.exec();
}