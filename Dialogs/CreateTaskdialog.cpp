#include "CreateTaskdialog.hpp"
#include "GeneralValues/Gui.hpp"
#include "Date/DateTranslator.hpp"

#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QCompleter>
#include <QStringList>
#include <QPushButton>
#include <QDateTime>

CreateTaskDialog::CreateTaskDialog(const QStringList& productName, QWidget *parent)
    : QDialog{parent},
      m_productNames(productName)
{
    setModal(true);
    setWindowTitle(QStringLiteral("Формирование рабочей задачи"));

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(5);

    m_taskName = createLineEdit();
    connect(m_taskName, &QLineEdit::textChanged, this, [this](){m_isNameCorrect = !m_taskName->text().isEmpty();
                                                                onLineEditCheck(m_taskName, m_isNameCorrect);});

    m_toDoAmount = createLineEdit();
    connect(m_toDoAmount, &QLineEdit::textChanged, this, [this](){bool flag {true};
                                                                  m_toDoAmount->text().toDouble(&flag);
                                                                  m_isToDoCorrect = flag;
                                                                  onLineEditCheck(m_toDoAmount, m_isToDoCorrect);});

    m_productName = createLineEdit();
    m_productNameCompleter = createCompleter();
    connect(m_productName, &QLineEdit::textChanged, this, [this](){m_isProductCorrect = m_productNames.contains(m_productName->text());
                                                                   onLineEditCheck(m_productName, m_isProductCorrect);});
    if (m_productName)
        m_productName->setCompleter(m_productNameCompleter);

    m_doneAmout = createLineEdit();
    m_doneAmout->setText(QStringLiteral("0"));
    connect(m_doneAmout, &QLineEdit::textChanged, this, [this](){bool flag {true};
                                                                 m_doneAmout->text().toDouble(&flag);
                                                                 m_isDoneCorrect = flag;
                                                                 onLineEditCheck(m_doneAmout, m_isDoneCorrect);});

    m_releaseDate = createLineEdit();
    m_releaseDate->setText(QStringLiteral("xx.xx.xxxx"));
    connect(m_releaseDate, &QLineEdit::textChanged, this, [this](){auto match = s_dateRegular.match(m_releaseDate->text());
                                                                   m_isDateCorrect = match.hasMatch();
                                                                   onLineEditCheck(m_releaseDate, m_isDateCorrect);});

    mainLayout->addLayout(createLineEditLayout(QStringLiteral("Наименование задачи:"),    m_taskName));
    mainLayout->addLayout(createLineEditLayout(QStringLiteral("Наименование продукта:"),  m_productName));
    mainLayout->addLayout(createLineEditLayout(QStringLiteral("Необходимо приготовить:"), m_toDoAmount));
    mainLayout->addLayout(createLineEditLayout(QStringLiteral("Уже изготовлено:"),        m_doneAmout));
    mainLayout->addLayout(createLineEditLayout(QStringLiteral("Срок выполнения:"),        m_releaseDate));

    m_createTask = createTaskButton();
    connect(m_createTask, &QPushButton::clicked, this, [this](){close();});
    m_createTask->setEnabled(false);
    auto* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(m_createTask);

    mainLayout->addSpacing(10);
    mainLayout->addLayout(buttonLayout);
}

void CreateTaskDialog::checkToDoMoreThanDone()
{
    bool toDoFlag {true};
    bool doneFlag {true};
    double toDo = m_toDoAmount->text().toDouble(&toDoFlag);
    double done = m_doneAmout->text().toDouble(&doneFlag);
    if (toDoFlag && doneFlag)
    {
        m_isDoneCorrect = done < toDo;
        if (!m_isDoneCorrect)
            m_doneAmout->setStyleSheet(Resources::Styles::redBorderLineEdit);
    }
}

QLineEdit* CreateTaskDialog::createLineEdit()
{
    auto* lineEdit = new QLineEdit(this);
    lineEdit->setStyleSheet(m_baseStyle);
    lineEdit->setFont(m_baseFont);
    return lineEdit;
}

QHBoxLayout* CreateTaskDialog::createLineEditLayout(const QString& labelText, QLineEdit* lineEdit)
{
    auto* layout = new QHBoxLayout();
    auto* label = new QLabel(labelText, this);
    layout->setSpacing(5);
    label->setFont(m_baseFont);
    layout->addWidget(label);
    layout->addStretch(1);
    layout->addWidget(lineEdit);
    return layout;
}

std::optional<Tasks::TaskData> CreateTaskDialog::getTaskData()
{
    std::optional<Tasks::TaskData> result;
    applyTaskCreation();
    if (m_createTask->isEnabled())
    {
        const std::string taskName = m_taskName->text().toStdString();
        const std::string productName = m_productName->text().toStdString();
        const std::string releaseDate = m_releaseDate->text().toStdString();
        const double toDo = m_toDoAmount->text().toDouble();
        const double done = m_doneAmout->text().toDouble();
        const GeneralValues::GUI gui = GeneralValues::Gui::generateGui();
        const std::string currentDate = QDateTime::currentDateTime().toString(QStringLiteral("dd.MM.yyyy")).toStdString();
        const GeneralValues::PriorityStatus priority = DateTranslator::calculatePriority(currentDate, releaseDate);
        result = Tasks::TaskData{taskName, productName, releaseDate, priority, toDo, done, gui};
    }
    return result;
}

QPushButton* CreateTaskDialog::createTaskButton()
{
    auto* button = new QPushButton(QStringLiteral("Создать задачу"), this);
    button->setFont(m_baseFont);
    button->setStyleSheet(QStringLiteral("QPushButton {") + Resources::Styles::baseElementsStyle + "}" + Resources::Styles::pressedButtonStyle);
    return button;
}

QCompleter* CreateTaskDialog::createCompleter()
{
    auto* completer = new QCompleter(m_productNames, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    return completer;
}

void CreateTaskDialog::onLineEditCheck(QLineEdit* lineEdit, bool flag)
{
    if (flag)
        lineEdit->setStyleSheet(m_baseStyle);
    else
        lineEdit->setStyleSheet(Resources::Styles::redBorderLineEdit);
    applyTaskCreation();
}

void CreateTaskDialog::applyTaskCreation()
{
    checkToDoMoreThanDone();
    if (m_isNameCorrect    &&
        m_isProductCorrect &&
        m_isToDoCorrect    &&
        m_isDoneCorrect    &&
        m_isDateCorrect)
    {
        m_createTask->setEnabled(true);
    }
    else
    {
        m_createTask->setEnabled(false);
    }
}
