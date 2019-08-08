#ifndef HUEREQUEST_H
#define HUEREQUEST_H

#include <QString>
#include <QJsonObject>

class HueRequest
{
public:
    enum Method {
        get,
        put,
        post
    };

    HueRequest(QString urlPath, QJsonObject json, Method method);
    HueRequest(const HueRequest& rhs);
    HueRequest operator=(const HueRequest& rhs);

    QString getUrlPath() const;
    QJsonObject getJson() const;
    Method getMethod() const;

private:
    QString m_urlPath;
    QJsonObject m_json;
    Method m_method;
};

#endif // HUEREQUEST_H
