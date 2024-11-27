\page ecs_readme ECS
# ECS Design

## Table of Contents

- [ECS Design](#ecs-design)
  - [Table of Contents](#table-of-contents)
  - [What is ECS ?](#what-is-ecs-)
    - [Why use ECS ?](#why-use-ecs-)
  - [Core concepts](#core-concepts)
  - [How does it work ?](#how-does-it-work-)
    - [But first, the signatures...](#but-first-the-signatures)
  - [Now we can talk...](#now-we-can-talk)
    - [First of the all, the **EntityManager**:](#first-of-the-all-the-entitymanager)
    - [Second, the **ComponentManager**:](#second-the-componentmanager)
      - [**ComponentArray**](#componentarray)
      - [**ComponentManager**](#componentmanager)
    - [Finally, the SystemManager:](#finally-the-systemmanager)
      - [SystemManager](#systemmanager)
  - [A global overview of the workflow](#a-global-overview-of-the-workflow)
    - [Initialization](#initialization)
    - [Entity Management](#entity-management)
    - [Component Management](#component-management)
    - [System Management](#system-management)
    - [Workflow Summary](#workflow-summary)
    - [EC Systems UML Diagram](#ec-systems-uml-diagram)

## What is ECS ?

ECS stands for Entity-Component-System. It's a design pattern that is used to decouple data from logic.
___
## Core concepts
- **Entity:** A general-purpose object identified by a unique ID. Entities are lightweight and don't contain data or behavior themselves.  
- **Component:** Raw data for one aspect of an entity, like position, health, etc. Components are simple structs or classes containing only data.  
- **System:** Logic that processes entities with a specific set of component types.

## How does it work ?
This ECS design consist of several class.  
At the top level, there is the **Coordinator class**. This class ties everything together, and the only thing it really does is call the method of the different managers.   

### But first, the signatures...
Before talking about the managers, i need to introduce the concept of signature.  
A signature is a `std::bitset` (basically an array of 0's and 1's). Each Entity has a signature describing the components that are tied to it. When you register a new component, a unique ID is tied to its type. This unique ID corresponds to an index in the signature.  

> **Example:** Let's say you have a position component. When you register it in the **ComponentManager**, a unique id is tied to its type. You can then create a new signature and set inside it the index corresponding to the unique id to 1.  
Now, if you tie this signature to an entity, this entity will have the position component and all of the systems needing the position component will know that this entity can be affected by it.
___
## Now we can talk...
### First of the all, the **EntityManager**: 
The EntityManager class is focused on the management of entities. An entity is essentially an ID that represents a game object, and the EntityManager is responsible for creating, tracking, and destroying these entities. Let's break down its responsibilities:

**Core Responsibilities:**  

> **Entity Creation (createEntity):** Handles the creation of a new entity. It typically involves assigning a unique ID to the entity. If there are available IDs (recycled from destroyed entities), it reuses them; otherwise, it creates a new one. This process ensures that the system can handle a large number of entities efficiently by reusing entity IDs while also efficiently packing the data, avoiding missed CPU cache-line.

> **Entity Destruction (destroyEntity):** This function is responsible for properly disposing of an entity. When an entity is no longer needed (like when a game object is removed from the game), this function is called to release its ID and mark it as available for future use. This is crucial for managing the lifecycle of game objects without leaking resources or IDs.

> **Signature Management (setSignature, getSignature):** Each entity in an ECS architecture has a signature, which is essentially a bitset where each bit represents whether the entity has a particular component type. This feature allows systems in the ECS framework to quickly determine which entities have the components they are interested in processing. The EntityManager is responsible for updating (with setSignature) and querying (with getSignature) these signatures.
### Second, the **ComponentManager**: 
This one is a bit trickier, its goal is to handle all the different components (which are basically structs).  

#### **ComponentArray<T>**
>This class is a template class, meaning it's designed to store components of any type (T). It's used to manage a specific type of component for all entities. Each ComponentArray instance corresponds to a different type of component in the ECS architecture. Its primary functions include:
>>**Inserting Components:** When a new component is added to an entity, it's stored in an array. The class maintains a mapping between the entity ID and its index in this array, ensuring fast access.
> 
>>**Removing Components:** When a component is removed from an entity, this class updates its internal structures to reflect this change, keeping the array packed by moving the last element to the position of the removed element.
>
>>**Accessing Components:** It allows quick retrieval of a component attached to an entity using the entity ID.
>
>>**Handling Entity Destruction:** If an entity is destroyed, this class ensures that all components associated with that entity are properly removed.
This class is a crucial part of the ECS architecture, enabling efficient and type-safe storage and retrieval of components.

#### **ComponentManager**
>The ComponentManager serves as the central point for managing different types of components in the ECS framework. It's responsible for:
>
>>**Component Type Registration:** Each component type (like Position, Velocity, etc.) is registered with a unique identifier. This registration process involves creating a ComponentArray for each new type.
>
>>**Adding and Removing Components:** It delegates the task of adding or removing components to the appropriate ComponentArray, based on the component type.
>
>>**Component Retrieval:** It provides a way to access specific components of an entity, again delegating to the relevant ComponentArray.
>
>>**Entity Destruction Handling:** When an entity is destroyed, the ComponentManager notifies each ComponentArray to remove any components associated with the destroyed entity.

In essence, the ComponentManager abstracts the complexities of handling various component types, allowing the rest of the ECS system to interact with components through a unified interface.

### Finally, the SystemManager:
The System class is a base class for all systems within the ECS framework. Systems are responsible for the logic that operates on entities that have a specific set of components.  
 
Each system maintains a set of entities that it is interested in. These are the entities that have the components required by this system to operate.

#### SystemManager
The SystemManager plays a central coordinating role for all the systems in the ECS architecture. It's responsible for:

>**System Registration (registerSystem):** This function is used to create and register a new system. It ensures that each system type is registered only once and stores a shared pointer to the created system. This approach allows for easy and efficient management of different system types within the game.

>**Setting System Signatures (setSignature):** Each system operates on entities with a specific set of components. The setSignature function associates a signature (a bitset representing component types) with a system. This signature is used to determine which entities the system should process.

>**Handling Entity Destruction (entityDestroyed):** When an entity is destroyed, SystemManager ensures that this entity is removed from all system's tracking sets, maintaining the integrity of each system's entity list.

>**Updating Entity Signatures (entitySignatureChanged):** When an entity's signature changes (due to adding or removing components), the SystemManager updates the entity sets of all systems. It checks each system's signature against the entity's new signature. If the entity's signature matches a system's signature, the entity is added to that system's set; otherwise, it's removed.

## A global overview of the workflow
The Coordinator class serves as the central orchestrator for the ECS, tying together the functionality of the EntityManager, ComponentManager, and SystemManager. Here's an overview of the global workflow in this ECS architecture:

### Initialization
>**Init Phase:** Upon initialization (init method), the Coordinator creates instances of EntityManager, ComponentManager, and SystemManager. This setup establishes the foundational structures for entity management, component handling, and system operations.

### Entity Management
>**Creating Entities:** When a new entity is needed (createEntity), the EntityManager generates a new entity ID.

>**Destroying Entities:** When an entity is destroyed (destroyEntity), the EntityManager, ComponentManager, and SystemManager are all notified to remove any references to this entity, clean up components, and update system entity lists.

### Component Management
>**Registering Components:** Components are registered (registerComponent) with the ComponentManager, allowing for the creation and tracking of different component types.

>**Adding Components:** When a component is added to an entity (addComponent), it is stored in the ComponentManager, and the entity's signature in the EntityManager is updated. The SystemManager is also notified to update the entity lists in relevant systems.

>**Removing Components:** Removing a component from an entity (removeComponent) follows a similar process but in reverse, updating the entity's signature to no longer include the component type.

### System Management
>**Registering Systems:** Systems are registered (registerSystem) with the SystemManager. Each system is responsible for processing entities with specific components.

>**Setting System Signatures:** Each system has a signature (setSystemSignature) that determines which entities it will process, based on their components.
### Workflow Summary
When a game object is created, an entity ID is generated.  
As components are added to or removed from this entity, the Coordinator updates the entity's signature to reflect its current component makeup.  
The SystemManager uses these signatures to maintain an up-to-date list of entities for each system.  
Each system processes its specific subset of entities, based on the components they possess.

### Examples
#### Basic example
First we create two very basic components :
```c++
struct Position {
    float x, y, z;
};

struct Velocity {
    float x, y, z;
};
```
Then we set up the `MovementSystem`:
```c++
class MovementSystem : public nexo::ecs::System {
    public:
        void update(float deltaTime) {
            for (auto entity : entities) {
                auto& position = coord->getComponent<Position>(entity);
                auto& velocity = coord->getComponent<Velocity>(entity);

                position.x += velocity.x * deltaTime;
                position.y += velocity.y * deltaTime;
                position.z += velocity.z * deltaTime;
            }
        }
};
```
> [!CAUTION]
The coordinator is a static member initialized during the init call of the coordinator class.  
> It is not thread safe ! So manage with caution

Now we can do our basic initialization routine :
```c++
nexo::ecs::Coordinator coordinator;
coordinator.init();

coordinator.registerComponent<Position>();
coordinator.registerComponent<Velocity>();
```
Create our system and assign it its signature :
```c++
auto movementSystem = coordinator.registerSystem<MovementSystem>();
nexo::ecs::Signature movementSignature;
movementSignature.set(coordinator.getComponentType<Position>(), true);
movementSignature.set(coordinator.getComponentType<Velocity>(), true);
coordinator.setSystemSignature<MovementSystem>(movementSignature);
```
Then create our entities and attach them the different components :
```c++
auto entity1 = coordinator.createEntity();
coordinator.addComponent(entity1, Position{0.0f, 0.0f, 0.0f});
coordinator.addComponent(entity1, Velocity{1.0f, 0.0f, 0.0f});

auto entity2 = coordinator.createEntity();
coordinator.addComponent(entity2, Position{5.0f, 5.0f, 5.0f});
coordinator.addComponent(entity2, Velocity{0.0f, -1.0f, 0.0f});
```
And finally, call the update function of our system inside our game loop :
```c++
for (int frame = 0; frame < 10; ++frame) {
    std::cout << "Frame " << frame << ":\n";

    movementSystem->update(0.016f); // Assuming 60 FPS, so ~16ms per frame

    // This is just an example of how you can then retrieve the component to do other stuff
    for (auto entity : movementSystem->entities) {
        auto& position = coordinator.getComponent<Position>(entity);
        std::cout << "Entity " << entity << " Position: ("
                  << position.x << ", " << position.y << ", " << position.z << ")\n";
    }

    std::cout << "---------------------\n";
}
```
[!NOTE]
You can find the full example along with a CMakeLists inside the `examples/ecs` folder.

### EC Systems UML Diagram
![ECS Systems UML Diagram](game_engine/ecs_systems_uml.png)
