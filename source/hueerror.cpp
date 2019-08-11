#include "hueerror.h"

/*!
 * \class HueError
 * \ingroup HueLib
 * \inmodule HueLib
 * \brief Contains the details of an error received from \l HueBridge::sendRequest().
 *
 * HueError holds the \e type, \e address and \e description of an error returned from
 * \l HueBridge::sendRequest(). \e type is an ID of the error and is an \c int. The easiest way
 * to test for specific errors is by comparison with \e type. \e address contains the path extention
 * to the resouce that produced the error and is a \e QString. \e description provides a readable
 * description of what caused the error and is a \e QString.
 *
 * A list of errors can be found in \l https://developers.meethue.com/develop/hue-api/error-messages/.
 *
 */

/*!
 * \fn HueError::HueError()
 *
 * Constructs an empty HueError.
 *
 */
HueError::HueError()
    : m_type(-1)
    , m_address("")
    , m_description("")
{

}

/*!
 * \fn HueError::HueError(int type, QString address, QString description)
 *
 * Constructs a HueError with contents specified by \a type, \a address and
 * \a description.
 *
 */
HueError::HueError(int type, QString address, QString description)
    : m_type(type)
    , m_address(address)
    , m_description(description)
{

}

/*!
 * \fn HueError::HueError(const HueError& rhs)
 *
 * Copy constructor - constructs a copy of \a rhs.
 *
 */
HueError::HueError(const HueError& rhs)
    : m_type(rhs.m_type)
    , m_address(rhs.m_address)
    , m_description(rhs.m_description)
{

}

/*!
 * \fn HueError HueError::operator=(const HueError& rhs)
 *
 * Copy assignment operator - assigns a copy of \a rhs.
 *
 */
HueError HueError::operator=(const HueError& rhs)
{
    if (&rhs == this)
        return *this;

    m_type = rhs.m_type;
    m_address = rhs.m_address;
    m_description = rhs.m_description;

    return *this;
}

/*!
 * \fn int HueError::getType() const
 *
 * Returns the error type.
 *
 */
int HueError::getType() const
{
    return m_type;
}

/*!
 * \fn QString HueError::getAddress() const
 *
 * Returns the error address.
 *
 */
QString HueError::getAddress() const
{
    return m_address;
}

/*!
 * \fn QString HueError::getDescription() const
 *
 * Returns the error description.
 *
 */
QString HueError::getDescription() const
{
    return m_description;
}

/*!
 * \fn void HueError::setType(const int type)
 *
 * Sets the error type specified by \a type.
 *
 */
void HueError::setType(const int type)
{
    m_type = type;
}

/*!
 * \fn void HueError::setAddress(const QString address)
 *
 * Sets the address type specified by \a address.
 *
 */
void HueError::setAddress(const QString address)
{
    m_address = address;
}

/*!
 * \fn void HueError::setDescription(const QString description)
 *
 * Sets the error description specified by \a description.
 *
 */
void HueError::setDescription(const QString description)
{
    m_description = description;
}

/*!
 * \fn HueError::operator QString() const
 *
 * Overloads \e QString() casting.
 *
 * Convenient for e.g. printing an error string to \e qDebug().
 *
 * \code
 *  HueBridge* bridge = new HueBridge("10.0.1.14");
 *  bridge->link();
 *
 *  HueBridge::ConnectionStatus status;
 *  bridge->testConnection(status);
 *
 *  if (status == HueBridge::JsonError)
 *      qDebug() << bridge->getLastError();
 *
 * \endcode
 *
 */
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
