#include "MainMenu.hpp"

#include <QListWidget>

MainMenu::MainMenu(QWidget *parent)
{
    m_menu = new QListWidget(parent);
    createItem(s_taskList);
    createItem(s_productList);
    createItem(s_shiftInformation);
    createItem(s_invoiceCreation);
    setVisibleSettings();
}

void MainMenu::setVisibleSettings()
{
    m_menu->setMinimumWidth(s_minWidth);
    m_menu->setSpacing(15);
    m_menu->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_menu->setStyleSheet("border: 1px solid black;");
}

void MainMenu::createItem(const char* text) const
{
    if (m_menu)
    {
        auto* item = new QListWidgetItem(m_menu);
        item->setText(text);
        item->setFont(m_baseFont);
        m_menu->addItem(item);
    }
}

void MainMenu::setMinSizes(int height, int width)
{
    m_menu->setMinimumHeight(height);
    m_menu->setMinimumWidth(width);
}

QListWidget* MainMenu::getListObject()
{
    return m_menu;
}
