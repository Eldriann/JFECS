/*
** EPITECH PROJECT, 2018
** JFEntityComponentSystem
** File description:
** EntityDestroyedEvent.hpp
*/

/* Created the 14/04/2019 at 12:11 by jfrabel */

#ifndef JFENTITYCOMPONENTSYSTEM_ENTITYDESTROYEDEVENT_HPP
#define JFENTITYCOMPONENTSYSTEM_ENTITYDESTROYEDEVENT_HPP

#include "Entity.hpp"

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

        /* I use a direct pointer to the entities being destroyed
        ** because an EntityHandler would set himself invalid on this event.
        ** DO NOT create an EntityHandler or store this Entity in the event callback
        ** if you register a custom listener for this event
        */
        /*!
         * @struct EntityDestroyedEvent
         * @brief An event emitted when an entity is being destroyed
         * DO NOT create an EntityHandler or store this pointer
         */
        struct EntityDestroyedEvent {
            jf::entities::Entity *entity; /*!< The entity being destroyed */
        };
    }
}

#else

namespace jf {

    namespace events {

        struct EntityDestroyedEvent;
    }
}

#endif //JFENTITYCOMPONENTSYSTEM_ENTITYDESTROYEDEVENT_HPP
