/*
** EPITECH PROJECT, 2018
** Project
** File description:
** Entity.cpp
*/

/* Created the 24/03/2019 at 16:19 by jfrabel */

#include "Entity.hpp"
#include "EntityManager.hpp"
#include "Events.hpp"
#include "EventManager.hpp"

jf::entities::Entity::Entity(const internal::ID &id, const std::string &name)
    : _id(id), _name(name), _components(), _enabled(true), _shouldBeKeeped(false)
{
    events::EventManager::getInstance().emit<events::EntityCreatedEvent>({EntityHandler(this)});
}

jf::entities::Entity::~Entity()
{
    for (auto &i : _components) {
        delete i.second;
        i.second = nullptr;
    }
    events::EventManager::getInstance().emit<events::EntityDestroyedEvent>({this});
}

const jf::internal::ID &jf::entities::Entity::getID() const
{
    return _id;
}

const std::string &jf::entities::Entity::getName() const
{
    return _name;
}

void jf::entities::Entity::setEnable(bool enabled)
{
    _enabled = enabled;
}

bool jf::entities::Entity::isEnabled() const
{
    return _enabled;
}

void jf::entities::Entity::setShouldBeKeeped(bool keep)
{
    _shouldBeKeeped = keep;
}

bool jf::entities::Entity::shouldBeKeeped() const
{
    return _shouldBeKeeped;
}