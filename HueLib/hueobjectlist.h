#ifndef HUEOBJECTLIST_H
#define HUEOBJECTLIST_H

#include <QString>
#include <memory>
#include <vector>

template <typename ObjectType>
class HueObjectList
{
public:
    typedef std::vector<std::shared_ptr<ObjectType>> ObjectList;

    class iterator {
    public:
        iterator(typename ObjectList::iterator iter);
        iterator operator++();
        bool operator!=(const iterator& rhs) const;
        const ObjectType& operator*() const;
    private:
        typename ObjectList::iterator m_iter;
    };

public:
    HueObjectList();
    HueObjectList(std::shared_ptr<ObjectList> hueObjectList);
    ~HueObjectList();
    HueObjectList(const HueObjectList& rhs);
    HueObjectList(HueObjectList&& rhs);
    HueObjectList& operator=(const HueObjectList& rhs);
    HueObjectList& operator=(HueObjectList&& rhs);

    iterator begin() const;
    iterator end() const;

    std::shared_ptr<ObjectType> find(int ID) const;
    std::shared_ptr<ObjectType> find(QString name) const;

    ObjectType* findRaw(int ID) const;
    ObjectType* findRaw(QString name) const;

private:
    std::shared_ptr<ObjectList> m_objectList;
};

template <typename ObjectType>
HueObjectList<ObjectType>::HueObjectList()
    : m_objectList(std::make_shared<ObjectList>())
{

}

template <typename ObjectType>
HueObjectList<ObjectType>::HueObjectList(std::shared_ptr<ObjectList> hueObjectList)
    : m_objectList(hueObjectList)
{

}

template <typename ObjectType>
HueObjectList<ObjectType>::~HueObjectList<ObjectType>()
{

}

template <typename ObjectType>
HueObjectList<ObjectType>::HueObjectList(const HueObjectList& rhs)
    : m_objectList(std::move(rhs.m_objectList))
{

}

template <typename ObjectType>
HueObjectList<ObjectType>::HueObjectList(HueObjectList&& rhs)
    : m_objectList(std::move(rhs.m_objectList))
{

}

template <typename ObjectType>
HueObjectList<ObjectType>& HueObjectList<ObjectType>::operator=(const HueObjectList& rhs)
{
    if (this == &rhs)
        return *this;

    m_objectList = std::move(rhs.m_objectList);
    return *this;
}

template <typename ObjectType>
HueObjectList<ObjectType>& HueObjectList<ObjectType>::operator=(HueObjectList&& rhs)
{
    if (this == &rhs)
        return *this;

    m_objectList = std::move(rhs.m_objectList);
    return *this;
}

template <typename ObjectType>
typename HueObjectList<ObjectType>::iterator HueObjectList<ObjectType>::begin() const
{
    return iterator(m_objectList.get()->begin());
}

template <typename ObjectType>
typename HueObjectList<ObjectType>::iterator HueObjectList<ObjectType>::end() const
{
    return iterator(m_objectList.get()->end());
}

template<typename HueObject>
typename std::shared_ptr<HueObject> HueObjectList<HueObject>::find(int ID) const
{
    std::shared_ptr<HueObject> retObject = std::make_shared<HueObject>();

    for (auto object : *m_objectList.get())
        if (object.get()->ID() == ID)
            retObject = object;

    return retObject;
}

template<typename HueObject>
typename std::shared_ptr<HueObject> HueObjectList<HueObject>::find(QString name) const
{
    std::shared_ptr<HueObject> retObject = std::make_shared<HueObject>();

    for (auto object : *m_objectList.get())
        if (object.get()->name().getName() == name)
            retObject = object;

    return retObject;
}

template<typename HueObject>
HueObject* HueObjectList<HueObject>::findRaw(int ID) const
{
    return this->find(ID).get();
}

template<typename HueObject>
HueObject* HueObjectList<HueObject>::findRaw(QString name) const
{
    return this->find(name).get();
}

// Iterator functions
template <typename ObjectType>
HueObjectList<ObjectType>::iterator::iterator(typename ObjectList::iterator iter)
    : m_iter(iter)
{

}

template <typename ObjectType>
typename HueObjectList<ObjectType>::iterator HueObjectList<ObjectType>::iterator::operator++()
{
    m_iter = std::next(m_iter);
    return *this;
}

template <typename ObjectType>
bool HueObjectList<ObjectType>::iterator::operator!=(const HueObjectList<ObjectType>::iterator& rhs) const
{
    return m_iter != rhs.m_iter;
}

template <typename ObjectType>
const ObjectType& HueObjectList<ObjectType>::iterator::operator*() const
{
    return *(m_iter->get());
}

class HueLight;
typedef HueObjectList<HueLight> HueLightList;

class HueGroup;
typedef HueObjectList<HueGroup> HueGroupList;


#endif // HUEOBJECTLIST_H
