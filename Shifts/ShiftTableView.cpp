#include "ShiftTableView.hpp"
#include "ShiftTaskInformationObserver.hpp"
#include "Date/DateTranslator.hpp"
#include "LedIndicator.hpp"

#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QMessageBox>
#include <QDateTime>
#include <QLabel>


ShiftTableView::ShiftTableView(ShiftTaskInformationObserver* observer, QWidget* parent):QWidget{parent},
                                                                                        m_observer(observer)
{
    auto* mainLayout = new QVBoxLayout(this);

    auto* finderLayout = new QHBoxLayout();
    finderLayout->setSpacing(5);

    m_shiftFinder = new QLineEdit(this);
    m_shiftFinder->setStyleSheet(Resources::Styles::baseLineEdit);
    m_shiftFinder->setPlaceholderText(m_reminderText);
    m_shiftFinder->setFont(m_baseFont);
    connect(m_shiftFinder, &QLineEdit::textChanged, this, &ShiftTableView::checkFinderText);
    finderLayout->addWidget(m_shiftFinder);

    m_findShift = new QPushButton(QStringLiteral("Просмотр смены"), this);
    m_findShift->setFont(m_baseFont);
    m_findShift->setStyleSheet(QStringLiteral("QPushButton{ ") + Resources::Styles::baseElementsStyle + QStringLiteral("}") + Resources::Styles::pressedButtonStyle);
    connect(m_findShift, &QPushButton::clicked, this, &ShiftTableView::onFindShiftClicked);
    finderLayout->addWidget(m_findShift);
    mainLayout->addLayout(finderLayout);
    mainLayout->setAlignment(finderLayout, Qt::AlignTop);

    m_taskInfo = new QGroupBox(this);
    m_taskInfo->setFont(m_baseFont);
    m_taskInfo->setVisible(false);

    m_taskInfoLayout = new QVBoxLayout(m_taskInfo);
    m_taskInfoLayout->setSpacing(5);

    mainLayout->addWidget(m_taskInfo);
    mainLayout->addStretch(1);
}

void ShiftTableView::checkFinderText()
{
    const QString currentText = m_shiftFinder->text();
    auto match = s_dateRegular.match(currentText);
    const std::string currentDate = DateTranslator::getCurrentDate();
    if (match.hasMatch() && DateTranslator::isCurrentDateMoreOrEqual(currentDate, currentText.toStdString()))
    {
        m_findShift->setEnabled(true);
        m_shiftFinder->setStyleSheet(Resources::Styles::baseLineEdit);
    }
    else
    {
        m_findShift->setEnabled(false);
        m_shiftFinder->setStyleSheet(Resources::Styles::redBorderLineEdit);
    }
}

void ShiftTableView::onFindShiftClicked()
{
    if (m_shiftFinder->text().isEmpty())
    {
        m_shiftFinder->setStyleSheet(Resources::Styles::redBorderLineEdit);
    }
    else if (!m_shiftFinder->text().isEmpty() && m_observer)
    {
        auto shiftTaskData = m_observer->getTasksData(m_shiftFinder->text().toStdString());
        if (!shiftTaskData.empty())
        {
            m_taskInfo->setTitle(QStringLiteral("Статистика за смену от ") + QString::fromStdString(shiftTaskData.front().date));
            if (m_taskInfo->isVisible())
            {
                for (int i = static_cast<int>(m_taskData.size() - 1); i > -1; --i)
                {
                    m_taskData.at(i)->deleteLater();
                    m_taskData.pop_back();
                }
                for (const auto& data: shiftTaskData)
                {
                    m_taskData.emplace_back(createTaskInfoWidget(data));
                    m_taskInfoLayout->addWidget(m_taskData.back());
                }
            }
            else
            {
                for (const auto& data: shiftTaskData)
                {
                    m_taskData.emplace_back(createTaskInfoWidget(data));
                    m_taskInfoLayout->addWidget(m_taskData.back());
                }
                m_taskInfo->setVisible(true);
            }
        }
        else
        {
            QMessageBox::warning(this,
                                 QStringLiteral("Ошибка просмотра статистики"),
                                 QStringLiteral("В указанную дату ") + m_shiftFinder->text() + QStringLiteral(" задачи не выполнялись"));
        }
    }
}

QWidget* ShiftTableView::createTaskInfoWidget(const ShiftData& data)
{
    auto* taskInfoWidget = new QWidget(m_taskInfo);
    auto* taskLayout = new QHBoxLayout(taskInfoWidget);
    auto* ledIndicator = new LedIndicator(this);
    if (data.isComplete)
        ledIndicator->setColor(Resources::classicGreen, QStringLiteral("Задача была завершена"));
    else
        ledIndicator->setColor(Resources::deepBlue, QStringLiteral("Задача была в работе"));
    taskLayout->addWidget(ledIndicator);
    taskLayout->addSpacing(5);

    const QFont localFont{Resources::baseFont, 13};
    auto* name = new QLabel(QString::fromStdString(data.taskName), taskInfoWidget);
    name->setFont(localFont);
    taskLayout->addWidget(name);
    taskLayout->addStretch(1);

    auto* done = new QLabel(QStringLiteral("Выполнено ") + QString::number(data.doneForShift) + QStringLiteral("/") + QString::number(data.needToDo), taskInfoWidget);
    done->setFont(localFont);
    taskLayout->addWidget(done);

    return taskInfoWidget;
}
