#include "mainwindow.hpp"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    auto taskManager = std::make_shared<Tasks::TaskManager>();
    MainWindow mainWindow(taskManager);
    mainWindow.setStyleSheet("background-color: #F0F8FF;");
    mainWindow.show();
    return a.exec();
}
