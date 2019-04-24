/*
** EPITECH PROJECT, 2018
** JFEntityComponentSystem
** File description:
** Component.hpp
*/

/* Created the 24/03/2019 at 17:28 by jfrabel */

#ifndef JFENTITYCOMPONENTSYSTEM_COMPONENT_HPP
#define JFENTITYCOMPONENTSYSTEM_COMPONENT_HPP

#include "Entity.hpp"
#include "EntityHandler.hpp"

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
         * @class Component
         * @brief A class representing a base component
         *
         * This class represent a component used to store data.
         * When creating a new component you need to derive from this class.
         * The first parameter of the derived component ctors NEEDS to be a reference to the parent entity.
         * You NEED to call the parent ctor in the ctors of the derived component
         *
         * If you want your component to emit creation and deletion event of your specific type:
         * add in ctor EMIT_CREATE(type_of_your_component);
         * add in dtor EMIT_DELETE(type_of_your_component);
         * You need to include EventManager.hpp for that to work
         */
        class Component {
        public:
            /*!
             * @brief dtor
             */
            virtual ~Component();
            /*!
             * @brief ctor
             * @param entity The entity this component will sit on
             */
            explicit Component(entities::Entity &entity);
            /*!
             * @brief Method used to retreive the entity this component is sitting on
             * @return A EntityHandler to the entity this component is sitting on
             */
            entities::EntityHandler getEntity();

        private:
            entities::Entity &_entity; /*!< The entity this component is linked to */
        };
    }
}

#else

namespace jf {

    namespace components {

        class Component;
    }
}

#endif //JFENTITYCOMPONENTSYSTEM_COMPONENT_HPP
