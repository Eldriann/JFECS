# JF Entity Component System

An entity component system implementation in C++.

Made by Julian Fräbel (<julian.frabel@epitech.eu>).

## Summary

1. [Summary](#summary)
2. [Introduction](#introduction)
3. [Entities and Components](#entities-and-components)
    1. [Entities](#entities)
    2. [Components](#components)
    3. [Entity Manager](#entity-manager)
4. [Systems](#systems)
    1. [System Interface](#system-interface)
    2. [System states and transitions](#system-states-and-transitions)
    3. [System Manager](#system-manager)
5. [Events](#events)
6. [Examples](#examples)
7. [Documentation](#documentation)

## Introduction

This ECS is currently divided in 3 main parts:
* [Entities/Components](#entities-and-components)
* [Systems](#systems)
* [Events](#events)

Each part has a manager that can be used to interact with the specifics elements.
Please refer to each section for more details.

To easily access one of the manager you can instantiate an ECSWrapper structure (see [examples](#examples)).

## Entities and Components

Entities and components are the elements that will be updated and represents "stuff" in a world.
An entity will have one or more components so you can see an entity as a "component container" and a component as a "data container".

The manager to use is `jf::entities::EntityManager`.
You can get the instance of this singleton class with `jf::entities::EntityManager::getInstance()` or use the usual `ECSWrapper`.

### Entities

To create a new entity simply use the Manager function:
```cpp
EntityHandler createEntity(const std::string &name); //name is simply the name to give to this new entity
```
This function will return a `jf::entities::EntityHandler` with the newly created entity.

An `EntityHandler` is a helper class that you will use to manipulate and store Entities in your code.
Simply use the `->` or `*` operator to access the Entity methods.
> **WARNING**: Never store a `jf::entity::Entity` directly in your code always use a EntityHandler

An Entity consist of a unique id and a name.
Both can be used to identify an entity but be aware ids are unique where names are not.
Entities also have a set of components on which logic will happend.

Entities can be enabled and disabled using the function:
```cpp
void setEnable(bool enabled);
```
> **INFO**: Entities can be later filter if they are enabled or not when applying logic to them

You can also indicate to others that an entity should not be destroyed with:
```cpp
void setShouldBeKeeped(bool keep);
```
> **INFO**: Entities that should be keeped will not be destroy on the call to `EntityManager::deleteAllEntities()` function

Of course both previous functions and identifiers have also getters. 
Please see the [documentation](#documentation) for more details.

You can add/remove components to an entity by using the functions:
```cpp
template<typename C, typename ...Params>
components::ComponentHandler<C> assignComponent(Params ...params);

template<typename C>
bool removeComponent();
```
> **INFO**: See the [documentation](#documentation) or the [examples](#examples) for details on how to use them.

To check if a given entity has a component / a set of components use the functions:
```cpp
template<typename C>
bool hasComponent();

template<typename C>
bool hasComponents(); //Works exactly the same as hasComponent but is needed for the other form of hasComponents

template<typename CA, typename CB, typename... Others>
bool hasComponents();
```
> **INFO**: See the [documentation](#documentation) or the [examples](#examples) for details on how to use them.

Finally you can get a `jf::components::ComponentHandler` to a component of a given entity by using:
```cpp
template<typename C>
components::ComponentHandler<C> getComponent();

template<typename C>
std::tuple<components::ComponentHandler<C>> getComponents();

template<typename CA, typename CB, typename... Others>
std::tuple<components::ComponentHandler<CA>, components::ComponentHandler<CB>, components::ComponentHandler<Others>...> getComponents();
```
> **INFO**: See the [documentation](#documentation) or the [examples](#examples) for details on how to use them.

### Components

Component can be seen as data containers.
A component will be used to store and compute logical operations on data depending on its type.

You should not create a component without an entity to hold it.
Refer to the section [entities](#entities) to see how to add (and create) a compenent to an entity.

If you need to store a reference / pointer to a component in your code always use a `jf::components::ComponentHandler`.
Like the `EntityHandler` you can use the operators `->` and `*` to access the component data and methods.
> **WARNING**: Remember never store directly a component. Alway use the Handler.

If you want to create a custom component you need to make it derive from `jf::components::Component` and need to call the default constructor of component.
Your custom component structure should look like this:
```cpp
class MyCustomComponent : public jf::components::Component {
public:
    MyCustomComponent(entities::Entity &entity, /* whatever you need*/)
    : jf::components::Component(entity) {
        EMIT_CREATE(MyCustomComponent);
    }
    
    ~MyCustomComponent() {
        EMIT_DELETE(MyCustomComponent);
    }
    
    /* Whatever methods you need */
}
```
> **INFO**: EMIT_CREATE and EMIT_DELETE are special macros used by the [EventSystem](#events) to emit the specific creation and destruction events of your component.
> They are not mandatory and can be removed if you do not wish specific events to be send.

You can always get the entity a component is sitting on by using the function:
```cpp
entities::EntityHandler getEntity();
```

### Entity Manager

The entity manager is used *as his name suggest* to manage entities.
With it you can [create](#entities), destroy, find but also update an entity based on its components.

To destroy an entity simply use it's id in the function:
```cpp
bool deleteEntity(const internal::ID &entityID);
```
> **INFO**: For this and the following functions please see the [documentation](#documentation) or the [examples](#examples) for details on how to use them.

The function used to retrieve an entity by its identifiers are as follow:
```cpp
EntityHandler getEntityByID(const internal::ID &entityID);

EntityHandler getEntityByName(const std::string &entityName, bool onlyEnabled = true);

std::vector<EntityHandler> getEntitiesByName(const std::string &entityName, bool onlyEnabled = true);
```
> **INFO**: Remember a name is not unique many entities can have the same name but not the same id

> **INFO**: Remember the part where you could disable entities? It start to become useful here.

You can also find entities that matches a set of components:
```cpp
std::vector<EntityHandler> getEntitiesWith(bool onlyEnabled = true);
```

The real powerful parts of this manager are the two functions:
```cpp
void applyToEach(std::function<void(EntityHandler)> func, bool onlyEnabled = true);

template<typename C, typename... Others>
void applyToEach(typename std::common_type<std::function<void(EntityHandler, components::ComponentHandler<C>, components::ComponentHandler<Others>...)>>::type func, bool onlyEnabled = true);
/* Don't mind the typename std::common_type<>::type it's a trick used to create a std::function object from the callable object implicitly in a template */
```
that can be used to apply a [callable object](https://en.cppreference.com/w/cpp/utility/functional/function) (lamda, functions and functor are callable objects) to every entities that have a specific set of components.
> **INFO**: Check [this](https://en.cppreference.com/w/cpp/types/common_type) to know more about std::common_type. Huge thanks to [redxdev ecs repository](https://github.com/redxdev/ECS) for showing me that it was possible to implicitly use a lambda as std::function in templated argument.

## Systems

Systems are what update the components from an entity.

### System Interface

To create a new system you need to create a class that derives from `jf::systems::ISystem`.

The different functions to implement are the functions that will be called when a system state is changed (see the [following table](#system-states-and-transitions) to know the transitions).

On a system destruction the states will be called in the following order starting from the state the system is currently in:
onAwake -> onStart -> onStop -> onTearDown.
A NOT_STARTED system will simply be destroyed and no additional function will be called.

### System States and Transitions

The following table represent the existing system states and the possible transitions with each state.

| State        | Next tick state | Next state if started | Next state if stopped | Next state if teared down |
|:------------:|:---------------:|:---------------------:|:---------------------:|:-------------------------:|
| NOT_STARTED  | NOT_STARTED     | AWAKING               | X                     | UNEXISTING                |
| AWAKING      | STARTING        | X                     | X                     | X                         |
| STARTING     | RUNNING         | X                     | X                     | X                         |
| RUNNING      | RUNNING         | X                     | STOPPING              | X                         |
| STOPPING     | STOPPED         | X                     | X                     | X                         |
| STOPPED      | STOPPED         | STARTING              | X                     | TEARING_DOWN              |
| TEARING_DOWN | UNEXISTING      | X                     | X                     | X                         |
> **INFO**: Note that an X represent that an exception will be throwed if tried to be done.

> **INFO**: UNEXISTING is not an existing state it just mean that the system will not exist anymore

### System Manager

The manager to use is `jf::systems::SystemManager`.
You can get the instance of this singleton class with `jf::systems::SystemManager::getInstance()` or use the usual `ECSWrapper`.

With the system manager you can add, remove, start, stop and tick systems.

When ticking if an error is throwed it will be caught by the SystemManager (if the exception derive from `std::exception`).
You can retrieve errors and handle them as you like by using the function:
```cpp
std::vector<ErrorReport> getErrors();
```

An `ErrorReport` is composed of a reference to the system that throwed the exception (as an ISystem, so you need to dynamic cast it if you want to use your functions), the error message and an `ErrorType` describing the state of the system when the exception was throwed.

To add or remove a system you need to call the following functions:
```cpp
template<typename T, typename ...Params>
void addSystem(Params ...params);

template<typename T>
void removeSystem();
```
> **INFO**: As usual refer to the [documentation](#documentation) or the [examples](#examples) for additional details

When a system has been added it's in the NOT_STARTED state and need to be started.

If you need to get a system that has been added you can use the following function:
```cpp
template<typename T>
T &getSystem();
```
> **WARNING**: As you can see this function returns a reference to the system.
> DO NOT store this reference in one of your classes as it can lead to undefined behaviour if the system is destroyed.

To start a system use the function:
```cpp 
template<typename T>
void startSystem();
```
> **INFO**: Remember on the first start onAwake will be called before onStart on the system. On the following starts only onStart will be called.

To stop a system use the function:
```cpp
template<typename T>
void stopSystem();
```
> **INFO**: Remember a system can not be stopped at any time check the [table](#system-states-and-transitions) if you need more infos.

You can also at any time get the current state of a system with the use of:
```cpp
template<typename T>
SystemState getState() const;
```

The main part of the system manager is the `tick` function that update all systems and make needed states transitions.
When ticking RUNNING systems will be given the elapsed time since their onUpdate function was last called. 
> **INFO**: The time in onUpdate is in nanoseconds

Time is handled inside the `SystemManager` with the use of a [`std::chrono::steady_clock`](https://en.cppreference.com/w/cpp/chrono/steady_clock) but you can influence time with a scaler.
The associated functions are:
```cpp
float getTimeScale() const;

void setTimeScale(float scale);
```
This scaler will be used as a multiplier on the elapsed time (0.5 will slow down time where 2 will double it).

## Events

The manager to use is `jf::events::EventManager`. 
You can get the instance of this singleton class with `jf::events::EventManager::getInstance()` or use the usual ECSWrapper.

The event part only uses 3 functions, one to add a listener, one to remove a listener and one to emit an event.

A listener is *something* that register to a type of event by giving a callback to use.

An event is a structure or a class.
There is no interface as it can be everything (even templates).

A callback is a [callable object](https://en.cppreference.com/w/cpp/utility/functional/function) that take as argument the pointer given at the registration and the event that was emitted.

To add a new listener use:
```cpp
template<typename RegistererType, typename EventType>
internal::ID addListener(RegistererType *registerer, typename std::common_type<std::function<void(RegistererType *, EventType)>>::type callback);
/* As in the EntityManager don't mind the std::common_type it's realy just a std::function */
```
> **INFO**: This callback will be called when emit\<EventType\> is called with the registerer as first parameter and the actual event as the second one.

> **INFO**: The event manager does not care what the registerer type is, as such there is no security that the memory pointed by registerer is valid.

To remove a listener use the ID given in addListener:
```cpp
void removeListener(const internal::ID &id);
```

To emit an event use:
```cpp
template<typename EventType>
void emit(const EventType &event);
```
> **INFO**: The event data will not be copied to any listener but events should not be used to modify the thing that has created the event but should be used to update the registerer based on what happened.
> This also implies that an EventType should be copy constructable.

> **REMINDER**: If ever you need to parse an Entity or a Component in an event use the handler to ensure safety as a check will be performed on the validity of the contained data.

Some events are integrated and emitted by the ecs:
* EntityCreatedEvent will be emitted when a new entity is created
* EntityDestroyedEvent will be emitted when an entity is destroyed
* ComponentCreatedEvent is a templated event that will be emitted when a new component of a certain type is emitted
    > **INFO**: `ComponentCreatedEvent<Component>` will be emitted every time a component is created
* ComponentDestroyedEvent is a templated event that will be emitted when a component of a certain type is destroyed
    > **INFO**: `ComponentDestroyedEvent<Component>` will be emitted every time a component is destroyed

If you create a [custom component](#components) you can call in it's constructor and destructor the macros `EMIT_CREATE` and `EMIT_DELETE` so that your custom component automatically call the `ComponentCreatedEvent` and `ComponentDestroyedEvent` events of your component type.

## Examples

In the following example we will create a custom component that hold an int, a custom system that display the value of this component every tick, a custom listener that will display each time our custom component is updated and a custom event fired every time the value of the custom component is updated.

> **REMINDER**: Dont forget to protect your headers (not showed here).

The header files:
```cpp
//IntComponent.hpp

#include "Component.hpp"

/*!
 * @class IntComponent
 * @brief A custom component that hold an int
 */
class IntComponent : public jf::components::Component //Remember to derive from jf::components::Component
{
public:
    /*!
     * @brief ctor
     * @param entity The entity this component sit on
     * @param val The default value of this component
     */
    IntComponent(jf::entities::Entity &entity, int val);

    /*!
     * @brief dtor
     */
    ~IntComponent();

    /*!
     * @brief Return the current value of the component
     * @return An int containing the value of the component
     */
    int getValue() const;

    /*!
     * @brief Set the value of the component
     * @param val The value to set the component to
     */
    void setValue(int val);

private:
    int _val; /*!< The value of the component */
};
```
> This class will be our custom component holding an integer

```cpp
//IntDisplaySystem.hpp

#include "System.hpp"

/*!
 * @class IntDisplaySystem
 * @brief A custom system that will display the value of the IntComponents
 */
class IntDisplaySystem : public jf::systems::ISystem {
public:
    void onAwake() override;

    void onStart() override;

    void onUpdate(const std::chrono::nanoseconds &elapsedTime) override;

    void onStop() override;

    void onTearDown() override;
};
```
> This class is our system that will display the values of ours IntComponents

```cpp
//IntComponentValueChangedEvent.hpp

#include "ComponentHandler.hpp"
#include "IntComponent.hpp"

/*!
 * @struct IntComponentValueChangedEvent
 * @brief An event that will be emitted when the value of an IntComponent is updated
 */
struct IntComponentValueChangedEvent {
    jf::components::ComponentHandler<IntComponent> component; /*!< The component that was updated */
};
```

The source files:
```cpp
//IntComponent.cpp

#include "ECSWrapper.hpp"
#include "IntComponent.hpp"
#include "IntComponentValueChangedEvent.hpp"
#include "ComponentCreatedEvent.hpp"

IntComponent::IntComponent(jf::entities::Entity &entity, int val)
    : Component(entity), _val(val)
{
    EMIT_CREATE(IntComponent); //We want to emit the event ComponentCreatedEvent<IntComponent>
}

IntComponent::~IntComponent()
{
    EMIT_DELETE(IntComponent); //We want to emit the event ComponentDestroyedEvent<IntComponent>
}

int IntComponent::getValue() const
{
    return _val;
}

void IntComponent::setValue(int val)
{
    _val = val;
    ECSWrapper ecs;
    ecs.eventManager.emit<IntComponentValueChangedEvent>({jf::components::ComponentHandler<IntComponent>(this)});
}
```

```cpp
//IntDisplaySystem.cpp

#include <iostream>
#include "ECSWrapper.hpp"
#include "IntDisplaySystem.hpp"
#include "IntComponent.hpp"

void IntDisplaySystem::onAwake()
{
    std::cout << "IntDisplaySystem onAwake" << std::endl;
}

void IntDisplaySystem::onStart()
{
    std::cout << "IntDisplaySystem onStart" << std::endl;
}

void IntDisplaySystem::onUpdate(const std::chrono::nanoseconds &elapsedTime)
{
    std::cout << "IntDisplaySystem onUpdate elapsed time: " << elapsedTime.count() << " ns" << std::endl;
    ECSWrapper ecs;
    //Note that we can use auto the lamda to deduce the type of our component, here: jf::entities::ComponentHandler<IntComponent>
    ecs.entityManager.applyToEach<IntComponent>([](jf::entities::EntityHandler entity, auto component) {
        std::cout << "Entity '" << entity->getName() << "' has for value in IntComponent: " << component->getValue() << std::endl;
    });
}

void IntDisplaySystem::onStop()
{
    std::cout << "IntDisplaySystem onStop" << std::endl;
}

void IntDisplaySystem::onTearDown()
{
    std::cout << "IntDisplaySystem onTearDown" << std::endl;
}
```

```cpp
//main.cpp

#include "ECSWrapper.hpp"
#include "IntComponent.hpp"
#include "IntDisplaySystem.hpp"
#include "IntComponentValueChangedEvent.hpp"

void callback(__attribute__((unused)) void *data, IntComponentValueChangedEvent event)
{
    std::cout << "IntComponentValueChangedEvent Event callback:, entity name: '" << event.component->getEntity()->getName() << "' new value: " << event.component->getValue() << std::endl;
}

int main()
{
    ECSWrapper ecs;

    //First we add our listener
    ecs.eventManager.addListener<void, IntComponentValueChangedEvent>(
        nullptr, //We do not need a registerer as we only display stuff but you would normally put your pointer here
        &callback //We can also used functions pointer not only lambda ;)
    );
    
    //Then we create some entities and setup components
    auto entityA = ecs.entityManager.createEntity("A");
    auto componentA = entityA->assignComponent<IntComponent, int>(6);
    
    auto entityB = ecs.entityManager.createEntity("B");
    auto componentB = entityB->assignComponent<IntComponent, int>(12);
    
    //We also need to register our system
    ecs.systemManager.addSystem<IntDisplaySystem>(); //IntDisplaySystem does not need any argument
    ecs.systemManager.startSystem<IntDisplaySystem>(); //We want to start it as soon as the ecs start ticking
    
    //We loop 5 times and update componentA on the third time
    
    for (int i = 0; i < 5; ++i) {
        std::cout << "Loop: " << i << std::endl;
        ecs.systemManager.tick();
        if (i == 2) {
            componentA->setValue(3);
        }
    }
    
    return 0;
}
```

The following result will be displayed:

```
$> ./example
Loop: 0
IntDisplaySystem onAwake
Loop: 1
IntDisplaySystem onStart
Loop: 2
IntDisplaySystem onUpdate elapsed time: 25754 ns
Entity 'B' has for value in IntComponent: 12
Entity 'A' has for value in IntComponent: 6
IntComponentValueChangedEvent Event callback:, entity name: 'A' new value: 3
Loop: 3
IntDisplaySystem onUpdate elapsed time: 48276 ns
Entity 'B' has for value in IntComponent: 12
Entity 'A' has for value in IntComponent: 3
Loop: 4
IntDisplaySystem onUpdate elapsed time: 28521 ns
Entity 'B' has for value in IntComponent: 12
Entity 'A' has for value in IntComponent: 3
IntDisplaySystem onStop
IntDisplaySystem onTearDown
```

## Documentation
You can generate additional documentation for this project using [Doxygen](http://www.doxygen.nl) by using the Doxyfile at the root of this repository.
```
$> cd /path/to/the/repository
$> mkdir docs
$> doxygen ./Doxyfile
$> web_browser ./docs/html/index.html
```