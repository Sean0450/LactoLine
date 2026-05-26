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
#include <QCheckBox>

CreateTaskDialog::CreateTaskDialog(const QStringList& productName, QWidget *parent)
    : DataCreationDialog{parent},
      m_productNames(productName)
{
    setWindowTitle(QStringLiteral("Формирование рабочей задачи"));

    auto* taskName = createLineEdit();
    createLineEditLayout(taskName, QStringLiteral("Наименование задачи:"), [&](){return !m_lineEdits[s_taskNameIndex]->text().isEmpty();});

    auto* toDoAmount = createLineEdit();
    createLineEditLayout(toDoAmount, QStringLiteral("Необходимо изготовить:"), [&](){bool flag {true};
                                                                                     double toDo = m_lineEdits[s_toDoIndex]->text().toDouble(&flag);
                                                                                     return flag && toDo > s_minToDoAmount;});

    auto* productionName = createLineEdit();
    m_productNameCompleter = createCompleter();
    productionName->setCompleter(m_productNameCompleter);
    createLineEditLayout(productionName, QStringLiteral("Наименование продукта:"), [&](){return m_productNames.contains(m_lineEdits[s_productNameIndex]->text());});
    auto* doneAmount = createLineEdit();
    doneAmount->setText(QStringLiteral("0"));
    createLineEditLayout(doneAmount, QStringLiteral("Уже изготовлено:"), [&](){bool flag {true};
                                                                               m_lineEdits[s_doneIndex]->text().toDouble(&flag);
                                                                               checkToDoMoreThanDone();
                                                                               return flag;});

    auto* releaseDate = createLineEdit();
    releaseDate->setText(QDateTime::currentDateTime().toString(QLatin1String(Resources::dateParseFormat)));
    createLineEditLayout(releaseDate, QStringLiteral("Срок выполнения:"), [&](){auto match = s_dateRegular.match(m_lineEdits[s_releaseDateIndex]->text());
                                                                                const std::string currentDate = QDateTime::currentDateTime().toString(QLatin1String(Resources::dateParseFormat)).toStdString();
                                                                                return match.hasMatch() && DateTranslator::isReleaseDateCorrect(currentDate, m_lineEdits[s_releaseDateIndex]->text().toStdString());});

    createButton(QStringLiteral("Создать задачу"), [&](){createTask();});

    m_moveToCurrentShift = new QCheckBox(QStringLiteral("Добавить к текущей смене"), this);
    m_moveToCurrentShift->setFont(QFont(Resources::baseFont, 12));
    m_mainLayot->addWidget(m_moveToCurrentShift);
}

void CreateTaskDialog::checkToDoMoreThanDone()
{
    bool toDoFlag {true};
    bool doneFlag {true};
    double toDo = m_lineEdits[s_toDoIndex]->text().toDouble(&toDoFlag);
    double done = m_lineEdits[s_doneIndex]->text().toDouble(&doneFlag);
    if (toDoFlag && doneFlag)
    {
        if (done < toDo)
            m_lineEdits[s_doneIndex]->setStyleSheet(Resources::Styles::redBorderLineEdit);
        else
            m_lineEdits[s_doneIndex]->setStyleSheet(m_baseStyle);
    }
}

void CreateTaskDialog::createTask()
{
    const std::string taskName = m_lineEdits[s_taskNameIndex]->text().toStdString();
    const double toDo = m_lineEdits[s_toDoIndex]->text().toDouble();
    const std::string productName = m_lineEdits[s_productNameIndex]->text().toStdString();
    const double done = m_lineEdits[s_doneIndex]->text().toDouble();
    const std::string releaseDate = m_lineEdits[s_releaseDateIndex]->text().toStdString();
    const GeneralValues::GUI gui = GeneralValues::Gui::generateGui();
    const std::string currentDate = QDateTime::currentDateTime().toString(QLatin1String(Resources::dateParseFormat)).toStdString();
    const GeneralValues::PriorityStatus priority = DateTranslator::calculatePriority(currentDate, releaseDate);
    m_createdTask = Tasks::TaskData{taskName, productName, releaseDate, priority, toDo, done, gui};
}

std::optional<Tasks::TaskData> CreateTaskDialog::getTaskData()
{
    std::optional<Tasks::TaskData> result;
    if (m_createdTask.productToDoAmount >= s_minToDoAmount)
    {
        result = m_createdTask;
    }
    return result;
}

bool CreateTaskDialog::isTaskForCurrentShift() const
{
    return m_moveToCurrentShift->isChecked();
}

QCompleter* CreateTaskDialog::createCompleter()
{
    auto* completer = new QCompleter(m_productNames, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    return completer;
}
