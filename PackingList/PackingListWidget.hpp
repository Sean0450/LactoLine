#pragma once

#include <QWidget>

QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QVBoxLayout)
QT_FORWARD_DECLARE_CLASS(QPushButton)

class PackingListObserver;
class GoodWidget;

class PackingList: public QWidget
{
    static constexpr int s_baseFiledsCount {5};
    std::vector<GoodWidget*> m_goods;
    QVBoxLayout* m_mainLayout {nullptr};
    QLabel* m_pageTitle {nullptr};
    QPushButton* m_totalButton {nullptr};
    QLabel* m_totalLabel {nullptr};
    QPushButton* m_addField {nullptr};
    QPushButton* m_removeField {nullptr};
    QPushButton* m_createDocument {nullptr};
    PackingListObserver* m_observer {nullptr};
    int m_selectedGoodNum {-1};

    void connectNewGood();
private slots:
    void onAddFieldButtonClicked();
    void onRemoveFieldButtonClicked();
    void onGoodSelected(int num);
    void onTotalClicked();
    void onCreateDocumentClicked();
public:
    explicit PackingList(PackingListObserver* observer, QWidget* parent = nullptr);
};
