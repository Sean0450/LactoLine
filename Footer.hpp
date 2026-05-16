#pragma once
#include <QWidget>
QT_FORWARD_DECLARE_CLASS(QLabel)

class Footer : public QWidget
{
    Q_OBJECT
    const QString m_currentDate;
    QLabel* m_mainLabel {nullptr};
public:
    explicit Footer(QWidget *parent = nullptr);
};
