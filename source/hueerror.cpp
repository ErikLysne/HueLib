#include "hueerror.h"

HueError::HueError()
    : m_type(-1)
    , m_address("")
    , m_description("")
{

}

HueError::HueError(int type, QString address, QString description)
    : m_type(type)
    , m_address(address)
    , m_description(description)
{

}

int HueError::getType() const
{
    return m_type;
}

QString HueError::getAddress() const
{
    return m_address;
}

QString HueError::getDescription() const
{
    return m_description;
}

void HueError::setType(const int type)
{
    m_type = type;
}

void HueError::setAddress(const QString address)
{
    m_address = address;
}

void HueError::setDescription(const QString description)
{
    m_description = description;
}

HueError::operator QString() const
{
    QString retval = "";
    retval += "ERROR:\n";
    retval += "..................................................................\n";
    retval += "Type:\t\t";        retval += QString::number(m_type);  retval += "\n";
    retval += "Address:\t\t";     retval += m_address;                retval += "\n";
    retval += "Description:\t"; retval += m_description;            retval += "\n";
    retval += "..................................................................\n";

    return retval;

}
