/*
** EPITECH PROJECT, 2018
** JFEntityComponentSystem
** File description:
** Internal.hpp
*/

/* Created the 11/04/2019 at 21:43 by jfrabel */

#ifndef JFENTITYCOMPONENTSYSTEM_INTERNAL_HPP
#define JFENTITYCOMPONENTSYSTEM_INTERNAL_HPP

#include <typeindex>

/*!
 * @namespace jf
 * @brief The jfecs library namespace
 */
namespace jf {

    /*!
     * @namespace jf::internal
     * @brief Do not use this namespace (used internally)
     */
    namespace internal {

        /*!
         * @brief A function used internally to deduce and store types
         * @tparam T The type to get the index for
         * @return An std::type_index of the given type
         */
        template<typename T>
        std::type_index getTypeIndex()
        {
            return std::type_index(typeid(T));
        };
    }
}

#endif //JFENTITYCOMPONENTSYSTEM_INTERNAL_HPP
