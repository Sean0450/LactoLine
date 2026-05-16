#include "TaskWidget.hpp"
#include <QLabel>
#include <QProgressBar>
#include <QHBoxLayout>
#include <QPushButton>

#include "LedIndicator.hpp"
#include "Dialogs/TaskDataDialog.hpp"


TaskWidget::TaskWidget(const Tasks::TaskData& taskData,
                       QWidget *parent)
    : QWidget{parent},
      m_taskData(taskData)
{
    auto* mainLayout = new QVBoxLayout(this);
    auto* mainTaskInfoLayout = new QHBoxLayout();

    auto* leftLayout = new QHBoxLayout();
    leftLayout->setSpacing(s_spacing);

    m_priority = new LedIndicator(this);
    leftLayout->addWidget(m_priority);

    m_taskName = new QLabel(this);
    m_taskName->setFont(m_baseFont);
    leftLayout->addWidget(m_taskName);

    m_rightLayout = new QHBoxLayout();
    m_rightLayout->setSpacing(s_spacing);
    m_rightLayout->addStretch();

    m_completeStatus = new QLabel(this);
    m_completeStatus->setFont(m_baseFont);
    m_rightLayout->addWidget(m_completeStatus);

    const QString style = QLatin1String(Resources::Styles::baseElementsStyle);

    m_progressBar = new QProgressBar(this);
    m_progressBar->setStyleSheet("QProgressBar {" + style + "}" +
                                 "QProgressBar::chunk {"
                                 "background-color: #05B8CC;"
                                 "width: 10px;"
                                 "}");
    m_progressBar->setToolTip(QLatin1String(s_progressToolTip));
    m_rightLayout->addWidget(m_progressBar);

    m_addTaskData = new QPushButton(this);
    m_addTaskData->setFont(m_baseFont);
    m_addTaskData->setText(s_addData);
    m_addTaskData->setStyleSheet("QPushButton {" + style + "}" +
                                 QLatin1String(Resources::Styles::pressedButtonStyle));
    connect(m_addTaskData, &QPushButton::clicked, this, &TaskWidget::onAddTaskDataClick);
    m_rightLayout->addWidget(m_addTaskData);

    m_releaseDate = new QLabel(this);
    m_releaseDate->setFont(m_baseFont);
    m_releaseDate->setText(QStringLiteral("<b>Выполнить до: </b>") + QString::fromStdString(m_taskData.releaseDate));

    auto* dateLayout = new QHBoxLayout();
    dateLayout->addSpacing(m_priority->width() + s_spacing);
    dateLayout->addWidget(m_releaseDate);

    mainTaskInfoLayout->addLayout(leftLayout);
    mainTaskInfoLayout->addLayout(m_rightLayout);

    mainLayout->addLayout(mainTaskInfoLayout);
    mainLayout->addLayout(dateLayout);
    applyTaskData();
}

int TaskWidget::getTaskNameWidth() const
{
    return m_taskName->text().size();
}

void TaskWidget::addSpacing(int layoutSpacing)
{
    m_rightLayout->insertSpacing(0, layoutSpacing);
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
        emit taskDataChanged({m_taskData.getIdentifier(),
                              std::variant<std::string, int>(m_taskData.doneProduct),
                              TaskDataChanged::DoneAmountChanged});
        if (m_taskData.doneProduct > m_taskData.productToDoAmount)
        {
            m_taskData.productToDoAmount = m_taskData.doneProduct;
            emit taskDataChanged({m_taskData.getIdentifier(),
                                  std::variant<std::string, int>(m_taskData.productToDoAmount),
                                  TaskDataChanged::ToDoAmountChanged});
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
        switch(m_taskData.status)
        {
        case GeneralValues::PriorityStatus::Extra:
            color = Resources::extraPriority;
            break;
        case GeneralValues::PriorityStatus::High:
            color = Resources::highPriority;
            break;
        case GeneralValues::PriorityStatus::Medium:
            color = Resources::mediumPriority;
            break;
        case GeneralValues::PriorityStatus::Low:
            color = Resources::lowPriority;
        }
        m_priority->setColor(color);
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

std::string TaskWidget::getTaskName() const
{
    return m_taskData.taskName;
}
