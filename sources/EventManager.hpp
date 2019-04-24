/*
** EPITECH PROJECT, 2018
** JFEntityComponentSystem
** File description:
** EventManager.hpp
*/

/* Created the 13/04/2019 at 19:02 by jfrabel */

#ifndef JFENTITYCOMPONENTSYSTEM_EVENTMANAGER_HPP
#define JFENTITYCOMPONENTSYSTEM_EVENTMANAGER_HPP

#include <queue>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "Internal.hpp"
#include "ID.hpp"

#define EMIT_CREATE(T) (jf::events::EventManager::getInstance().emit<jf::events::ComponentCreatedEvent<T>>({jf::components::ComponentHandler<T>(this)}))
#define EMIT_DELETE(T) (jf::events::EventManager::getInstance().emit<jf::events::ComponentDestroyedEvent<T>>({this}))

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
         * @class BaseListener
         * @brief A class used internaly to store EventListeners
         */
        class BaseListener {
        public:
            /*!
             * @brief ctor
             * @param id The ID to set to this Listener
             */
            explicit BaseListener(const internal::ID &id);
            /*!
             * @brief dtor
             */
            virtual ~BaseListener() = default;

            /*!
             * @brief equality operator
             * @param id The id to compare to
             * @return true if the id match false otherwise
             */
            bool operator==(const internal::ID &id);

            /*!
             * @brief Get the id of this listener
             * @return The id of this listener
             */
            const internal::ID &getID() const;
        protected:
            internal::ID _id; /*!< The id of the listener */
        };

        /*!
         * @class EventListener
         * @brief A class used internaly to manage events and callbacks
         * @tparam RegistererType The type of the register
         * @tparam EventType The type of the event to register to
         */
        template<typename RegistererType, typename EventType>
        class EventListener final : public BaseListener {
        public:
            /*!
             * @brief ctor
             * @param id The id to set this listener to
             * @param registerer The registerer of this listener
             * @param callback The callback to send when the EnventType event is emitted
             */
            EventListener(const internal::ID &id, RegistererType *registerer,
                typename std::common_type<std::function<void(RegistererType *, EventType)>>::type callback)
                : BaseListener(id), _registerer(registerer), _callback(callback)
            {}

            /*!
             * @brief ctor
             * @param other The listener to cpy
             */
            EventListener(const EventListener<RegistererType, EventType> &other)
            : BaseListener(other._id), _registerer(other._registerer), _callback(other._callback)
            {}

            /*!
             * @brief dtor
             */
            ~EventListener() override = default;

            /*!
             * @brief Method used to receive an event and trigger a callback
             * @param event The received event
             */
            void receive(const EventType &event)
            {
                _callback(_registerer, event);
            }

            /*!
             * @brief equality operator
             * @param listener The listener to compare to
             * @return true if the ids are the same false otherwise
             */
            bool operator==(const EventListener<RegistererType, EventType> &listener)
            {
                return _id == listener._id;
            }

        private:
            RegistererType *_registerer; /*!< The registerer of this event */
            std::function<void(RegistererType *, EventType)> _callback; /*!< The function callback */
        };

        /*!
         * @class EventManager
         * @brief A singleton class used to manage events
         */
        class EventManager {
        public:
            /*!
             * @brief ctor
             * @param other The other EventManager to create from
             */
            EventManager(const EventManager &other) = delete;
            /*!
             * @brief dtor
             */
            ~EventManager();

            /*!
             * @brief A static method used to retrieve the EventManager instance
             * @return A reference to the instance
             */
            static EventManager &getInstance();
        private:
            /*!
             * @brief ctor
             */
            EventManager();

        public:
            /*!
             * @brief Assignment operator
             * @param other The EventManager to assign to
             * @return The current instance
             */
            EventManager &operator=(const EventManager &other) = delete;

        public:
            /*!
             * @brief Get the next id to use for the next listener
             * @return The next id to use
             */
            uint64_t getNextID();

            /*!
             * @brief Add a new listener that will trigger a callback with the event and the registerer on a given event
             * @tparam RegistererType The type of the registerer
             * @tparam EventType The type of the event to register to
             * @param registerer A pointer to the registerer
             * @param callback The callback to trigger
             * @return The id of the created listener
             */
            template<typename RegistererType, typename EventType>
            internal::ID addListener(RegistererType *registerer,
                typename std::common_type<std::function<void(RegistererType *, EventType)>>::type callback)
            {
                internal::ID id(getNextID());
                auto *listener = new EventListener<RegistererType, EventType>(id, registerer, callback);
                auto existing = _listeners.find(internal::getTypeIndex<EventType>());
                if (existing == _listeners.end()) {
                    std::vector<BaseListener *> newVec;
                    newVec.push_back(listener);
                    _listeners.insert(std::make_pair(internal::getTypeIndex<EventType>(), newVec));
                } else {
                    existing->second.push_back(listener);
                }
                return id;
            }

            /*!
             * @brief Remove a listener by it's ID
             * @param id The id of the listener to remove
             */
            void removeListener(const internal::ID &id)
            {
                for (auto &vec : _listeners) {
                    for (auto it = vec.second.begin(); it != vec.second.end(); it++) {
                        if ((*it)->getID() == id) {
                            delete (*it);
                            vec.second.erase(it);
                            _freeIDs.emplace(id);
                            return;
                        }
                    }
                }
            }

            /*!
             * @brief Emit a given event and trigger all the listeners that register to this event
             * @tparam EventType The type of the event to emit
             * @param event The event to emit
             */
            template<typename EventType>
            void emit(const EventType &event)
            {
                auto vec = _listeners.find(internal::getTypeIndex<EventType>());
                if (vec != _listeners.end()) {
                    for (auto &listener : vec->second) {
                        reinterpret_cast<EventListener<void, EventType>*>(listener)->receive(event);
                    }
                }
            }

        private:
            std::queue<internal::ID> _freeIDs; /*!< free ids */
            uint64_t _maxID; /*!< The last maximum id given */
            std::unordered_map<std::type_index, std::vector<BaseListener *>> _listeners; /*!< The existing listeners */
        };
    }
}

#else

namespace jf {

    namespace events {

        class BaseListener;

        template<typename RegistererType, typename EventType>
        class EventListener;

        class EventManager;
    }
}

#endif //JFENTITYCOMPONENTSYSTEM_EVENTMANAGER_HPP
