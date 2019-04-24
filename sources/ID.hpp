/*
** EPITECH PROJECT, 2018
** JFEntityComponentSystem
** File description:
** ID.hpp
*/

/* Created the 12/04/2019 at 11:14 by jfrabel */

#ifndef JFENTITYCOMPONENTSYSTEM_ID_HPP
#define JFENTITYCOMPONENTSYSTEM_ID_HPP

#include <functional>
#include <cstdint>

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
         * @class ID
         * @brief An entity ID
         */
        class ID final {
        public:
            const uint64_t invalidID = 0; /*!< Something representing an invalid ID */
        public:
            /*!
             * @brief ctor
             */
            ID();
            /*!
             * @brief ctor
             * @param id The id to set
             */
            explicit ID(uint64_t id);
            /*!
             * @brief cpy ctor
             * @param other The ID to copy
             */
            ID(const ID &other);

        public:
            /*!
             * @brief Assignment operator
             * @param rhs The ID to assign as
             * @return This object
             */
            ID &operator=(const ID &rhs);
            /*!
             * @brief inequality operator
             * @param rhs The object to compare to
             * @return false if the ids are the same true otherwise
             */
            bool operator!=(const ID &rhs) const;
            /*!
             * @brief equality operator
             * @param rhs The object to compare to
             * @return true if the ids are the same false otherwise
             */
            bool operator==(const ID &rhs) const;
            /*!
             * @brief let operator
             * @param rhs The object to compare to
             * @return true if this id is less or equal to the other id
             */
            bool operator<=(const ID &rhs) const;
            /*!
             * @brief get operator
             * @param rhs The object to compare to
             * @return true if this id is greater or equal to the other id
             */
            bool operator>=(const ID &rhs) const;
            /*!
             * @brief lt operator
             * @param rhs The object to compare to
             * @return true if this id is less than the other id
             */
            bool operator<(const ID &rhs) const;
            /*!
             * @brief gt operator
             * @param rhs The object to compare to
             * @return true if this id is greater than the other id
             */
            bool operator>(const ID &rhs) const;

        public:
            /*!
             * @brief Get this id value
             * @return The value corresponding to this id
             */
            uint64_t getID() const;

            /*!
             * @brief Return if this id is valid
             * @return true if the id is valid false otherwise
             */
            bool isValid() const;

        private:
            uint64_t _id; /*!< The internal value of this id */
        };
    }
}

/* Overload of the hash object to be able to used ID as keys in standart containers */
namespace std {
    template <>
    class hash<jf::internal::ID> {
    public:
        size_t operator()(const jf::internal::ID &id) const
        {
            return hash<uint64_t>()(id.getID());
        }
    };
};

#else

namespace jf {

    namespace internal {

        class ID;
    }
}

#endif //JFENTITYCOMPONENTSYSTEM_ID_HPP
