/*
** EPITECH PROJECT, 2018
** JFEntityComponentSystem
** File description:
** ID.cpp
*/

/* Created the 12/04/2019 at 11:14 by jfrabel */

#include "ID.hpp"

jf::internal::ID::ID()
    : _id(0)
{}

jf::internal::ID::ID(uint64_t id)
    : _id(id)
{}


jf::internal::ID::ID(const jf::internal::ID &other)
    : _id(other._id)
{}

bool jf::internal::ID::operator!=(const jf::internal::ID &rhs) const
{
    return _id != rhs._id;
}

bool jf::internal::ID::operator==(const jf::internal::ID &rhs) const
{
    return _id == rhs._id;
}

bool jf::internal::ID::operator<=(const jf::internal::ID &rhs) const
{
    return _id <= rhs._id;
}

bool jf::internal::ID::operator>=(const jf::internal::ID &rhs) const
{
    return _id >= rhs._id;
}

bool jf::internal::ID::operator<(const jf::internal::ID &rhs) const
{
    return _id < rhs._id;
}

bool jf::internal::ID::operator>(const jf::internal::ID &rhs) const
{
    return _id > rhs._id;
}

uint64_t jf::internal::ID::getID() const
{
    return _id;
}

bool jf::internal::ID::isValid() const
{
    return _id != invalidID;
}

jf::internal::ID &jf::internal::ID::operator=(const jf::internal::ID &rhs)
{
    if (this == &rhs)
        return *this;
    _id = rhs._id;
    return *this;
}
