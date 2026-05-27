#pragma once

#include <QGroupBox>

#include "TaskData.hpp"
#include "Resources.hpp"


QT_FORWARD_DECLARE_CLASS(QVBoxLayout);
class TaskWidget;
class TaskChangedObserver;

class TaskGroup : public QGroupBox
{
    Q_OBJECT
    const QFont m_baseFont {Resources::baseFont, 16};

protected:
    std::vector<TaskWidget*> m_tasks;
    TaskChangedObserver* m_observer {nullptr};
    QVBoxLayout* m_mainLayout {nullptr};
public:
    explicit TaskGroup(const QString& title, std::vector<Tasks::TaskData>&& data, TaskChangedObserver* observer, QWidget *parent = nullptr);
    virtual void addWidget(const Tasks::TaskData& data, TaskChangedObserver* observer);
    void setEnabled(bool enabled);
    QStringList getTaskNames() const;
    TaskWidget* getWidget(const QString& taskName);
    bool hasTasks() const;
    virtual ~TaskGroup() = default;
};
