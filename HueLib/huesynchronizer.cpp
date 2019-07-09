#include "huesynchronizer.h"

#include "hueabstractobject.h"

#include <QtDebug>

HueSynchronizer::HueSynchronizer()
    : m_hueObjects()
    , m_timer(new QTimer(this))
    , m_isActive(false)
{
    m_timer->setSingleShot(false);
    m_timer->setInterval(DEFAULT_HUE_SYNC_INTERVAL_MILLISECONDS);

    connect(m_timer, &QTimer::timeout,
            this, &HueSynchronizer::synchronize);

}

HueSynchronizer& HueSynchronizer::instance()
{
    static HueSynchronizer singleton;
    return singleton;
}

void HueSynchronizer::setSyncIntervalMilliSec(int intervalMilliSec)
{
    instance().m_timer->setInterval(intervalMilliSec);
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
    // Need to fix out-of-range error
    /*
    auto objectPosition = std::find(m_hueObjects.begin(), m_hueObjects.end(), object);

    if (objectPosition != m_hueObjects.end())
        m_hueObjects.erase(objectPosition);

    if (m_hueObjects.empty())
        stop();
    */
}

void HueSynchronizer::synchronize()
{
    qDebug() << m_hueObjects.size();
    for (auto hueObject : m_hueObjects) {
        hueObject->synchronize();
    }
}
