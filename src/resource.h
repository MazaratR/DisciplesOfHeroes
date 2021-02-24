#ifndef RESOURCE_H
#define RESOURCE_H

#include <cstdint>

#include "object_types.h"
#include "resource_types.h"

#include "interfaces/i_interactable_object.h"

#include "forward/i_hero_forward.h"

class Resource
        : public IInteractableObject
{
public:
    Resource();
    Resource(ResourceType _type, int _value = 0);

public:
    ResourceType resourceType() const;
    int value() const { return m_value; }

    void interact(IHeroShared& _hero, IInteractObjShared&) override;
    std::string title() const override;

private:
    ResourceType m_resourceType = ResourceType::Unknown;
    int m_value = 0;
};

#endif // RESOURCE_H
