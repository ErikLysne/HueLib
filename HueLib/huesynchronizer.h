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

    void addHueObject(std::shared_ptr<HueAbstractObject> object);
    void removeHueObject(std::shared_ptr<HueAbstractObject> object);

private:
    explicit HueSynchronizer();
    HueSynchronizer(const HueSynchronizer& rhs) = delete;
    HueSynchronizer& operator=(HueSynchronizer& rhs) = delete;

private slots:
    void synchronize();

private:
    std::vector<std::shared_ptr<HueAbstractObject>> m_hueObjects;
    QTimer* m_timer;
    bool m_isActive;
};

#endif // HUESYNCHRONIZER_H
