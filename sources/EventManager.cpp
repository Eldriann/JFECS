/*
** EPITECH PROJECT, 2018
** JFEntityComponentSystem
** File description:
** EventManager.cpp
*/

/* Created the 13/04/2019 at 19:02 by jfrabel */

#include "EventManager.hpp"

jf::events::EventManager::EventManager(): _freeIDs(), _maxID(0), _listeners()
{

}

jf::events::EventManager::~EventManager()
{
    for (auto &vec : _listeners) {
        for (auto &listener : vec.second) {
            delete listener;
        }
    }
}

jf::events::EventManager &jf::events::EventManager::getInstance()
{
    static EventManager instance;
    return instance;
}

uint64_t jf::events::EventManager::getNextID()
{
    if (_freeIDs.empty()) {
        _maxID++;
        return _maxID;
    } else {
        internal::ID id = _freeIDs.front();
        _freeIDs.pop();
        return id.getID();
    }
}

jf::events::BaseListener::BaseListener(const jf::internal::ID &id): _id(id)
{

}

bool jf::events::BaseListener::operator==(const jf::internal::ID &id)
{
    return _id == id;
}

const jf::internal::ID &jf::events::BaseListener::getID() const
{
    return _id;
}
