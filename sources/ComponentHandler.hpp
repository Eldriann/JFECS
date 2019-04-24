/*
** EPITECH PROJECT, 2018
** JFEntityComponentSystem
** File description:
** ComponentHandler.hpp
*/

/* Created the 14/04/2019 at 12:02 by jfrabel */

#ifndef JFENTITYCOMPONENTSYSTEM_COMPONENTHANDLER_HPP
#define JFENTITYCOMPONENTSYSTEM_COMPONENTHANDLER_HPP

#include "ID.hpp"
#include "ComponentDestroyedEvent.hpp"
#include "EventManager.hpp"

/*!
 * @namespace jf
 * @brief The jfecs library namespace
 */
namespace jf {

    /*!
     * @namespace jf::components
     * @brief The namespace containing the components
     */
    namespace components {

        /*!
         * @class ComponentHandler
         * @brief A class used to safely handle components
         * @tparam T The type of handled component
         *
         * A handler can be valid or invalid.
         * An invalid handler is a handler that manages no component or a destroyed component.
         */
        template<typename T>
        class ComponentHandler {
        public:
            /*!
             * @brief default ctor, will create an invalid ComponentHandler
             */
            ComponentHandler(): _ptr(nullptr), _eventListenerID()
            {
                _eventListenerID = events::EventManager::getInstance().addListener<ComponentHandler<Component>, events::ComponentDestroyedEvent<Component>>(
                    reinterpret_cast<ComponentHandler<Component>*>(this),
                    [](ComponentHandler<Component> *data, events::ComponentDestroyedEvent<Component> event) {
                        if (event.component == data->get())
                            data->set(nullptr);
                    });
            }

            /*!
             * @brief ctor
             * @param component The handeled component
             */
            explicit ComponentHandler(T *component): _ptr(component), _eventListenerID()
            {
                _eventListenerID = events::EventManager::getInstance().addListener<ComponentHandler<Component>, events::ComponentDestroyedEvent<Component>>(
                    reinterpret_cast<ComponentHandler<Component>*>(this),
                    [](ComponentHandler<Component> *data, events::ComponentDestroyedEvent<Component> event) {
                        if (event.component == data->get())
                            data->set(nullptr);
                    });
            }

            /*!
             * @brief cpy ctor
             * @param other The ComponentHandler to copy
             */
            ComponentHandler(const ComponentHandler<T> &other): _ptr(other._ptr), _eventListenerID()
            {
                _eventListenerID = events::EventManager::getInstance().addListener<ComponentHandler<Component>, events::ComponentDestroyedEvent<Component>>(
                    reinterpret_cast<ComponentHandler<Component>*>(this),
                    [](ComponentHandler<Component> *data, events::ComponentDestroyedEvent<Component> event) {
                        if (event.component == data->get())
                            data->set(nullptr);
                    });
            }

            /*!
             * @brief dtor
             */
            virtual ~ComponentHandler()
            {
                events::EventManager::getInstance().removeListener(_eventListenerID);
            }

        public:
            /*!
             * @brief operator*
             * @return A reference on the managed component
             * @throw jf::BadHandlerException if this handler is invalid (destroyed component, uninitialized handler)
             */
            virtual T &operator*()
            {
                if (!isValid())
                    throw jf::BadHandlerException("Invalid ComponentHandler", "ComponentHandler");
                return *_ptr;
            }

            /*!
             * @brief operator * const
             * @return A const reference on the managed component
             * @throw jf::BadHandlerException if this handler is invalid (destroyed component, uninitialized handler)
             */
            virtual const T &operator*() const
            {
                if (!isValid())
                    throw jf::BadHandlerException("Invalid ComponentHandler", "ComponentHandler");
                return *_ptr;
            }

            /*!
             * @brief operator ->
             * @return A pointer to the managed component
             * @throw jf::BadHandlerException if this handler is invalid (destroyed component, uninitialized handler)
             */
            virtual T *operator->()
            {
                if (!isValid())
                    throw jf::BadHandlerException("Invalid ComponentHandler", "ComponentHandler");
                return _ptr;
            }

            /*!
             * @brief operator -> const
             * @return A const pointer to the managed component
             * @throw jf::BadHandlerException if this handler is invalid (destroyed component, uninitialized handler)
             */
            virtual const T *operator->() const
            {
                if (!isValid())
                    throw jf::BadHandlerException("Invalid ComponentHandler", "ComponentHandler");
                return _ptr;
            }

            /*!
             * @brief equality operator
             * @param rhs The ComponentHandler to compare to
             * @return true if the two handler manage the same component false otherwise (return true if both invalid)
             */
            virtual bool operator==(const ComponentHandler<T> &rhs) const
            {
                if (!isValid() && !rhs.isValid())
                    return true;
                if (!isValid() || !rhs.isValid())
                    return false;
                return _ptr == rhs._ptr;
            }

            ComponentHandler<T> &operator=(const ComponentHandler<T> &other)
            {
                if (&other == this)
                    return *this;
                _ptr = other._ptr;
                return *this;
            }

        public:
            /*!
             * @brief Method to know if the handler is valid or not
             * @return true if the handler is valid false otherwise
             */
            virtual bool isValid() const
            {
                return _ptr != nullptr;
            }

            /*!
             * @brief An alternative method to get the managed component without a throw
             * @return The managed pointer
             *
             * DO NOT store component manually. Always use a handler.
             */
            virtual T *get()
            {
                return _ptr;
            }

            /*!
             * @brief A method to set the managed component.
             */
            virtual void set(T *component)
            {
                _ptr = component;
            }

        private:
            T *_ptr; /*!< A pointer to the handeled component, will be null if handler is invalid */
            internal::ID _eventListenerID; /*!< The id of the event invalidating this handler on component destruction*/
        };
    }
}

#else

namespace jf {

    namespace components {

        template<typename T>
        class ComponentHandler;
    }
}

#endif //JFENTITYCOMPONENTSYSTEM_COMPONENTHANDLER_HPP
