#ifndef HUEREPLY_H
#define HUEREPLY_H

#include <QJsonObject>
#include <QVariant>

#include "hueerror.h"

class HueReply
{
public:
    HueReply();
    HueReply(bool isValid, bool timedOut, QJsonObject json,
             int httpStatus, HueError error);
    HueReply(const HueReply& rhs);
    HueReply operator=(const HueReply& rhs);

    bool isValid() const;
    bool timedOut() const;
    bool containsError() const;
    QJsonObject getJson() const;
    int getHttpStatus() const;
    HueError getError() const;

    void isValid(const bool replyValid);
    void timedOut(const bool timedOut);
    void setJson(const QJsonObject json);
    void setHttpStatus(const int httpStatus);
    void setError(const HueError error);

    operator QString() const;

private:
    bool m_replyValid;
    bool m_timedOut;
    QJsonObject m_json;
    int m_httpStatus;
    HueError m_error;
};

#endif // HUEREPLY_H
