/*
** EPITECH PROJECT, 2018
** JFEntityComponentSystem
** File description:
** SystemManager.hpp
*/

/* Created the 20/04/2019 at 21:54 by jfrabel */

#ifndef JFENTITYCOMPONENTSYSTEM_SYSTEMMANAGER_HPP
#define JFENTITYCOMPONENTSYSTEM_SYSTEMMANAGER_HPP

#include <unordered_map>
#include "System.hpp"
#include "Internal.hpp"
#include "Exceptions.hpp"

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
         * @enum SystemState
         * @brief An enum describing a system state
         */
        enum SystemState {
            NOT_STARTED, /*!< The system is not started and will awake if start is called */
            AWAKING, /*!< The system will be awaken in next tick */
            STARTING, /*!< The system will be started in next tick */
            RUNNING, /*!< The system is running and will be updated in next tick */
            STOPPING, /*!< The system will be stopped in next tick */
            STOPPED, /*!< The system is stopped and nothing will be done in next tick */
            TEARING_DOWN /*!< The system is tearing down, you should not use it anymore (used on engine destruction) */
        };

        /*!
         * @class SystemManager
         * @brief A singleton class used to manage systems
         */
        class SystemManager {
        public:
            struct ErrorReport {
                enum ErrorType {
                    ERROR_TYPE_ON_AWAKE,
                    ERROR_TYPE_ON_START,
                    ERROR_TYPE_ON_UPDATE,
                    ERROR_TYPE_ON_STOP,
                    ERROR_TYPE_ON_TEARDOWN,
                };
                ISystem &system;
                std::string error;
                ErrorType type;
            };

        public:
            /*!
             * @brief cpy ctor
             * @param other The other object to create from
             */
            SystemManager(const SystemManager &other) = delete;
            /*!
             * @brief dtor
             */
            ~SystemManager();

            /*!
             * @brief assignment operator
             * @param rhs The object to assign to
             * @return this object
             */
            SystemManager &operator=(const SystemManager &rhs) = delete;

            /*!
             * @brief Static function used to retrieve the instance of this singleton class
             * @return The instance of this class
             */
            static SystemManager &getInstance();

            /*!
             * @brief Get the the current timescale (useful when time manipulation is required)
             * @return The current timescale
             */
            float getTimeScale() const;
            /*!
             * @brief Set the time scale
             * @param scale The new timescale to use
             */
            void setTimeScale(float scale);

            /*!
             * @brief Get the current state of a system
             * @tparam T The type of the system to get
             * @return The current type of the system to get
             * @throw SystemNotFoundException if the system does not exist
             */
            template<typename T>
            SystemState getState() const;

            /*!
             * @brief Get a system by it's type to set options
             * @tparam T The type of the system to get
             * @return A reference to the system to get
             * @throw SystemNotFoundException if the system does not exist
             *
             * Please do not store systems directly as it will lead to errors if the system is destroyed
             */
            template<typename T>
            T &getSystem();

            /*!
             * @brief Make all the systems tick. You need to call this function one time per game loop
             * In the best of all words the game loop will only have a call to this function while the game is running
             */
            void tick();

            /*!
             * @brief Return all the errors that occurred since getErrors was last called
             * @return A vector of errors
             */
            std::vector<ErrorReport> getErrors();

            /*!
             * @brief Add a system to the existing systems
             * @tparam T The type of the system to add
             * @tparam Params The types of the parameters given to the system constructor
             * @param params The parameters to be given to the system constructor
             * @throw SystemAlreadyExistingException If a system of type T is still existing and has not be destroyed
             */
            template<typename T, typename ...Params>
            void addSystem(Params ...params);

            /*!
             * @brief Set a system state to TEARING_DOWN, the system will be destroyed on next tick
             * @tparam T The type of the system to remove
             * @throw SystemNotFoundException if the system to tear down is not to be found
             * @throw SystemLogicalException if the current system state does not allow it to be teared down
             * Only a STOPPED system can be teared down
             */
            template<typename T>
            void removeSystem();

            /*!
             * @brief Will set a system state to STOPPING
             * @tparam T The type of system to stop
             * @throw SystemNotFoundException if the system to stop is not to be found
             * @throw SystemLogicalException if the current system state does not allow it to be stopped
             * Only a RUNNING system can be stopped
             */
            template<typename T>
            void stopSystem();

            /*!
             * @brief Will start a system (set it's state to AWAKING or STARTING depending on the last state)
             * @tparam T The type of the system to start
             * @throw SystemNotFoundException if the system to stop is not to be found
             * @throw SystemLogicalException if the current system state does not allow it to be started
             * Only a NOT_STARTED/STOPPED system can be started.
             * A NOT_STARTED system will then be set to AWAKING and a STOPPED system will be set to STARTING
             */
            template<typename T>
            void startSystem();

        private:
            /*!
             * @brief ctor
             */
            SystemManager();

            /*!
             * @brief Add an error
             * @param sys The system
             * @param msg The message
             * @param type The type
             */
            void addError(ISystem &sys, const std::string &msg, ErrorReport::ErrorType type);

        private:
            std::unordered_map<std::type_index, std::pair<SystemState, ISystem *>> _systems; /*!< The systems */
            float _timeScale; /*!< A scaler for the time used (1 by default) */
            std::chrono::steady_clock::time_point _last; /*!< The internal clock used to compute time manipulations */
            std::vector<ErrorReport> _errors; /*!< The errors that happened since getErrors() was last called */
        };


        template<typename T, typename... Params>
        void SystemManager::addSystem(Params... params)
        {
            auto system = _systems.find(internal::getTypeIndex<T>());
            if (system != _systems.end()) {
                throw SystemAlreadyExistingException("Systen already existing", "addSystem");
            }
            T *newSys = new T(params...);
            _systems.insert(std::make_pair(internal::getTypeIndex<T>(), std::make_pair(NOT_STARTED, newSys)));
        }

        template<typename T>
        void SystemManager::removeSystem()
        {
            auto system = _systems.find(internal::getTypeIndex<T>());
            if (system == _systems.end()) {
                throw SystemNotFoundException("System not found", "stopSystem");
            }
            switch (system->second.first) {
            case NOT_STARTED:
                delete system.second.second;
                _systems.erase(system);
                break;
            case AWAKING:
                throw SystemLogicalException("Can not tear down an AWAKING system", "stopSystem");
            case STARTING:
                throw SystemLogicalException("Can not tear down a STARTING system", "stopSystem");
            case RUNNING:
                throw SystemLogicalException("Can not tear down a RUNNING system", "stopSystem");
            case STOPPING:
                throw SystemLogicalException("Can not tear down a STOPPING system", "stopSystem");
            case STOPPED:
                system->second.first = TEARING_DOWN;
                break;
            case TEARING_DOWN:
                throw SystemLogicalException("Can not stop a TEARING_DOWN system", "stopSystem");
            }
        }

        template<typename T>
        void SystemManager::stopSystem()
        {
            auto system = _systems.find(internal::getTypeIndex<T>());
            if (system == _systems.end()) {
                throw SystemNotFoundException("System not found", "stopSystem");
            }
            switch (system->second.first) {
            case NOT_STARTED:
                throw SystemLogicalException("Can not stop a NOT_STARTED system", "stopSystem");
            case AWAKING:
                throw SystemLogicalException("Can not stop an AWAKING system", "stopSystem");
            case STARTING:
                throw SystemLogicalException("Can not stop a STARTING system", "stopSystem");
            case RUNNING:
                system->second.first = STOPPING;
                break;
            case STOPPING:
                throw SystemLogicalException("Can not stop a STOPPING system", "stopSystem");
            case STOPPED:
                throw SystemLogicalException("Can not stop a STOPPED system", "stopSystem");
            case TEARING_DOWN:
                throw SystemLogicalException("Can not stop a TEARING_DOWN system", "stopSystem");
            }
        }

        template<typename T>
        void SystemManager::startSystem()
        {
            auto system = _systems.find(internal::getTypeIndex<T>());
            if (system == _systems.end()) {
                throw SystemNotFoundException("System not found", "startSystem");
            }
            switch (system->second.first) {
            case NOT_STARTED:
                system->second.first = AWAKING;
                break;
            case AWAKING:
                throw SystemLogicalException("Can not start an AWAKING system", "startSystem");
            case STARTING:
                throw SystemLogicalException("Can not start a STARTING system", "startSystem");
            case RUNNING:
                throw SystemLogicalException("Can not start a RUNNING system", "startSystem");
            case STOPPING:
                throw SystemLogicalException("Can not start a STOPPING system", "startSystem");
            case STOPPED:
                system->second.first = STARTING;
                break;
            case TEARING_DOWN:
                throw SystemLogicalException("Can not start a TEARING_DOWN system", "startSystem");
            }
        }

        template<typename T>
        jf::systems::SystemState jf::systems::SystemManager::getState() const
        {
            const auto system = _systems.find(internal::getTypeIndex<T>());
            if (system == _systems.end()) {
                throw SystemNotFoundException("System not found", "getState");
            }
            return system->second.first;
        }

        template<typename T>
        T &jf::systems::SystemManager::getSystem()
        {
            auto system = _systems.find(internal::getTypeIndex<T>());
            if (system == _systems.end()) {
                throw SystemNotFoundException("System not found", "getSystem");
            }
            return *static_cast<T *>(system->second.second);
        }

    }
}

#else

namespace jf {

    namespace systems {

        class SystemManager;
    }
}

#endif //JFENTITYCOMPONENTSYSTEM_SYSTEMMANAGER_HPP
