/*
** EPITECH PROJECT, 2018
** JFEntityComponentSystem
** File description:
** EntityHandler.hpp
*/

/* Created the 14/04/2019 at 11:59 by jfrabel */

#ifndef JFENTITYCOMPONENTSYSTEM_ENTITYHANDLER_HPP
#define JFENTITYCOMPONENTSYSTEM_ENTITYHANDLER_HPP

#include "ID.hpp"
#include "Entity.hpp"
#include "EntityDestroyedEvent.hpp"
#include "EventManager.hpp"

/*!
 * @namespace jf
 * @brief The jfecs library namespace
 */
namespace jf {

    /*!
     * @namespace jf::entities
     * @brief Namespace for entity related classes
     */
    namespace entities {

        /*!
         * @class EntityHandler
         * @brief A class that handle entities
         *
         * A handler can be valid or invalid.
         * An invalid handler is a handler that manages no entity or a destroyed one.
         */
        class EntityHandler {
        public:

            /*!
             * @brief default ctor, this will create an invalid EntityHandler
             */
            EntityHandler(): _ptr(nullptr), _eventListenerID()
            {
                _eventListenerID = events::EventManager::getInstance().addListener<EntityHandler, events::EntityDestroyedEvent>(
                    this,
                    [](EntityHandler *data, events::EntityDestroyedEvent event) {
                        if (event.entity == data->get())
                            data->set(nullptr);
                    });
            }

            /*!
             * @brief ctor
             * @param entity The entity to handle
             */
            explicit EntityHandler(Entity *entity): _ptr(entity), _eventListenerID()
            {
                _eventListenerID = events::EventManager::getInstance().addListener<EntityHandler, events::EntityDestroyedEvent>(
                    this,
                    [](EntityHandler *data, events::EntityDestroyedEvent event) {
                        if (event.entity == data->get())
                            data->set(nullptr);
                    });
            }

            /*!
             * @brief cpy ctor
             * @param other The handler to copy
             */
            EntityHandler(const EntityHandler &other): _ptr(other._ptr), _eventListenerID()
            {
                _eventListenerID = events::EventManager::getInstance().addListener<EntityHandler, events::EntityDestroyedEvent>(
                    this,
                    [](EntityHandler *data, events::EntityDestroyedEvent event) {
                        if (event.entity == data->get())
                            data->set(nullptr);
                    });
            }

            /*!
             * @brief dtor
             */
            virtual ~EntityHandler()
            {
                events::EventManager::getInstance().removeListener(_eventListenerID);
            }

        public:
            /*!
             * @brief operator*
             * @return A reference on the managed entity
             * @throw jf::BadHandlerException if this handler is invalid (destroyed entity, uninitialized handler)
             */
            virtual Entity &operator*()
            {
                if (!isValid())
                    throw jf::BadHandlerException("Invalid EntityHandler", "EntityHandler");
                return *_ptr;
            }

            /*!
             * @brief operator* const
             * @return A const reference on the managed entity
             * @throw jf::BadHandlerException if this handler is invalid (destroyed entity, uninitialized handler)
             */
            virtual const Entity &operator*() const
            {
                if (!isValid())
                    throw jf::BadHandlerException("Invalid EntityHandler", "EntityHandler");
                return *_ptr;
            }

            /*!
             * @brief operator ->
             * @return A pointer to the managed entity
             * @throw jf::BadHandlerException if this handler is invalid (destroyed entity, uninitialized handler)
             */
            virtual Entity *operator->()
            {
                if (!isValid())
                    throw jf::BadHandlerException("Invalid EntityHandler", "EntityHandler");
                return _ptr;
            }

            /*!
             * @brief operator -> const
             * @return A pointer to the const managed entity
             * @throw jf::BadHandlerException if this handler is invalid (destroyed entity, uninitialized handler)
             */
            virtual const Entity *operator->() const
            {
                if (!isValid())
                    throw jf::BadHandlerException("Invalid EntityHandler", "EntityHandler");
                return _ptr;
            }

            /*!
             * @brief equality operator
             * @param rhs The EntityHandler to compare to
             * @return true if the two handler manage the same entity false otherwise (return true if both invalid)
             */
            virtual bool operator==(const EntityHandler &rhs) const
            {
                if (!isValid() && !rhs.isValid())
                    return true;
                if (!isValid() || !rhs.isValid())
                    return false;
                return _ptr == rhs._ptr;
            }

            EntityHandler &operator=(const EntityHandler &other)
            {
                if (&other == this)
                    return *this;
                _ptr = other._ptr;
                return *this;
            }

        public:
            /*!
             * @brief check if this handler is valid
             * @return true if the handler is valid false otherwise
             */
            virtual bool isValid() const
            {
                return _ptr != nullptr;
            }

            /*!
             * @brief An other way to get the managed entity without a throw
             * @return The managed pointer
             * DO NOT store an Entity directly always use an EntityHandler
             */
            virtual Entity *get()
            {
                return _ptr;
            }

            /*!
             * @brief Set the managed Entity
             * @param entity the entity to manage
             */
            virtual void set(Entity *entity)
            {
                _ptr = entity;
            }

        private:
            Entity *_ptr; /*!< The managed entity, will be nullptr if invalid */
            internal::ID _eventListenerID; /*!< The id of the event invalidating this handler on component destruction*/
        };
    }
}

#else

namespace jf {

    namespace entities {

        class EntityHandler;
    }
}

#endif //JFENTITYCOMPONENTSYSTEM_ENTITYHANDLER_HPP
