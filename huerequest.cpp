#include "huerequest.h"

HueRequest::HueRequest(QString urlPath, QJsonObject json, Method method) :
    m_urlPath(urlPath),
    m_json(json),
    m_method(method)
{

}

HueRequest::HueRequest(const HueRequest& rhs) :
    m_urlPath(rhs.m_urlPath),
    m_json(rhs.m_json),
    m_method(rhs.m_method)
{

}

HueRequest HueRequest::operator=(const HueRequest &rhs)
{
    if (&rhs == this)
        return *this;

    m_urlPath = rhs.m_urlPath;
    m_json = rhs.m_json;
    m_method = rhs.m_method;

    return *this;
}

QString HueRequest::getUrlPath() const
{
    return m_urlPath;
}

QJsonObject HueRequest::getJson() const
{
    return m_json;
}

HueRequest::Method HueRequest::getMethod() const
{
    return m_method;
}
