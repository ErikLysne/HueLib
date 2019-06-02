#ifndef HUEREPLY_H
#define HUEREPLY_H

#include <QJsonObject>
#include <QVariant>

class HueReply
{
public:
    HueReply();
    HueReply(bool isValid, bool timedOut, QJsonObject json, int httpStatus,
             int errorType, QString errorAddress, QString errorDescription);
    HueReply(const HueReply& rhs);
    HueReply operator=(const HueReply& rhs);

    bool isValid() const;
    bool timedOut() const;
    QJsonObject getJson() const;
    int getHttpStatus() const;
    int getErrorType() const;
    QString getErrorAddress() const;
    QString getErrorDescription() const;

    void isValid(const bool replyValid);
    void timedOut(const bool timedOut);
    void setJson(const QJsonObject json);
    void setHttpStatus(const int httpStatus);
    void setErrorType(const int errorType);
    void setErrorAddress(const QString errorAddress);
    void setErrorDescription(const QString errorDescription);

private:
    bool m_replyValid;
    bool m_timedOut;
    QJsonObject m_json;
    int m_httpStatus;
    int m_errorType;
    QString m_errorAddress;
    QString m_errorDescription;
};

Q_DECLARE_METATYPE(HueReply);

#endif // HUEREPLY_H
