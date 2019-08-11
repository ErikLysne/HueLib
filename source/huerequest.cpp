#include "huerequest.h"

/*!
 * \class HueRequest
 * \ingroup HueLib
 * \inmodule HueLib
 * \brief Contains the details of request sent to \l HueBridge::sendRequest().
 *
 * HueRequest holds the \e URL \e path \e extention, \e JSON and \e method sent to
 * \l HueBridge::sendRequest().
 *
 * The \e URL \e path \e extention will be appended to the base URL which contains
 * the IP address and username of the bridge. It determines which resource to address
 * and which property to retrieve/modify. Is a \e QString.
 *
 * \e JSON is the body of the request. It specifies modifications on the bridge.
 *
 * \l Method is either \e GET, \e PUT or \e POST.
 *
 *
 */

/*!
 * \enum HueRequest::Method
 * This enum defines which method should be used when calling \l HueBridge::sendRequest().
 *
 *  \value Get
 *      Send a \e GET request.
 *
 * \value Put
 *      Send a \e PUT request.
 *
 * \value Post
 *      Send a \e POST request.
 *
 * \sa getMethod()
 *
 */

/*!
 * \fn HueRequest::HueRequest(QString urlPath, QJsonObject json, Method method)
 *
 * Constructs a HueRequest with contents specified by \a urlPath, \a json and \a method.
 *
 */
HueRequest::HueRequest(QString urlPath, QJsonObject json, Method method)
    : m_urlPath(urlPath)
    , m_json(json)
    , m_method(method)
{

}

/*!
 * \fn HueRequest::HueRequest(const HueRequest& rhs)
 *
 * Copy constructor - constructs a copy of \a rhs.
 *
 */
HueRequest::HueRequest(const HueRequest& rhs)
    : m_urlPath(rhs.m_urlPath)
    , m_json(rhs.m_json)
    , m_method(rhs.m_method)
{

}

/*!
 * \fn HueRequest HueRequest::operator=(const HueRequest &rhs)
 *
 * Copy assignment operator - assigns a copy of \a rhs.
 *
 */
HueRequest HueRequest::operator=(const HueRequest &rhs)
{
    if (&rhs == this)
        return *this;

    m_urlPath = rhs.m_urlPath;
    m_json = rhs.m_json;
    m_method = rhs.m_method;

    return *this;
}

/*!
 * \fn QString HueRequest::getUrlPath() const
 *
 * Returns URL path extention of request as a \e QString.
 *
 * \sa getJson(), getMethod()
 *
 */
QString HueRequest::getUrlPath() const
{
    return m_urlPath;
}


/*!
 * \fn QJsonObject HueRequest::getJson() const
 *
 * Returns JSON from request as a \e QJsonObject.
 *
 * \sa getUrlPath(), getMethod()
 *
 */
QJsonObject HueRequest::getJson() const
{
    return m_json;
}

/*!
 * \fn HueRequest::Method HueRequest::getMethod() const
 *
 * Returns method from request as a \e HueRequest::Method.
 *
 * \sa getUrlPath(), getJson()
 *
 */
HueRequest::Method HueRequest::getMethod() const
{
    return m_method;
}
