#include "huesynchronizer.h"

#include "hueabstractobject.h"
#include "huelight.h"
#include "huegroup.h"

#include <QtDebug>

HueSynchronizer::HueSynchronizer()
    : m_hueObjects()
    , m_timer(new QTimer(this))
    , m_isActive(false)
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

void HueSynchronizer::setSyncInterval(int intervalMilliSec)
{
    instance().m_timer->setInterval(intervalMilliSec);
}

int HueSynchronizer::clearAll()
{
    return instance().clear(HueSynchronizer::ClearAll);
}

int HueSynchronizer::clearGroups()
{
    return instance().clear(HueSynchronizer::ClearGroups);
}

int HueSynchronizer::clearLights()
{
    return instance().clear(HueSynchronizer::ClearLights);
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

int HueSynchronizer::listSize()
{
    return static_cast<int>(m_hueObjects.size());
}

bool HueSynchronizer::isActive()
{
    return m_isActive;
}

bool HueSynchronizer::addHueObject(std::shared_ptr<HueAbstractObject> object)
{
    bool objectWasAdded = false;
    auto objectPosition = std::find(m_hueObjects.begin(), m_hueObjects.end(), object);

    if (objectPosition == m_hueObjects.end()) {
        m_hueObjects.push_back(object);
        objectWasAdded = true;
    }

    if (!isActive())
        start();

    return objectWasAdded;
}

bool HueSynchronizer::removeHueObject(std::shared_ptr<HueAbstractObject> object)
{
    bool objectWasRemoved = false;
    auto objectPosition = std::find(m_hueObjects.begin(), m_hueObjects.end(), object);

    if (objectPosition != m_hueObjects.end()) {
        m_hueObjects.erase(objectPosition);
        objectWasRemoved = true;
    }

    if (m_hueObjects.empty())
        stop();

    return objectWasRemoved;
}

void HueSynchronizer::synchronize()
{
    for (auto hueObject : m_hueObjects) {
        hueObject->synchronize();
    }
}


int HueSynchronizer::clear(HueSynchronizer::ClearCondition condition)
{
    int objectsRemoved = 0;

    for (auto object : m_hueObjects) {
        bool conditionIsMatched = false;

        switch (condition) {
        case ClearAll:
            conditionIsMatched = true;
            break;

        case ClearGroups:
        {
            HueGroup* groupPointer = dynamic_cast<HueGroup*>(object.get());
            conditionIsMatched = groupPointer != nullptr;
            break;
        }
        case ClearLights:
        {
            HueLight* lightPointer = dynamic_cast<HueLight*>(object.get());
            conditionIsMatched = lightPointer != nullptr;
            break;
        }
        }

        if (conditionIsMatched)
            if (removeHueObject(object))
                objectsRemoved++;
    }

    return objectsRemoved;
}
