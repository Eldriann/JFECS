/*
** EPITECH PROJECT, 2018
** JFEntityComponentSystem
** File description:
** ComponentDestroyedEvent.hpp
*/

/* Created the 14/04/2019 at 12:16 by jfrabel */

#ifndef JFENTITYCOMPONENTSYSTEM_COMPONENTDESTROYEDEVENT_HPP
#define JFENTITYCOMPONENTSYSTEM_COMPONENTDESTROYEDEVENT_HPP

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

        /*
        ** I use a direct pointer to the components being destroyed
        ** because a ComponentHandler would set himself invalid on this event.
        ** DO NOT create a ComponentHandler or store this Component in the event callback
        ** if you register a custom listener for this event
        */
        /*!
         * @struct ComponentDestroyedEvent
         * @brief An event emitted when a component is destroyed
         * @tparam T The type of destroyed component
         *
         * You can listen for jf::events::ComponentDestroyedEvent<jf::component::Component>
         * It will be emitted for any type of component
         */
        template<typename T>
        struct ComponentDestroyedEvent {
            T *component; /*!< The component being destroyed */
        };
    }
}

#else

namespace jf {

    namespace events {

        template<typename T>
        struct ComponentDestroyedEvent;
    }
}

#endif //JFENTITYCOMPONENTSYSTEM_COMPONENTDESTROYEDEVENT_HPP
