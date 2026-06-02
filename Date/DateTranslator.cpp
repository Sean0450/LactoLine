#include "DateTranslator.hpp"
#include "Date.hpp"
#include "Resources.hpp"

#include <chrono>
#include <QDateTime>
#include <QMessageBox>

GeneralValues::PriorityStatus DateTranslator::calculatePriority(const std::string& viewDateFormat, const std::string& releaseDate)
{
    using namespace GeneralValues;
    return Priority(Date::Date{releaseDate}, Date::Date{viewDateFormat}).status();
}

bool DateTranslator::isReleaseDateCorrect(const std::string& currentDate, const std::string& releaseDate)
{
    bool result {true};
    try
    {
        const Date::Date current {currentDate};
        const Date::Date release {releaseDate};
        result = release.isDateMore(current);
    }
    catch(std::exception& exc)
    {
        result = false;
        QMessageBox::warning(nullptr,
                             QStringLiteral("Ошибка ввода даты"),
                             QString::fromStdString(exc.what()));
    }
    return result;
}

bool DateTranslator::isCurrentDateMoreOrEqual(const std::string& currentDate, const std::string& dateToCompare)
{
    const Date::Date current {currentDate};
    const Date::Date compareDate {dateToCompare};
    return current.isDateMore(compareDate);
}

std::string DateTranslator::getModelCurrentDate()
{
    auto now = std::chrono::system_clock::now();
    std::chrono::year_month_day date{std::chrono::floor<std::chrono::days>(now)};
    std::string correctDate;
    constexpr int dateLength {10};
    constexpr auto spliter {'.'};
    correctDate.reserve(dateLength);
    unsigned int day = static_cast<unsigned int>(date.day());
    unsigned int month = static_cast<unsigned int>(date.month());
    int year = static_cast<int>(date.year());
    if (day < 10)
        correctDate += "0" + std::to_string(day);
    else
        correctDate += std::to_string(day);
    correctDate += spliter;
    if (month < 10)
        correctDate += "0" + std::to_string(month);
    else
        correctDate += std::to_string(month);
    correctDate += spliter;
    correctDate += std::to_string(year);
    return correctDate;
}

std::string DateTranslator::getCurrentDate()
{
    return QDateTime::currentDateTime().toString(QLatin1String(Resources::dateParseFormat)).toStdString();
}
