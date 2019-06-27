#include "huesynchronizer.h"
#include "hueabstractobject.h"

HueSynchronizer::HueSynchronizer() :
    m_hueObjects(),
    m_timer(new QTimer(this)),
    m_isActive(false)
{
    m_timer->setSingleShot(false);
    m_timer->setInterval(defaultSyncIntervalMilliSec);

    connect(m_timer, &QTimer::timeout,
            this, &HueSynchronizer::synchronize);

}

HueSynchronizer& HueSynchronizer::instance()
{
    static HueSynchronizer singleton;
    return singleton;
}

void HueSynchronizer::start()
{
    m_isActive = true;
    m_timer->start();
}

void HueSynchronizer::stop()
{
    m_isActive = false;
    m_timer->stop();
}

bool HueSynchronizer::isActive()
{
    return m_isActive;
}

void HueSynchronizer::addHueObject(HueAbstractObject* object)
{
    m_hueObjects.push_back(object);

    if (!isActive())
        start();
}

void HueSynchronizer::removeHueObject(HueAbstractObject* object)
{
    m_hueObjects.remove(object);

    if (m_hueObjects.empty())
        stop();
}

void HueSynchronizer::setSyncIntervalMilliSec(int intervalMilliSec)
{
    m_timer->setInterval(intervalMilliSec);
}

void HueSynchronizer::synchronize()
{
    for (auto hueObject : m_hueObjects) {
        hueObject->synchronize();
    }
}
