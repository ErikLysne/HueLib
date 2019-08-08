#ifndef HUEERROR_H
#define HUEERROR_H

#include <QString>

class HueError
{
public:
    HueError();
    HueError(int type, QString address, QString description);

    int getType() const;
    QString getAddress() const;
    QString getDescription() const;

    void setType(const int type);
    void setAddress(const QString address);
    void setDescription(const QString description);

    operator QString() const;

private:
    int m_type;
    QString m_address;
    QString m_description;
};

#endif // HUEERROR_H
