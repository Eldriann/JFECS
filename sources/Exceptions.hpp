/*
** EPITECH PROJECT, 2018
** JFEntityComponentSystem
** File description:
** Exceptions.hpp
*/

/* Created the 24/03/2019 at 17:41 by jfrabel */

#ifndef JFENTITYCOMPONENTSYSTEM_EXCEPTIONS_HPP
#define JFENTITYCOMPONENTSYSTEM_EXCEPTIONS_HPP

#include <exception>
#include <string>

/*!
 * @namespace jf
 * @brief The jfecs library namespace
 */
namespace jf {

    /*!
     * @class jf::ECSException
     * @brief Base class for all ecs exceptions
     */
    class ECSException : public std::exception {
    public:
        /*!
         * @brief Create a new ECSException
         * @param error The error message
         * @param where Where does this error happened
         */
        ECSException(const std::string &error, const std::string &where);
        /*!
         * @brief Get informations on what happened
         * @return A char array containing the error message
         */
        const char *what() const noexcept override;
        /*!
         * @brief Get informations on where it happened
         * @return A char array containing the error location
         */
        const char *where() const noexcept;
    private:
        std::string _what; /*!< The internal error message */
        std::string _where; /*!< The internal location error message */
    };

    /*!
     * @class jf::BadHandlerException
     * @brief A class representing an invalid usage of a handler
     */
    class BadHandlerException : public ECSException {
    public:
        /*!
         * ctor
         * @param error The error message
         * @param where The error location
         */
        BadHandlerException(const std::string &error, const std::string &where);
    };

    /*!
     * @class jf::EntityAlreadyRegisteredException
     * @brief A class representing an error on the entity registration
     */
    class EntityAlreadyRegisteredException : public ECSException {
    public:
        /*!
         * ctor
         * @param error The error message
         * @param where The error location
         */
        EntityAlreadyRegisteredException(const std::string &error, const std::string &where);
    };

    /*!
     * @class SystemAlreadyExistingException
     * @brief An exception throwed when trying to add an already existing system
     */
    class SystemAlreadyExistingException : public ECSException {
    public:
        /*!
         * ctor
         * @param error The error message
         * @param where The error location
         */
        SystemAlreadyExistingException(const std::string &error, const std::string &where);
    };

    /*!
     * @class SystemLogicalException
     * @brief An exception throwed when a logical error is encountered (stoping an awaking system...)
     */
    class SystemLogicalException : public ECSException {
    public:
        /*!
         * ctor
         * @param error The error message
         * @param where The error location
         */
        SystemLogicalException(const std::string &error, const std::string &where);
    };

    /*!
     * @class SystemNotFoundException
     * @brief An exception throwed when a system is required but it is not found
     */
    class SystemNotFoundException : public ECSException {
    public:
        /*!
         * ctor
         * @param error The error message
         * @param where The error location
         */
        SystemNotFoundException(const std::string &error, const std::string &where);
    };
}

#else

namespace jf {

    class ECSException;

    class BadHandlerException;

    class EntityAlreadyRegisteredException;

    class SystemAlreadyExistingException;
    class SystemLogicalException;
    class SystemNotFoundException;
}

#endif //JFENTITYCOMPONENTSYSTEM_EXCEPTIONS_HPP
