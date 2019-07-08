#include "huereply.h"

HueReply::HueReply()
    : m_replyValid(false)
    , m_timedOut(false)
    , m_json()
    , m_httpStatus(0)
    , m_errorType(0)
    , m_errorAddress("")
    , m_errorDescription("")
{

}

HueReply::HueReply(bool replyValid, bool timedOut, QJsonObject json, int httpStatus,
                   int errorType, QString errorAddress, QString errorDescription)
    : m_replyValid(replyValid)
    , m_timedOut(timedOut)
    , m_json(json)
    , m_httpStatus(httpStatus)
    , m_errorType(errorType)
    , m_errorAddress(errorAddress)
    , m_errorDescription(errorDescription)
{

}

HueReply::HueReply(const HueReply& rhs)
    : m_replyValid(rhs.m_replyValid)
    , m_timedOut(rhs.m_timedOut)
    , m_json(rhs.m_json)
    , m_httpStatus(rhs.m_httpStatus)
    , m_errorType(rhs.m_errorType)
    , m_errorAddress(rhs.m_errorAddress)
    , m_errorDescription(rhs.m_errorDescription)
{

}

HueReply HueReply::operator=(const HueReply &rhs)
{
    if (&rhs == this)
        return *this;

    m_replyValid = rhs.m_replyValid;
    m_timedOut = rhs.m_timedOut;
    m_json = rhs.m_json;
    m_httpStatus = rhs.m_httpStatus;
    m_errorType = rhs.m_errorType;
    m_errorAddress = rhs.m_errorAddress;
    m_errorDescription = rhs.m_errorDescription;

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

QJsonObject HueReply::getJson() const
{
    return m_json;
}

int HueReply::getHttpStatus() const
{
    return m_httpStatus;
}

int HueReply::getErrorType() const
{
    return m_errorType;
}

QString HueReply::getErrorAddress() const
{
    return m_errorAddress;
}

QString HueReply::getErrorDescription() const
{
    return m_errorDescription;
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

void HueReply::setErrorType(const int errorType)
{
    m_errorType = errorType;
}

void HueReply::setErrorAddress(const QString errorAddress)
{
    m_errorAddress = errorAddress;
}

void HueReply::setErrorDescription(const QString errorDescription)
{
    m_errorDescription = errorDescription;
}
