/*
** EPITECH PROJECT, 2018
** JFEntityComponentSystem
** File description:
** Exceptions.cpp
*/

/* Created the 24/03/2019 at 17:41 by jfrabel */

#include "Exceptions.hpp"

jf::ECSException::ECSException(const std::string &error, const std::string &where)
    : _what(error), _where(where)
{}

const char *jf::ECSException::what() const noexcept
{
    return _what.c_str();
}

const char *jf::ECSException::where() const noexcept
{
    return _where.c_str();
}

jf::BadHandlerException::BadHandlerException(const std::string &error, const std::string &where)
    : ECSException(error, where)
{}

jf::EntityAlreadyRegisteredException::EntityAlreadyRegisteredException(const std::string &error,
                                                                       const std::string &where)
    : ECSException(error, where)
{}

jf::SystemAlreadyExistingException::SystemAlreadyExistingException(const std::string &error, const std::string &where)
    : ECSException(error, where)
{}

jf::SystemLogicalException::SystemLogicalException(const std::string &error, const std::string &where)
    : ECSException(error, where)
{}

jf::SystemNotFoundException::SystemNotFoundException(const std::string &error, const std::string &where)
    : ECSException(error, where)
{}