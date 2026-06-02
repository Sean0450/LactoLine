#pragma once

#include <QWidget>
#include <QRegularExpression>

#include "Shifts/ShiftData.hpp"
#include "Resources.hpp"

QT_FORWARD_DECLARE_CLASS(QLineEdit)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QGroupBox)
QT_FORWARD_DECLARE_CLASS(QVBoxLayout)

class ShiftTaskInformationObserver;

class ShiftTableView: public QWidget
{
    Q_OBJECT
    const QFont m_baseFont{Resources::baseFont, 14};
    const QString m_reminderText {"Введите дату рабочей смены"};
    std::vector<QWidget*> m_taskData;
    QGroupBox* m_taskInfo {nullptr};
    QVBoxLayout* m_taskInfoLayout{nullptr};
    QLineEdit* m_shiftFinder {nullptr};
    QPushButton* m_findShift {nullptr};
    ShiftTaskInformationObserver* m_observer {nullptr};
    static const inline QRegularExpression s_dateRegular {"((0[1-9]|[12][0-9]|3[01])\\.([0][1-9]|1[0-2])\\.2[0-9]([2-9][6-9]|[3-9][0-9]))"};

    QWidget* createTaskInfoWidget(const ShiftData& data);
private slots:
    void checkFinderText();
    void onFindShiftClicked();
public:
    explicit ShiftTableView(ShiftTaskInformationObserver* observer, QWidget* parent = nullptr);
};
