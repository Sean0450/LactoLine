#include "TaskWidget.hpp"
#include <QLabel>
#include <QProgressBar>
#include <QHBoxLayout>
#include <QPushButton>

#include "LedIndicator.hpp"
#include "Dialogs/TaskDataDialog.hpp"
#include "Commands/ToDoProductChangedCommand.hpp"
#include "TaskChangedObserver.hpp"


TaskWidget::TaskWidget(const Tasks::TaskData& taskData,
                       TaskChangedObserver* observer,
                       QWidget *parent)
    : QWidget{parent},
      m_taskData(taskData),
      m_taskChangedObserver(observer)
{
    auto* mainLayout = new QVBoxLayout(this);

    auto* leftLayout = new QHBoxLayout();
    leftLayout->setSpacing(s_spacing);

    m_priority = new LedIndicator(this);
    leftLayout->addWidget(m_priority);

    m_taskName = new QLabel(this);
    m_taskName->setFont(m_baseFont);
    leftLayout->addWidget(m_taskName);
    leftLayout->addStretch(1);

    m_completeStatus = new QLabel(this);
    m_completeStatus->setFont(m_baseFont);
    leftLayout->addWidget(m_completeStatus);

    const QString style = QLatin1String(Resources::Styles::baseElementsStyle) + QStringLiteral("}");

    m_progressBar = new QProgressBar(this);
    m_progressBar->setStyleSheet(QStringLiteral("QProgressBar {") + style +
                                 QStringLiteral("QProgressBar::chunk {"
                                 "background-color: #05B8CC;"
                                 "width: 10px;"
                                 "}"));
    m_progressBar->setToolTip(QLatin1String(s_progressToolTip));
    leftLayout->addWidget(m_progressBar);

    m_addTaskData = new QPushButton(this);
    m_addTaskData->setFont(m_baseFont);
    m_addTaskData->setText(QStringLiteral("Внести данные"));
    m_addTaskData->setStyleSheet("QPushButton {" + style +
                                 QLatin1String(Resources::Styles::pressedButtonStyle));
    connect(m_addTaskData, &QPushButton::clicked, this, &TaskWidget::onAddTaskDataClick);
    leftLayout->addWidget(m_addTaskData);

    m_releaseDate = new QLabel(this);
    m_releaseDate->setFont(m_baseFont);
    m_releaseDate->setText(QStringLiteral("<b>Выполнить до: </b>") + QString::fromStdString(m_taskData.releaseDate));

    auto* dateLayout = new QHBoxLayout();
    dateLayout->addSpacing(m_priority->width() + s_spacing);
    dateLayout->addWidget(m_releaseDate);

    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(dateLayout);
    mainLayout->addStretch(1);

    applyTaskData();
}

int TaskWidget::getTaskNameWidth() const
{
    return m_taskName->text().size();
}

QString TaskWidget::constructCompleteString()
{
    QString result {QStringLiteral("Выполнено ")};
    if (m_completeStatus && m_progressBar)
    {
        result += QString::number(m_progressBar->value()) + "\\" +
                  QString::number(m_progressBar->maximum());
    }
    return result;
}

void TaskWidget::onAddTaskDataClick()
{
    auto dialog = TaskDataDialog(m_taskData.doneProduct, this);
    dialog.exec();
    if (m_taskData.doneProduct < dialog.getCreatedProduct())
    {
        m_taskData.doneProduct = dialog.getCreatedProduct();
        if (m_taskChangedObserver)
            m_taskChangedObserver->doneProductAmountChanged(m_taskData.getIdentifier(), dialog.getCreatedProduct(), dialog.getWastedRawMaterials());
        if (m_taskData.doneProduct > m_taskData.productToDoAmount)
        {
            m_taskData.productToDoAmount = m_taskData.doneProduct;
            if (m_taskChangedObserver)
                m_taskChangedObserver->taskDataChanged(std::make_unique<ToDoProductChangedCommand>(m_taskData));
        }
        applyTaskData();
    }
}

void TaskWidget::applyTaskData()
{
    bool isFinished {isTaskFinished()};
    if (m_taskName && !isFinished)
    {
        m_taskName->setText(QString::fromStdString(m_taskData.taskName));
    }

    if (m_priority && !isFinished)
    {
        Color color;
        QString toolTipText;
        switch(m_taskData.status)
        {
        case GeneralValues::PriorityStatus::Extra:
            color = Resources::birghtRed;
            toolTipText = QStringLiteral("Важнейшая задача");
            break;
        case GeneralValues::PriorityStatus::High:
            color = Resources::brightOrange;
            toolTipText = QStringLiteral("Приоритетная задача");
            break;
        case GeneralValues::PriorityStatus::Medium:
            color = Resources::deepBlue;
            toolTipText = QStringLiteral("Задача средней важности");
            break;
        case GeneralValues::PriorityStatus::Low:
            color = Resources::brightGreen;
            toolTipText = QStringLiteral("Второстепенная задача");
        }
        m_priority->setColor(color, toolTipText);
    }

    if (m_progressBar)
    {
        m_progressBar->setRange(0, m_taskData.productToDoAmount);
        m_progressBar->setValue(m_taskData.doneProduct);
        if (isTaskFinished())
        {
            m_addTaskData->setEnabled(false);
            m_addTaskData->setText(QStringLiteral("Завершена"));
        }
    }

    if (m_completeStatus)
    {
        m_completeStatus->setText(constructCompleteString());
    }
}

bool TaskWidget::isTaskFinished() const
{
    return m_taskData.productToDoAmount - m_taskData.doneProduct < 0.0001;
}

void TaskWidget::setTaskData(Tasks::TaskData && taskData)
{
    m_taskData = std::move(taskData);
    applyTaskData();
}

std::string TaskWidget::getTaskIdentifier() const
{
    return m_taskData.getIdentifier();
}

std::string TaskWidget::getTaskName() const
{
    return m_taskData.taskName;
}

void TaskWidget::setObserver(TaskChangedObserver* observer)
{
    m_taskChangedObserver = observer;
}
