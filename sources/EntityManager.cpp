/*
** EPITECH PROJECT, 2018
** JFEntityComponentSystem
** File description:
** EntityManager.cpp
*/

/* Created the 12/04/2019 at 11:12 by jfrabel */

#include <algorithm>
#include "EntityManager.hpp"

jf::entities::EntityManager &jf::entities::EntityManager::getInstance()
{
    static EntityManager instance;
    return instance;
}

jf::entities::EntityManager::EntityManager()
    : _maxId(0), _entities(), _freeIDs(), _toDestroyIDs()
{

}

void jf::entities::EntityManager::registerNewEntity(jf::entities::Entity *entity)
{
    auto existing = _entities.find(entity->getID());
    if (existing != _entities.end()) {
        throw EntityAlreadyRegisteredException("Entity ID already existing", "registerNewEntity");
    }
    _entities.insert(std::make_pair(entity->getID(), entity));
}

bool jf::entities::EntityManager::unregisterEntity(const jf::internal::ID &entityID)
{
    auto existing = _entities.find(entityID);
    if (existing != _entities.end()) {
        _freeIDs.emplace(entityID);
        delete existing->second;
        _entities.erase(existing);
        return true;
    }
    return false;
}

jf::entities::EntityHandler jf::entities::EntityManager::createEntity(const std::string &name)
{
    Entity *entity;
    if (_freeIDs.empty()) {
        ++_maxId;
        entity = new Entity(internal::ID(_maxId), name);
    } else {
        internal::ID id = _freeIDs.front();
        _freeIDs.pop();
        entity = new Entity(internal::ID(id.getID()), name);
    }
    registerNewEntity(entity);
    return getEntityByID(entity->getID());
}

jf::entities::EntityHandler jf::entities::EntityManager::getEntityByID(const jf::internal::ID &entityID)
{
    auto entity = _entities.find(entityID);
    if (entity == _entities.end())
        return jf::entities::EntityHandler();
    return jf::entities::EntityHandler(entity->second);
}

bool jf::entities::EntityManager::deleteEntity(const jf::internal::ID &entityID)
{
    return unregisterEntity(entityID);
}

void jf::entities::EntityManager::applyToEach(std::function<void(EntityHandler)> func, bool onlyEnabled)
{
    for (auto &entity : _entities) {
        if (entity.second->isEnabled() || !onlyEnabled)
            func(EntityHandler(entity.second));
    }
}

jf::entities::EntityManager::~EntityManager()
{
    for (auto &entity : _entities) {
        delete entity.second;
    }
}

jf::entities::EntityHandler jf::entities::EntityManager::getEntityByName(
    const std::string &entityName, bool onlyEnabled)
{
    for (auto &entity : _entities) {
        if (entity.second != nullptr && entity.second->getName() == entityName
        && (entity.second->isEnabled() || !onlyEnabled))
            return jf::entities::EntityHandler(entity.second);
    }
    return jf::entities::EntityHandler();
}

std::vector<jf::entities::EntityHandler> jf::entities::EntityManager::getEntitiesByName(
    const std::string &entityName, bool onlyEnabled)
{
    std::vector<jf::entities::EntityHandler> matching;
    for (auto &entity : _entities) {
        if (entity.second != nullptr && entity.second->getName() == entityName
        && (entity.second->isEnabled() || !onlyEnabled))
            matching.emplace_back(entity.second);
    }
    return matching;
}

void jf::entities::EntityManager::deleteAllEntities()
{
    auto entity = std::find_if(_entities.begin(), _entities.end(), [](const std::pair<internal::ID, Entity *> &pair) {
        return !pair.second->shouldBeKeeped();
    });
    while (entity != _entities.end()) {
        deleteEntity(entity->first);
        entity = std::find_if(_entities.begin(), _entities.end(), [](const std::pair<internal::ID, Entity *> &pair) {
            return !pair.second->shouldBeKeeped();
        });
    }
}

void jf::entities::EntityManager::safeDeleteEntity(const jf::internal::ID &entityID)
{
    _toDestroyIDs.push_back(entityID);
}

void jf::entities::EntityManager::applySafeDelete()
{
    for (auto &id : _toDestroyIDs) {
        unregisterEntity(id);
    }
    _toDestroyIDs.clear();
}
