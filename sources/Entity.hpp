/*
** EPITECH PROJECT, 2018
** Project
** File description:
** Entity.hpp
*/

/* Created the 24/03/2019 at 16:19 by jfrabel */

#ifndef PROJECT_ENTITY_HPP
#define PROJECT_ENTITY_HPP

#include <unordered_map>
#include "Exceptions.hpp"
#include "Internal.hpp"
#include "ID.hpp"
#include "Component.hpp"
#include "ComponentHandler.hpp"

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
         * @class Entity
         * @brief The class describing an entity
         *
         * This class describe an entity.
         * An entity is something that hold components.
         * You should not handle directly Entities objects use EntityHandler instead
         */
        class Entity final {
        public:
            /*!
             * @brief Create an Entity.
             * DO NOT create entity manually use the EntityManager or the helper function createEntity()
             */
            Entity(const internal::ID &id, const std::string &name);

            /*!
             * @brief dtor
             */
            ~Entity();

            /*!
             * @brief Method used to get a component of this entity
             * @tparam C The type of requested component
             * @return A ComponentHandler
             *
             * The returned handler will be invalid if there is no component of the requested type on the entity
             */
            template<typename C>
            components::ComponentHandler<C> getComponent();

            /*!
             * @brief Method used to retrieve many components in a single call
             * @tparam C The type of requested component
             * @return A tuple of ComponentHandler
             *
             * The ComponentHandlers in the tuple
             * will be invalid if there is no component of the requested type on the entity
             */
            template<typename C>
            std::tuple<components::ComponentHandler<C>> getComponents();

            /*!
             * @brief Method used to retrieve many components in a single call
             * @tparam CA The first type of requested component
             * @tparam CB The second type of requested component
             * @tparam Others All the other types of requested components (can be empty)
             * @return A tuple of ComponentHandler
             *
             * The ComponentHandlers in the tuple
             * will be invalid if there is no component of the requested type on the entity
             */
            template<typename CA, typename CB, typename... Others>
            std::tuple<components::ComponentHandler<CA>, components::ComponentHandler<CB>, components::ComponentHandler<Others>...> getComponents();

            /*!
             * @brief Method used to know if an entity has a component of a specific type
             * @tparam C The type of required component
             * @return true if there is a component of this type on the entity false otherwise
             */
            template<typename C>
            bool hasComponent();

            /*!
             * @brief Method used to know if an entity has the totality of the requested components on it
             * @tparam C The type of required component
             * @return true if the entity possess all the required components false otherwise
             */
            template<typename C>
            bool hasComponents();

            /*!
             * @brief Method used to know if an entity has the totality of the requested components on it
             * @tparam CA The first type of required component
             * @tparam CB The second type of required component
             * @tparam Others The other types of required components (can be empty)
             * @return true if the entity possess all the required components false otherwise
             */
            template<typename CA, typename CB, typename... Others>
            bool hasComponents();

            /*!
             * @brief Assign a type of component to this entity
             * @tparam C The type of component to assign
             * @tparam Params The types of parameters given to the constructor of the component
             * @param params The parameters given to the constructor of the component
             * @return A ComponentHandler with the newly created component
             */
            template<typename C, typename ...Params>
            components::ComponentHandler<C> assignComponent(Params ...params);

            /*!
             * @brief Remove a component from this entity
             * @tparam C The type of component to remove
             * @return true if the component was present on the entity false otherwise
             */
            template<typename C>
            bool removeComponent();

            /*!
             * @brief Get the ID of this entity
             * @return The ID of this entity
             */
            const internal::ID &getID() const;
            /*!
             * @brief Get the name of this entity
             * @return The name of this entity
             */
            const std::string &getName() const;

            /*!
             * @brief Enable/dissable an entity
             * @param enabled The value to set
             */
            void setEnable(bool enabled);

            /*!
             * @brief Return if an entity is enabled
             * @return true if the entity is enabled false otherwise
             */
            bool isEnabled() const;

            /*!
             * @brief Set if the entity should be keeped
             * @param keep The value to set
             */
            void setShouldBeKeeped(bool keep);

            /*!
             * @brief Return if the entity is marked as to keep
             * @return true if should be keeped false otherwise
             */
            bool shouldBeKeeped() const;

        private:
            const internal::ID _id; /*!< An id identifing this object (will be unique) */
            const std::string _name; /*!< A name identifing this object (does not need to be unique) */
            std::unordered_map<std::type_index, components::Component *> _components; /*!< The components of this obj */
            bool _enabled; /*!< A boolean used to know if an entity is enabled or not */
            bool _shouldBeKeeped; /*!< A boolean used to know if an entity should be kept when destroyAll is called */
        };

        template<typename C>
        components::ComponentHandler<C> Entity::getComponent()
        {
            auto elem = _components.find(internal::getTypeIndex<C>());
            if (elem == _components.end())
                return components::ComponentHandler<C>();
            else
                return components::ComponentHandler<C>(reinterpret_cast<C *>(elem->second));
        }

        template<typename C>
        std::tuple<components::ComponentHandler<C>> Entity::getComponents()
        {
            return std::tuple<components::ComponentHandler<C>>(getComponent<C>());
        }

        template<typename CA, typename CB, typename... Others>
        std::tuple<components::ComponentHandler<CA>,
            components::ComponentHandler<CB>,
            components::ComponentHandler<Others>...> Entity::getComponents()
        {
            return std::tuple_cat(getComponents<CA>(), getComponents<CB, Others...>());
        }

        template<typename C>
        bool Entity::hasComponent()
        {
            return (_components.count(internal::getTypeIndex<C>()) != 0);
        }

        template<typename C>
        bool Entity::hasComponents()
        {
            return hasComponent<C>();
        }

        template<typename CA, typename CB, typename... Others>
        bool Entity::hasComponents()
        {
            return (hasComponent<CA>() && hasComponents<CB, Others...>());
        }

        template<typename C, typename... Params>
        components::ComponentHandler<C> Entity::assignComponent(Params... params)
        {
            C *newComp = new C(*this, params...);
            auto oldComp = _components.find(internal::getTypeIndex<C>());
            if (oldComp != _components.end()) {
                delete oldComp->second;
                oldComp->second = newComp;
            } else {
                _components.insert(std::make_pair(internal::getTypeIndex<C>(), newComp));
            }
            return components::ComponentHandler<C>(
                reinterpret_cast<C *>(_components.find(internal::getTypeIndex<C>())->second));
        }

        template<typename C>
        bool Entity::removeComponent()
        {
            auto comp = _components.find(internal::getTypeIndex<C>());
            if (comp != _components.end()) {
                delete comp->second;
                _components.erase(comp);
                return true;
            }
            return false;
        }
    }
}

#else

namespace jf {

    namespace entities {

        class Entity;
    }
}

#endif //PROJECT_ENTITY_HPP
