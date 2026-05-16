#pragma once

#include <QWidget>
#include <QFont>

#include "Resources.hpp"
#include "TaskData.hpp"
#include "ChangedData.hpp"

QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QProgressBar)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QHBoxLayout)

class LedIndicator;

class TaskWidget : public QWidget
{
    Q_OBJECT
    static constexpr auto* s_progressToolTip {"Процент завершения задачи"};
    static constexpr auto* s_addData {"Внести данные"};
    static constexpr int s_spacing {10};
    const QFont m_baseFont{Resources::baseFont, 12};
    Tasks::TaskData m_taskData;

    QLabel* m_taskName {nullptr};
    QLabel* m_completeStatus{nullptr};
    QLabel* m_releaseDate {nullptr};
    LedIndicator* m_priority {nullptr};
    QProgressBar* m_progressBar{nullptr};
    QPushButton* m_addTaskData {nullptr};
    QHBoxLayout* m_rightLayout {nullptr};

    QString constructCompleteString();
    void applyTaskData();
    bool isTaskFinished() const;

private slots:
    void onAddTaskDataClick();
public:
    explicit TaskWidget(const Tasks::TaskData& taskData,
                        QWidget *parent = nullptr);
    void setTaskData(Tasks::TaskData && taskData);
    int getTaskNameWidth() const;
    void addSpacing(int layoutSpacing);
    std::string getTaskName() const;
signals:
    void taskDataChanged(const ChangedData& data);
};
