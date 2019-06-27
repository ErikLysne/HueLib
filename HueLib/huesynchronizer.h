#ifndef HUESYNCHRONIZER_H
#define HUESYNCHRONIZER_H

#include <QObject>
#include <QTimer>

class HueAbstractObject;

class HueSynchronizer : public QObject
{
    Q_OBJECT

public:
    const int defaultSyncIntervalMilliSec = 1000;

    static HueSynchronizer& instance();

    void start();
    void stop();

    bool isActive();

    void addHueObject(HueAbstractObject* object);
    void removeHueObject(HueAbstractObject* object);

    void setSyncIntervalMilliSec(int intervalMilliSec);

private:
    explicit HueSynchronizer();
    HueSynchronizer(const HueSynchronizer& rhs) = delete;
    HueSynchronizer& operator=(HueSynchronizer& rhs) = delete;

private slots:
    void synchronize();

private:
    std::list<HueAbstractObject*> m_hueObjects;
    QTimer* m_timer;
    bool m_isActive;
};

#endif // HUESYNCHRONIZER_H
