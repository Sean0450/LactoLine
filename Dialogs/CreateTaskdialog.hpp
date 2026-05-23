#pragma once

#include <QDialog>
#include <QRegularExpression>

#include "Resources.hpp"
#include "Tasks/TaskData.hpp"

QT_FORWARD_DECLARE_CLASS(QLineEdit)
QT_FORWARD_DECLARE_CLASS(QComboBox)
QT_FORWARD_DECLARE_CLASS(QHBoxLayout)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QCompleter)

class CreateTaskDialog : public QDialog
{
    Q_OBJECT
    const QFont m_baseFont {Resources::baseFont, 13};
    const QString m_baseStyle {QStringLiteral("QLineEdit {") + QLatin1String(Resources::Styles::baseElementsStyle) + QStringLiteral("}")};
    const QStringList& m_productNames;
    static const inline QRegularExpression s_dateRegular {"((0[1-9]|[12][0-9]|3[01])\\.([0][1-9]|1[0-2])\\.2[0-9]([2-9][6-9]|[3-9][0-9]))"};
    static constexpr double s_minToDoAmount {1.0};

    bool m_isNameCorrect    {false};
    bool m_isToDoCorrect    {false};
    bool m_isProductCorrect {false};
    bool m_isDoneCorrect    {true};
    bool m_isDateCorrect    {false};
    Tasks::TaskData m_createdTask;

    QLineEdit* m_taskName {nullptr};
    QLineEdit* m_toDoAmount {nullptr};
    QLineEdit* m_productName {nullptr};
    QCompleter* m_productNameCompleter {nullptr};
    QLineEdit* m_doneAmout {nullptr};
    QLineEdit* m_releaseDate {nullptr};
    QPushButton* m_createTask {nullptr};

    QLineEdit* createLineEdit();
    QHBoxLayout* createLineEditLayout(const QString& labelText, QLineEdit* lineEdit);
    QPushButton* createTaskButton();
    QCompleter* createCompleter();
    void applyTaskCreation();
    void checkToDoMoreThanDone();
    void createTask();

private slots:
    void onLineEditCheck(QLineEdit* lineEdit, bool flag);
    void onReleaseDateCheck();
    void onToDoAmountCheck();
public:
    explicit CreateTaskDialog(const QStringList& productName, QWidget *parent = nullptr);
    std::optional<Tasks::TaskData> getTaskData();
};
