#ifndef HUESYNCHRONIZER_H
#define HUESYNCHRONIZER_H

#include <QObject>
#include <QTimer>

#define DEFAULT_HUE_SYNC_INTERVAL_MILLISECONDS 5000

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
    static void setSyncIntervalMilliSec(int intervalMilliSec);
    static int clearAll();
    static int clearGroups();
    static int clearLights();

    void start();
    void stop();

    int listSize();
    int clear(ClearCondition condition);

    bool isActive();

    bool addHueObject(std::shared_ptr<HueAbstractObject> object);
    bool removeHueObject(std::shared_ptr<HueAbstractObject> object);

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
