/*
** EPITECH PROJECT, 2018
** JFEntityComponentSystem
** File description:
** System.hpp
*/

/* Created the 14/04/2019 at 21:40 by jfrabel */

#ifndef JFENTITYCOMPONENTSYSTEM_SYSTEM_HPP
#define JFENTITYCOMPONENTSYSTEM_SYSTEM_HPP

#include <chrono>

/*!
 * @namespace jf
 * @brief The jfecs library namespace
 */
namespace jf {

    /*!
     * @namespace jf::systems
     * @brief A namespace used for all system related classes
     */
    namespace systems {

        /*!
         * @class ISystem
         * @brief An interface that should be inplemented by all systems
         *
         * On the first system start onAwake will be called.
         * On a system start onStart will be called (on first start will be called 1 iteration after onAwake).
         * While a system is running onUpdate will be called each iteration.
         * As the engine can put system to sleep (stop) onStop will be called when a system will stop running.
         * Systems that have been stoped can be resumed, then onStart will be called again and onUpdate each iteration.
         * On engine stop onTearDown will be called on the system.
         *
         * Please remember that a stopped system can still receive events.
         * If you do not wish this behaviour it's up to you to implement a "state" for your systems
         * or to unregister the listeners.
         */
        class ISystem {
        public:
            /*!
             * @brief dtor
             */
            virtual ~ISystem() = default;
            /*!
             * @brief The first method to be called on systems first start
             */
            virtual void onAwake() = 0;
            /*!
             * @brief A method called on system start but after onAwake (will be called one iteration after)
             */
            virtual void onStart() = 0;
            /*!
             * @brief A method called each iteration, it's first call will be one iteration after onStart
             * @param elapsedTime The elapsed time since this function was last called
             */
            virtual void onUpdate(const std::chrono::nanoseconds &elapsedTime) = 0;
            /*!
             * @brief A method called when a system is stopped
             */
            virtual void onStop() = 0;
            /*!
             * @brief A method called on system tear down
             */
            virtual void onTearDown() = 0;
        };
    }
}

#endif //JFENTITYCOMPONENTSYSTEM_SYSTEM_HPP
