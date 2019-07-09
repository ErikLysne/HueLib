#ifndef HUEOBJECTLIST_H
#define HUEOBJECTLIST_H

#include <QString>
#include <memory>
#include <vector>

template <typename HueObject>
class HueObjectList
{
public:
    typedef std::vector<std::shared_ptr<HueObject>> ObjectList;

    class iterator {
    public:
        iterator(typename ObjectList::iterator iter);
        iterator operator++();
        bool operator!=(const iterator& rhs) const;
        const HueObject& operator*() const;
    private:
        typename ObjectList::iterator m_iter;
    };

public:
    HueObjectList();
    HueObjectList(std::shared_ptr<ObjectList> hueObjectList);
    ~HueObjectList();
    HueObjectList(const HueObjectList& rhs);
    HueObjectList(HueObjectList&& rhs);
    const HueObjectList& operator=(const HueObjectList& rhs);
    const HueObjectList& operator=(HueObjectList&& rhs);

    int size() const;

    iterator begin() const;
    iterator end() const;

    std::shared_ptr<HueObject> at(int index) const;
    std::shared_ptr<HueObject> fetch(int ID) const;
    std::shared_ptr<HueObject> fetch(QString name) const;
    HueObject* fetchRaw(int ID) const;
    HueObject* fetchRaw(QString name) const;

private:
    std::shared_ptr<ObjectList> m_objectList;
};

template <typename HueObject>
HueObjectList<HueObject>::HueObjectList()
    : m_objectList(std::make_shared<ObjectList>())
{

}

template <typename HueObject>
HueObjectList<HueObject>::HueObjectList(std::shared_ptr<ObjectList> hueObjectList)
    : m_objectList(hueObjectList)
{

}

template <typename HueObject>
HueObjectList<HueObject>::~HueObjectList<HueObject>()
{

}

template <typename HueObject>
HueObjectList<HueObject>::HueObjectList(const HueObjectList& rhs)
    : m_objectList(std::move(rhs.m_objectList))
{

}

template <typename HueObject>
HueObjectList<HueObject>::HueObjectList(HueObjectList&& rhs)
    : m_objectList(std::move(rhs.m_objectList))
{

}

template <typename HueObject>
const HueObjectList<HueObject>& HueObjectList<HueObject>::operator=(const HueObjectList& rhs)
{
    if (this == &rhs)
        return *this;

    m_objectList = std::move(rhs.m_objectList);
    return *this;
}

template <typename HueObject>
const HueObjectList<HueObject>& HueObjectList<HueObject>::operator=(HueObjectList&& rhs)
{
    if (this == &rhs)
        return *this;

    m_objectList = std::move(rhs.m_objectList);
    return *this;
}

template  <typename HueObject>
int HueObjectList<HueObject>::size() const
{
    return m_objectList.get()->size();
}

template <typename HueObject>
typename HueObjectList<HueObject>::iterator HueObjectList<HueObject>::begin() const
{
    return iterator(m_objectList.get()->begin());
}

template <typename HueObject>
typename HueObjectList<HueObject>::iterator HueObjectList<HueObject>::end() const
{
    return iterator(m_objectList.get()->end());
}

template <typename HueObject>
std::shared_ptr<HueObject> HueObjectList<HueObject>::at(int index) const
{
    if (index > size())
        return std::make_shared<HueObject>();

    return m_objectList.get()->at(index);
}

template <typename HueObject>
typename std::shared_ptr<HueObject> HueObjectList<HueObject>::fetch(int ID) const
{
    std::shared_ptr<HueObject> retObject = std::make_shared<HueObject>();

    for (auto object : *m_objectList.get())
        if (object.get()->ID() == ID)
            retObject = object;

    return retObject;
}

template <typename HueObject>
typename std::shared_ptr<HueObject> HueObjectList<HueObject>::fetch(QString name) const
{
    std::shared_ptr<HueObject> retObject = std::make_shared<HueObject>();

    for (auto object : *m_objectList.get())
        if (object.get()->name().getName() == name)
            retObject = object;

    return retObject;
}

template <typename HueObject>
HueObject* HueObjectList<HueObject>::fetchRaw(int ID) const
{
    return this->fetch(ID).get();
}

template <typename HueObject>
HueObject* HueObjectList<HueObject>::fetchRaw(QString name) const
{
    return this->fetch(name).get();
}

// Iterator functions
template <typename HueObject>
HueObjectList<HueObject>::iterator::iterator(typename ObjectList::iterator iter)
    : m_iter(iter)
{

}

template <typename HueObject>
typename HueObjectList<HueObject>::iterator HueObjectList<HueObject>::iterator::operator++()
{
    m_iter = std::next(m_iter);
    return *this;
}

template <typename HueObject>
bool HueObjectList<HueObject>::iterator::operator!=(const HueObjectList<HueObject>::iterator& rhs) const
{
    return m_iter != rhs.m_iter;
}

template <typename HueObject>
const HueObject& HueObjectList<HueObject>::iterator::operator*() const
{
    return *(m_iter->get());
}

class HueLight;
typedef HueObjectList<HueLight> HueLightList;

class HueGroup;
typedef HueObjectList<HueGroup> HueGroupList;


#endif // HUEOBJECTLIST_H
