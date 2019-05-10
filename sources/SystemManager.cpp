/*
** EPITECH PROJECT, 2018
** JFEntityComponentSystem
** File description:
** SystemManager.cpp
*/

/* Created the 20/04/2019 at 21:54 by jfrabel */

#include <vector>
#include <iostream>
#include "SystemManager.hpp"

jf::systems::SystemManager &jf::systems::SystemManager::getInstance()
{
    static SystemManager instance;
    return instance;
}

jf::systems::SystemManager::SystemManager()
    : _systems(), _timeScale(1), _last(std::chrono::steady_clock::now())
{

}

jf::systems::SystemManager::~SystemManager()
{
    for (auto &system : _systems) {
        switch (system.second.first) {
        case NOT_STARTED:
            delete system.second.second;
            break;
        case AWAKING:
            system.second.second->onAwake();
#ifdef __unix__
            __attribute__((fallthrough));
#endif
        case STARTING:
            system.second.second->onStart();
#ifdef __unix__
            __attribute__((fallthrough));
#endif
        case RUNNING:
        case STOPPING:
            system.second.second->onStop();
#ifdef __unix__
            __attribute__((fallthrough));
#endif
        case STOPPED:
        case TEARING_DOWN:
            system.second.second->onTearDown();
            delete system.second.second;
            break;
        }
    }
}

void jf::systems::SystemManager::tick()
{
    auto now = std::chrono::steady_clock::now();
    std::chrono::nanoseconds elapsedTime = std::chrono::duration_cast<std::chrono::nanoseconds>((now - _last) * _timeScale);
    std::vector<std::type_index> toErase;
    for (auto &system : _systems) {
        switch (system.second.first) {
        case NOT_STARTED:
            break;
        case AWAKING:
            try {
                system.second.second->onAwake();
            } catch (std::exception &e) {
                addError(*system.second.second, e.what(), ErrorReport::ERROR_TYPE_ON_AWAKE);
                system.second.first = NOT_STARTED;
                continue;
            }
            system.second.first = STARTING;
            break;
        case STARTING:
            try {
                system.second.second->onStart();
            } catch (std::exception &e) {
                addError(*system.second.second, e.what(), ErrorReport::ERROR_TYPE_ON_START);
                system.second.first = STOPPED;
                continue;
            }
            system.second.first = RUNNING;
            break;
        case RUNNING:
            try {
                system.second.second->onUpdate(elapsedTime);
            } catch (std::exception &e) {
                addError(*system.second.second, e.what(), ErrorReport::ERROR_TYPE_ON_UPDATE);
            }
            break;
        case STOPPING:
            try {
                system.second.second->onStop();
            } catch (std::exception &e) {
                addError(*system.second.second, e.what(), ErrorReport::ERROR_TYPE_ON_STOP);
            }
            system.second.first = STOPPED;
            break;
        case STOPPED:
            break;
        case TEARING_DOWN:
            try {
                system.second.second->onTearDown();
            } catch (std::exception &e) {
                addError(*system.second.second, e.what(), ErrorReport::ERROR_TYPE_ON_TEARDOWN);
            }
            delete system.second.second;
            toErase.push_back(system.first);
            break;
        }
    }
    for (auto &typeindex : toErase)
        _systems.erase(typeindex);
    _last = now;
}

float jf::systems::SystemManager::getTimeScale() const
{
    return _timeScale;
}

void jf::systems::SystemManager::setTimeScale(float scale)
{
    _timeScale = scale;
}

std::vector<jf::systems::SystemManager::ErrorReport> jf::systems::SystemManager::getErrors()
{
    std::vector<ErrorReport> toReturn = _errors;
    _errors.clear();
    return toReturn;
}

void jf::systems::SystemManager::addError(jf::systems::ISystem &sys, const std::string &msg,
                                          jf::systems::SystemManager::ErrorReport::ErrorType type)
{
    ErrorReport newErr = {sys, msg, type};
    _errors.push_back(newErr);
}
