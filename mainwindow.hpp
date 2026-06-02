#pragma once

#include <QMainWindow>

#include "Tasks/TaskManager.hpp"

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
    std::weak_ptr<Tasks::TaskManager> m_taskManager;

private slots:
    void newTaskCreated(const Tasks::TaskData& data);

public:
    MainWindow(std::weak_ptr<Tasks::TaskManager> taskManager, QWidget *parent = nullptr);
    ~MainWindow();
};
