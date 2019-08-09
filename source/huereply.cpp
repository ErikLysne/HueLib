#include "huereply.h"

HueReply::HueReply()
    : m_replyValid(false)
    , m_timedOut(false)
    , m_json()
    , m_httpStatus(0)
    , m_error()
{

}

HueReply::HueReply(bool replyValid, bool timedOut, QJsonObject json,
                   int httpStatus, HueError error)
    : m_replyValid(replyValid)
    , m_timedOut(timedOut)
    , m_json(json)
    , m_httpStatus(httpStatus)
    , m_error(error)
{

}

HueReply::HueReply(const HueReply& rhs)
    : m_replyValid(rhs.m_replyValid)
    , m_timedOut(rhs.m_timedOut)
    , m_json(rhs.m_json)
    , m_httpStatus(rhs.m_httpStatus)
    , m_error(rhs.m_error)
{

}

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

bool HueReply::isValid() const
{
    return m_replyValid;
}

bool HueReply::timedOut() const
{
    return m_timedOut;
}

bool HueReply::containsError() const
{
    return m_error.getType() != -1;
}

QJsonObject HueReply::getJson() const
{
    return m_json;
}

int HueReply::getHttpStatus() const
{
    return m_httpStatus;
}

HueError HueReply::getError() const
{
    return m_error;
}

void HueReply::isValid(bool replyValid)
{
    m_replyValid = replyValid;
}

void HueReply::timedOut(const bool timedOut)
{
    m_timedOut = timedOut;
}

void HueReply::setJson(const QJsonObject json)
{
    m_json = json;
}

void HueReply::setHttpStatus(const int httpStatus)
{
    m_httpStatus = httpStatus;
}

void HueReply::setError(const HueError error)
{
    m_error = error;
}

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
