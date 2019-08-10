#include "huereply.h"

/*!
 * \class HueReply
 * \ingroup HueLib
 * \brief Contains the details of reply received from \l HueBridge::sendRequest().
 *
 * HueReply holds the \e JSON, \e HTTP \e status, and \e error returned from \l HueBridge::sendRequest().
 * Also holds booleans specifying if the reply is \e valid and if it \e timed \e out.
 *
 * In case a \e GET request was sent, JSON contains the raw JSON reply. If a \e PUT or \e POST request
 * was sent successfully, or \e GET/ \e PUT/ \e POST request returned with a JSON error, JSON contains
 * the object contained within the \e success/ \e error reply (i.e. JSON is stripped of the outer \e success
 * / \e error brackets). This is becasue \e success and \e error JSON are returned as JSON \b arrays while
 * replies to \e GET requests are returned as JSON \b objects. JSON is always a \e QJsonObject.
 *
 * \e HTTP \e status is the HTTP status of the reply. If the status is not 200, the reply is not valid.
 * \note replies with 200 status can still contain JSON errors. Instead of testing for 200, use \l isValid().
 * \e HTTP \e status is an int.
 *
 * \e error contans details about any errors in the reply, and is a \l HueError object.
 *
 */

/*!
 * \fn HueReply::HueReply()
 *
 * Constructs an empty HueReply.
 *
 */
HueReply::HueReply()
    : m_replyValid(false)
    , m_timedOut(false)
    , m_json()
    , m_httpStatus(0)
    , m_error()
{

}

/*!
 * \fn HueReply::HueReply(bool replyValid, bool timedOut, QJsonObject json, int httpStatus, HueError error)
 *
 * Constructs a HueReply with contents specified by \a replyValid, \a timedOut, \a json, \a httpStatus and \a error.
 *
 */
HueReply::HueReply(bool replyValid, bool timedOut, QJsonObject json,
                   int httpStatus, HueError error)
    : m_replyValid(replyValid)
    , m_timedOut(timedOut)
    , m_json(json)
    , m_httpStatus(httpStatus)
    , m_error(error)
{

}

/*!
 * \fn HueReply::HueReply(const HueReply& rhs)
 *
 * Copy constructor - constructs a copy of \a rhs.
 *
 */
HueReply::HueReply(const HueReply& rhs)
    : m_replyValid(rhs.m_replyValid)
    , m_timedOut(rhs.m_timedOut)
    , m_json(rhs.m_json)
    , m_httpStatus(rhs.m_httpStatus)
    , m_error(rhs.m_error)
{

}

/*!
 * \fn HueReply HueReply::operator=(const HueReply& rhs)
 *
 * Copy assignment operator - assigns a copy of \a rhs.
 *
 */
HueReply HueReply::operator=(const HueReply& rhs)
{
    if (&rhs == this)
        return *this;

    m_replyValid = rhs.m_replyValid;
    m_timedOut = rhs.m_timedOut;
    m_json = rhs.m_json;
    m_httpStatus = rhs.m_httpStatus;
    m_error = rhs.m_error;

    return *this;
}

/*!
 * \fn bool HueReply::isValid() const
 *
 * Returns \c true if reply is valid. A reply is valid if it does not contain any
 * errors and did not timeout.
 *
 * \sa timedOut(), containsError()
 *
 */
bool HueReply::isValid() const
{
    return m_replyValid;
}

/*!
 * \fn bool HueReply::timedOut() const
 *
 * Returns \c true if reply timed out.
 *
 * \sa isValid(), containsError()
 *
 */
bool HueReply::timedOut() const
{
    return m_timedOut;
}

/*!
 * \fn bool HueReply::containsError() const
 *
 * Returns \c true if reply contains an error.
 *
 * \sa timedOut(), containsError()
 *
 */
bool HueReply::containsError() const
{
    return m_error.getType() != -1;
}

/*!
 * \fn QJsonObject HueReply::getJson() const
 *
 * Returns JSON from reply as a \e QJsonObject.
 *
 * \sa getHttpStatus(), getError()
 *
 */
QJsonObject HueReply::getJson() const
{
    return m_json;
}

/*!
 * \fn int HueReply::getHttpStatus() const
 *
 * Returns HTTP status from reply as an \c int.
 *
 * \sa getJson(), getError()
 *
 */
int HueReply::getHttpStatus() const
{
    return m_httpStatus;
}

/*!
 * \fn HueError HueReply::getError() const
 *
 * Returns error from reply as \l HueError.
 *
 * \sa getJson(), getHttpStatus()
 *
 */
HueError HueReply::getError() const
{
    return m_error;
}

/*!
 * \fn void HueReply::isValid(const bool replyValid)
 *
 * Sets validity of the reply as specified by \a replyValid.
 *
 * \note should not be called explicitly.
 *
 */
void HueReply::isValid(const bool replyValid)
{
    m_replyValid = replyValid;
}

/*!
 * \fn void HueReply::timedOut(const bool timedOut)
 *
 * Sets timed out of the reply as specified by \a timedOut.
 *
 * \note should not be called explicitly.
 *
 */
void HueReply::timedOut(const bool timedOut)
{
    m_timedOut = timedOut;
}

/*!
 * \fn void HueReply::setJson(const QJsonObject json)
 *
 * Sets JSON of the reply as specified by \a json.
 *
 * \note should not be called explicitly.
 *
 */
void HueReply::setJson(const QJsonObject json)
{
    m_json = json;
}

/*!
 * \fn void HueReply::setHttpStatus(const int httpStatus)
 *
 * Sets HTTP status of the reply as specified by \a httpStatus.
 *
 * \note should not be called explicitly.
 *
 */
void HueReply::setHttpStatus(const int httpStatus)
{
    m_httpStatus = httpStatus;
}

/*!
 * \fn void HueReply::setError(const HueError error)
 *
 * Sets error of the reply as specified by \a error.
 *
 * \note should not be called explicitly.
 *
 */
void HueReply::setError(const HueError error)
{
    m_error = error;
}

/*!
 * \fn HueReply::operator QString() const
 *
 * Overloads \e QString() casting.
 *
 * Convenient for e.g. printing the contents of HueReply \e qDebug().
 *
 * \code
 *  HueBridge* bridge = new HueBridge("10.0.1.14");
 *  bridge->link();
 *
 *  bridge->testConnection();
 *
 *  qDebug() << bridge->getLastReply();
 *
 * \endcode
 *
 */
HueReply::operator QString() const
{
    QString retval = "";
    retval += "Reply:\n";
    retval += "..................................................................\n";
    retval += "Is valid:\t\t";          retval += (m_replyValid ? "True" : "False");            retval += "\n";
    retval += "Timed out:\t\t";         retval += (m_timedOut ? "True" : "False");              retval += "\n";
    retval += "HTTP status code:\t";    retval += QString::number(m_httpStatus);                retval += "\n";
    retval += "Contains error:\t";      retval += (m_error.getType() != -1 ? "True" : "False"); retval += "\n";

    if (m_error.getType() != -1)
        retval += QString(m_error);

    retval += "..................................................................\n";

    return retval;
}
