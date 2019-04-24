/*
** EPITECH PROJECT, 2018
** JFEntityComponentSystem
** File description:
** Component.cpp
*/

/* Created the 24/03/2019 at 17:28 by jfrabel */

#include "Component.hpp"
#include "Events.hpp"
#include "EventManager.hpp"

jf::components::Component::Component(jf::entities::Entity &entity)
    : _entity(entity)
{
    EMIT_CREATE(Component);
}

jf::entities::EntityHandler jf::components::Component::getEntity()
{
    return jf::entities::EntityHandler(&_entity);
}

jf::components::Component::~Component()
{
    EMIT_DELETE(Component);
}
