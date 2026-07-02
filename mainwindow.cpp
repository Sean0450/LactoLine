#include <QStackedWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QListWidget>

#include "mainwindow.hpp"
#include "MainMenu.hpp"
#include "Footer.hpp"
#include "Tasks/TaskListWidget.hpp"
#include "Product/ProductTableWidget.hpp"
#include "Shifts/ShiftTableView.hpp"
#include "PackingList/PackingListWidget.hpp"

MainWindow::MainWindow(std::weak_ptr<Tasks::TaskManager> taskManager, QWidget *parent)
    : QMainWindow(parent)
    , m_taskManager(std::move(taskManager))
{
    auto* centralWidget = new QWidget(this);
    auto* mainLayout = new QHBoxLayout(centralWidget);
    auto* verticalLayout = new QVBoxLayout();

    m_pagesList = new MainMenu(centralWidget);
    m_pagesList->setMinSizes(this->height());

    m_stack = new QStackedWidget(centralWidget);
    connect(m_pagesList->getListObject(), &QListWidget::currentRowChanged, this, [&](int rowIndex){m_stack->setCurrentIndex(rowIndex);});

    auto* footer = new Footer(centralWidget);
    verticalLayout->addWidget(m_stack);
    verticalLayout->addStretch(1);
    verticalLayout->addWidget(footer);

    mainLayout->addWidget(m_pagesList->getListObject());
    mainLayout->setStretchFactor(m_pagesList->getListObject(), 2);
    mainLayout->addLayout(verticalLayout);
    mainLayout->setStretchFactor(verticalLayout, 4);
    setCentralWidget(centralWidget);
    registerMainWidgets();
}

void MainWindow::newTaskCreated(const Tasks::TaskData& data)
{
    if (auto manager = m_taskManager.lock())
    {
        manager->addTask(data);
    }
}

void MainWindow::registerMainWidgets()
{
    if (auto manager = m_taskManager.lock())
    {
        auto* lst = new TaskListWidget(manager->getCurrentShiftTaskData(), manager.get(), this);
        connect(lst, &TaskListWidget::taskAdded, this, [&](const auto& taskData){newTaskCreated(taskData);});
        m_stack->addWidget(lst);
        m_stack->addWidget(new ProductTableWidget(manager.get(), this));
        m_stack->addWidget(new ShiftTableView(manager.get(), this));
        m_stack->addWidget(new PackingList(manager.get(), this));
    }
}
