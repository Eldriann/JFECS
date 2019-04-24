/*
** EPITECH PROJECT, 2018
** JFEntityComponentSystem
** File description:
** ECSWrapper.cpp
*/

/* Created the 21/04/2019 at 22:41 by jfrabel */

#include "ECSWrapper.hpp"

ECSWrapper::ECSWrapper()
    : eventManager(jf::events::EventManager::getInstance()),
      entityManager(jf::entities::EntityManager::getInstance()),
      systemManager(jf::systems::SystemManager::getInstance())
{

}
