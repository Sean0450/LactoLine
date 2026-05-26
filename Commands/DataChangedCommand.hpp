#pragma once

#include <string>

template <typename Dto, typename ModelType>
class DataChangedCommand
{
protected:
    Dto& m_data;
public:
    DataChangedCommand(Dto& data):m_data(data){}
    virtual ModelType getUpdatedTask(ModelType task) const = 0;
    virtual std::string getChangingField() const = 0;
    virtual std::string getChangingFieldName() const = 0;
    virtual std::string getIdentifier() const = 0;
    ~DataChangedCommand() = default;
};
