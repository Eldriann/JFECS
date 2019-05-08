cmake_minimum_required(VERSION 3.11)

project(JFEntityComponentSystem)

file(
        GLOB_RECURSE
        SOURCES_CPP
        sources/*.cpp
)
file(
        GLOB_RECURSE
        SOURCES_HPP
        sources/*.hpp
)

add_library(jfecs ${SOURCES_CPP} ${SOURCES_HPP})
target_include_directories(
        jfecs
        PUBLIC
        sources/*
)
add_library(jfecs.so SHARED ${SOURCES_CPP} ${SOURCES_HPP})
target_include_directories(
        jfecs
        PUBLIC
        sources/*
)

install(TARGETS jfecs jfecs.so DESTINATION lib)
install(FILES ${SOURCES_HPP} DESTINATION include)