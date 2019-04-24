/*
** EPITECH PROJECT, 2018
** JFEntityComponentSystem
** File description:
** EntityCreatedEvent.hpp
*/

/* Created the 14/04/2019 at 12:10 by jfrabel */

#ifndef JFENTITYCOMPONENTSYSTEM_ENTITYCREATEDEVENT_HPP
#define JFENTITYCOMPONENTSYSTEM_ENTITYCREATEDEVENT_HPP

#include "EntityHandler.hpp"

/*!
 * @namespace jf
 * @brief The jfecs library namespace
 */
namespace jf {

    /*!
     * @namespace jf::events
     * @brief A namespace handling event related stuff
     */
    namespace events {

        /*!
         * @struct EntityCreatedEvent
         * @brief An event emitted when a new Entity is created
         */
        struct EntityCreatedEvent {
            jf::entities::EntityHandler entity; /*!< The newly created entity */
        };
    }
}

#else

namespace jf {

    namespace events {

        struct EntityCreatedEvent;
    }
}

#endif //JFENTITYCOMPONENTSYSTEM_ENTITYCREATEDEVENT_HPP
