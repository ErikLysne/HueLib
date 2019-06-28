#ifndef HUESYNCHRONIZER_H
#define HUESYNCHRONIZER_H

#include <QObject>
#include <QTimer>

#define DEFAULT_HUE_SYNC_INTERVAL_MILLISECONDS 10000

class HueAbstractObject;

class HueSynchronizer : public QObject
{
    Q_OBJECT

public:
    static HueSynchronizer& instance();
    static void setSyncIntervalMilliSec(int intervalMilliSec);

    void start();
    void stop();

    bool isActive();

    void addHueObject(HueAbstractObject* object);
    void removeHueObject(HueAbstractObject* object);

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
