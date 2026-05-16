#include <QStackedWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QListWidget>

#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "MainMeny.hpp"
#include "Footer.hpp"
#include "Tasks/Tasklist.hpp"
#include "Database/TasksTable.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    auto* centralWidget = new QWidget(this);
    auto* mainLayout = new QHBoxLayout(centralWidget);
    auto* verticalLayout = new QVBoxLayout();

    m_pagesList = new MainMenu(centralWidget);
    m_pagesList->setMinSizes(this->height());

    m_stack = new QStackedWidget(centralWidget);
    m_stack->addWidget(new QLabel("HY!"));

    auto* footer = new Footer(centralWidget);
    verticalLayout->addWidget(m_stack);
    verticalLayout->addStretch(1);
    verticalLayout->addWidget(footer);

    std::vector<Tasks::TaskData> data;
    data.emplace_back("Налить 10 бутылок молока", "Молоко", "20.06.2026", GeneralValues::PriorityStatus::Extra, 10, 0, "");
    data.emplace_back("Налить 50 бутылок йогурта", "Йогурт", "20.06.2026", GeneralValues::PriorityStatus::Medium, 50, 0, "");
    data.emplace_back("Сварить 20 кг сыра", "Сыр", "20.06.2026", GeneralValues::PriorityStatus::Low, 20, 0, "");

    auto* lst = new TaskList(data, this);
    m_stack->addWidget(lst);
    m_stack->setCurrentIndex(1);
    mainLayout->addWidget(m_pagesList->getListObject());
    mainLayout->addLayout(verticalLayout);
    setCentralWidget(centralWidget);
    try{
         TasksTable table("test.db3");
    }catch(std::exception& e)
    {

    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
