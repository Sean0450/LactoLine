#pragma once

#include <QMainWindow>
QT_FORWARD_DECLARE_CLASS(QStackedWidget)

class MainMenu;

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
Ui::MainWindow *ui {nullptr};
QStackedWidget* m_stack {nullptr};
MainMenu* m_pagesList {nullptr};

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
