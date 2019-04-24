/*
** EPITECH PROJECT, 2018
** JFEntityComponentSystem
** File description:
** ECSWrapper.hpp
*/

/* Created the 21/04/2019 at 22:41 by jfrabel */

#ifndef JFENTITYCOMPONENTSYSTEM_ECSWRAPPER_HPP
#define JFENTITYCOMPONENTSYSTEM_ECSWRAPPER_HPP

#include "EventManager.hpp"
#include "EntityManager.hpp"
#include "SystemManager.hpp"

/*!
 * @struct ECSWrapper
 * @brief A simple struct that can be used as a wrapper for the ecs engine
 */
struct ECSWrapper {
    /*!
     * @brief ctor
     */
    ECSWrapper();
    jf::events::EventManager &eventManager; /*!< The EventManager singleton instance */
    jf::entities::EntityManager &entityManager; /*!< The EntityManager singleton instance */
    jf::systems::SystemManager &systemManager; /*!< The SystemManager singleton instance */
};

#endif //JFENTITYCOMPONENTSYSTEM_ECSWRAPPER_HPP
