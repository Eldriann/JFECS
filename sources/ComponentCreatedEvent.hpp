/*
** EPITECH PROJECT, 2018
** JFEntityComponentSystem
** File description:
** ComponentCreatedEvent.hpp
*/

/* Created the 14/04/2019 at 12:13 by jfrabel */

#ifndef JFENTITYCOMPONENTSYSTEM_COMPONENTCREATEDEVENT_HPP
#define JFENTITYCOMPONENTSYSTEM_COMPONENTCREATEDEVENT_HPP

#include "ComponentHandler.hpp"

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
         * @struct ComponentCreatedEvent
         * @brief An event emitted when a component is created
         * @tparam T The type of the created component
         *
         * You can listen for jf::events::ComponentCreatedEvent<jf::component::Component>
         * It will be emitted for any type of component
         */
        template<typename T>
        struct ComponentCreatedEvent {
            components::ComponentHandler<T> component; /*!< A ComponentHandler with the newly created component */
        };
    }
}

#else

namespace jf {

    namespace events {

        template<typename T>
        struct ComponentCreatedEvent;
    }
}

#endif //JFENTITYCOMPONENTSYSTEM_COMPONENTCREATEDEVENT_HPP
