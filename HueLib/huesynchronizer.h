#ifndef HUESYNCHRONIZER_H
#define HUESYNCHRONIZER_H

#include <QObject>
#include <QTimer>

class HueAbstractObject;

class HueSynchronizer : public QObject
{
    Q_OBJECT
private:
    enum ClearCondition {
        ClearAll,
        ClearGroups,
        ClearLights
    };

public:
    static HueSynchronizer& instance();
    static void setSyncInterval(int intervalMilliSec);
    static int clearAll();
    static int clearGroups();
    static int clearLights();

    void start();
    void stop();

    int listSize();
    bool isActive();

    bool addHueObject(std::shared_ptr<HueAbstractObject> object);
    bool removeHueObject(std::shared_ptr<HueAbstractObject> object);

private:
    explicit HueSynchronizer();
    HueSynchronizer(const HueSynchronizer& rhs) = delete;
    HueSynchronizer& operator=(HueSynchronizer& rhs) = delete;
    int clear(ClearCondition condition);

private slots:
    void synchronize();

private:
    const int defaultSyncIntervalMilliSec = 5000;

    std::vector<std::shared_ptr<HueAbstractObject>> m_hueObjects;
    QTimer* m_timer;
    bool m_isActive;
};

#endif // HUESYNCHRONIZER_H
