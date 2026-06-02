#pragma once

#include <QRegularExpression>

#include "DataCreationDialog.hpp"
#include "Tasks/TaskData.hpp"

QT_FORWARD_DECLARE_CLASS(QLineEdit)
QT_FORWARD_DECLARE_CLASS(QComboBox)
QT_FORWARD_DECLARE_CLASS(QHBoxLayout)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QCompleter)
QT_FORWARD_DECLARE_CLASS(QCheckBox)

class CreateTaskDialog : public DataCreationDialog
{
    Q_OBJECT
    const QStringList& m_productNames;
    static const inline QRegularExpression s_dateRegular {"((0[1-9]|[12][0-9]|3[01])\\.([0][1-9]|1[0-2])\\.2[0-9]([2-9][6-9]|[3-9][0-9]))"};
    static constexpr double s_minToDoAmount {1.0};

    static constexpr int s_taskNameIndex {0};
    static constexpr int s_toDoIndex {1};
    static constexpr int s_productNameIndex {2};
    static constexpr int s_doneIndex {3};
    static constexpr int s_wastedRawMaterialsIndex {4};
    static constexpr int s_releaseDateIndex {5};

    Tasks::TaskData m_createdTask;
    QCompleter* m_productNameCompleter {nullptr};
    QCheckBox* m_moveToCurrentShift {nullptr};

    QCompleter* createCompleter();
    void checkToDoMoreThanDone();
    void createTask();
public:
    explicit CreateTaskDialog(const QStringList& productName, QWidget *parent = nullptr);
    std::optional<Tasks::TaskData> getTaskData();
    bool isTaskForCurrentShift() const;
};
