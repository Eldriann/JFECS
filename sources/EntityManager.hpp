/*
** EPITECH PROJECT, 2018
** JFEntityComponentSystem
** File description:
** EntityManager.hpp
*/

/* Created the 12/04/2019 at 11:12 by jfrabel */

#ifndef JFENTITYCOMPONENTSYSTEM_ENTITYMANAGER_HPP
#define JFENTITYCOMPONENTSYSTEM_ENTITYMANAGER_HPP

#include <vector>
#include <unordered_map>
#include "ID.hpp"
#include "Entity.hpp"
#include "EntityHandler.hpp"

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
         * @class EntityManager
         * @brief A singleton managing the lifetime of entities
         */
        class EntityManager final {
        public:

            /*!
             * @brief cpy ctor
             * @param other The manager to copy
             */
            EntityManager(const EntityManager &other) = delete;

            /*!
             * @brief dtor
             */
            ~EntityManager();

            /*!
             * @brief Static method used to retrieve the singleton instance
             * @return A reference to the singleton instance
             */
            static EntityManager &getInstance();

        private:
            /*!
             * @brief default ctor
             */
            EntityManager();

        public:
            /*!
             * @brief Assignment operator
             * @param other The other manager to assign to
             * @return this instance
             */
            EntityManager &operator=(const EntityManager &other) = delete;

        public:
            /*!
             * @brief Create a new empty entity
             * @param name The name to give to this entity
             * @return An entity handler with the new entity
             */
            EntityHandler createEntity(const std::string &name);
            /*!
             * @brief Delete an entity given it's id
             * @param entityID The id of the entity to delete
             * @return true if the entity existed and was deleted false otherwise
             */
            bool deleteEntity(const internal::ID &entityID);
            /*!
             * @brief Delete an entity given it's id
             * @param entityID The id of the entity to delete
             * @return true if the entity existed and was deleted false otherwise
             */
            void safeDeleteEntity(const internal::ID &entityID);

            /*!
             * @brief Delete all the existing entities except the ones marked as shouldBeKeeped
             */
            void deleteAllEntities();

            /*!
             * @brief Delete entities that have been marked for safe delete.
             */
            void applySafeDelete();

            /*!
             * @brief Get an entity given an ID
             * @param entityID The id of the entity to get
             * @return An EntityHandler to the requested entity (will be invalid if entity not found)
             */
            EntityHandler getEntityByID(const internal::ID &entityID);
            /*!
             * @brief Get the first entity with the matching name
             * @param entityName The name of the entity to get
             * @param onlyEnabled Only get enabled entities
             * @return An EntityHandler to the requested entity (will be invalid if entity not found)
             */
            EntityHandler getEntityByName(const std::string &entityName, bool onlyEnabled = true);
            /*!
             * @brief Get all entities with a specific name
             * @param entityName The requested name
             * @param onlyEnabled Get only enabled entities
             * @return A vector of EntityHandle with the matching entities (empty vector if no entity is found)
             */
            std::vector<EntityHandler> getEntitiesByName(const std::string &entityName, bool onlyEnabled = true);

            /*!
             * @brief Get all entities having all the required components
             * @tparam C The first required component
             * @tparam Others The others required components (can be empty)
             * @param onlyEnabled Get only enabled entities
             * @return A vector of EntityHandle with the matching entities (empty vector if no entity is found)
             */
            template<typename C, typename... Others>
            std::vector<EntityHandler> getEntitiesWith(bool onlyEnabled = true);

            /*!
             * @brief Apply a function/functor/lambda... to all the existing entities
             * @param func The function to apply
             * @param onlyEnabled apply only to enabled entities
             */
            void applyToEach(std::function<void(EntityHandler)> func, bool onlyEnabled = true);

            /*!
             * @brief Apply a fuction/functor/lamda... to all entities with a specific set of components
             * @tparam C The first required component
             * @tparam Others The others required components (can be empty)
             * @param func The function to apply
             * @param onlyEnabled apply only to enabled entities
             */
            template<typename C, typename... Others>
            void applyToEach(typename std::common_type<std::function<void(EntityHandler, components::ComponentHandler<C>, components::ComponentHandler<Others>...)>>::type func, bool onlyEnabled = true);

        private:
            /*!
             * @brief Internal function used to register a new entity
             * @param entity The entity to register
             */
            void registerNewEntity(Entity *entity);
            /*!
             * @brief Unregister an entity by it's ID
             * @param entityID The ID of the entity to unregister
             * @return true if the entity existed false otherwise
             */
            bool unregisterEntity(const internal::ID &entityID);

        private:
            uint64_t _maxId; /*!< The last biggest existing entity */
            std::unordered_map<internal::ID, Entity *> _entities; /*!< A map containing all the existing entities */
            std::queue<internal::ID> _freeIDs; /*!< Ids destroyed and not used */
            std::vector<internal::ID> _toDestroyIDs; /*!< Ids of entities to destroy */
        };

        template<typename C, typename... Others>
        std::vector<entities::EntityHandler> entities::EntityManager::getEntitiesWith(bool onlyEnabled)
        {
            std::vector<entities::EntityHandler> matching;
            for (auto &i : _entities) {
                if (i.second->hasComponents<C, Others...>() && (i.second->isEnabled() || !onlyEnabled)) {
                    matching.emplace_back(i.second);
                }
            }
            return matching;
        }

        template<typename C, typename... Others>
        void entities::EntityManager::applyToEach(
            typename std::common_type<std::function<void(EntityHandler, components::ComponentHandler<C>, components::ComponentHandler<Others>...)>>::type func, bool onlyEnabled)
        {
            for (auto &entity : _entities) {
                if (entity.second->hasComponents<C, Others...>() && (entity.second->isEnabled() || !onlyEnabled)) {
                    func(EntityHandler(entity.second), entity.second->getComponent<C>(), entity.second->getComponent<Others>()...);
                }
            }
        }
    }
}

#else

namespace jf {

    namespace entities {

        class EntityManager;
    }
}

#endif //JFENTITYCOMPONENTSYSTEM_ENTITYMANAGER_HPP
