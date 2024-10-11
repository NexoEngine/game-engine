\page network_readme Network
# R-TYPE Network Documentation

## Table of Contents

- [R-TYPE Network Documentation](#r-type-network-documentation)
  - [Table of Contents](#table-of-contents)
  - [Introduction](#introduction)
  - [Components](#components)
    - [Network Messages](#network-messages)
    - [Client-Server Interaction](#client-server-interaction)
      - [Client.hpp (ClientInterface)](#clienthpp-clientinterface)
      - [Server.hpp (ServerInterface)](#serverhpp-serverinterface)
    - [Connection Management (Connection.hpp)](#connection-management-connectionhpp)
    - [Message Handling (Message.hpp)](#message-handling-messagehpp)
    - [Thread-Safe Queues (TsQueue.hpp)](#thread-safe-queues-tsqueuehpp)
    - [Main Server Logic](#main-server-logic)
  - [Key Concepts](#key-concepts)
      - [Serialization with Boost](#serialization-with-boost)
      - [Asynchronous Communication](#asynchronous-communication)
      - [Client-Server Model](#client-server-model)
      - [Concurrency and Thread Safety](#concurrency-and-thread-safety)
      - [Network Implementation UML Diagram](#network-implementation-uml-diagram)

## Introduction

This documentation provides an overview of the network architecture of the R-TYPE server, including the key components, concepts, and functionalities. It's intended to help you understand the network design and implementation of the server, as well as the role of each component in the overall architecture.

## Components

### Network Messages

* **NetworkMessage.hpp**: This file defines an enumeration (**NetworkMessage**) that lists all types of messages that can be sent and received in the network. This includes server responses like **ServerAccept** and **ServerDeny**, client actions like **ClientConnect** and **ClientUpdatePlayerDirection**, and game-specific messages like **ServerCreatePlayerShip** and **ServerUpdateEnemyVelocity**.

* **NetworkBody.hpp**: This file includes various structures corresponding to each message type defined in **NetworkMessage.hpp**. Each structure represents the body of a network message and is tailored to carry the data relevant to its message type. For instance, **ServerCreatePlayerShip** includes fields like **entityNetId**, **shipName**, and **pos**. These structures are equipped with serialization functions to convert the data into a format suitable for network transmission, using Boost serialization.

Together, these components define the schema and data handling for all network communication within your R-TYPE server.

### Client-Server Interaction

#### Client.hpp (ClientInterface)

* **connect()**: Establishes a connection to the server. It initiates the process of joining the server, essential for multiplayer interactions.

* **disconnect()**: Handles the disconnection process from the server. This method is crucial for gracefully exiting the game or handling lost connections.

* **sendMessage()**: Responsible for sending messages to the server. This method plays a key role in communicating actions or requests from the client to the server.

* **handleMessage()**: Processes incoming messages from the server. It's crucial for receiving and acting upon updates or commands from the server.

#### Server.hpp (ServerInterface)

* **start()**: Initiates the server, making it ready to accept client connections. This is the entry point for the server's operation.

* **stop()**: Gracefully shuts down the server. This method is important for proper resource management and termination of server processes.

* **waitForConnection()**: Listens for and handles new client connections. This function is key to expanding the multiplayer environment.

* **broadcastMessage()**: Sends messages to all connected clients. Essential for synchronizing game state across all players.

* **handleClientMessage()**: Processes messages received from clients. It's critical for implementing game logic based on client inputs.

### Connection Management (Connection.hpp)

* **Connection Class Template**: Manages individual network connections, encapsulating the details of message transmission and reception.

* **send()**: Responsible for sending messages over the network. This method is critical for both client and server to communicate data effectively.

* **receive()**: Handles incoming messages, ensuring the data sent by the other party is correctly received and processed.

* **getHeader()**: Extracts the header of a received message, crucial for understanding the message context and type.

* **getBody()**: Retrieves the body of the message after the header is processed. This method is essential for accessing the actual content or data of the message.

* **close()**: Safely closes the connection. It's important for resource management and preventing leaks or errors when a connection is no longer needed.

These components work together to efficiently manage the complexities of network communication within the R-TYPE server environment.

### Message Handling (Message.hpp)

* **Message Class Template**: Defines the structure and functionalities for network messages, both for sending and receiving.

* **serialize()**: Converts the message data to a serialized format suitable for transmission. Essential for preparing data to be sent over the network.

* **deserialize()**: Transforms received serialized data back into its original format. Key for interpreting received messages.

* **getHeader()**: Retrieves the header from a message, providing information like message type and size.

* **setHeader()**: Assigns a header to a message, crucial for identifying and categorizing the message before sending.

* **getBody()**: Accesses the body of the message, where the main content or data is stored.

* **setBody()**: Sets the body of the message with the necessary data to be transmitted.

These methods ensure that messages are correctly formatted, sent, and interpreted within the R-TYPE server network communication system.

### Thread-Safe Queues (TsQueue.hpp)

* **TsQueue Class Template**: Implements a thread-safe queue to manage messages in a concurrent environment, ensuring safe access across multiple threads.

* **enqueue()**: Adds a new item to the queue. This is crucial for inserting messages into the queue in a thread-safe manner.

* **dequeue()**: Removes and returns an item from the queue. It's essential for processing messages in the order they were received.

* **isEmpty()**: Checks whether the queue is empty. This method is useful for determining if there are messages waiting to be processed.

* **clear()**: Empties all items from the queue. Important for resetting the queue or clearing pending messages when necessary.

This component is vital for maintaining the integrity of message handling in a multi-threaded environment, ensuring that message processing is seamless and free from concurrency issues.

### Main Server Logic

* **Server.cpp**: This file contains the core functionality of the server. It includes the main loop for handling network events, processing client requests, and managing game logic. It's responsible for initializing the server, setting up connections, and coordinating the various components of the server.

* **Main.cpp**: Acts as the entry point of the server application. It typically initializes the server environment, creates necessary objects and components, and starts the server loop. It's crucial for bootstrapping the server and orchestrating the high-level flow of the application.

These components collectively form the backbone of the server, ensuring that it runs smoothly, handles client interactions appropriately, and maintains the game state effectively.

## Key Concepts

#### Serialization with Boost

    Involves converting data structures or objects into a format that can be easily stored or transmitted and then reconstructed later. Boost's serialization library is used for this purpose, providing an efficient way to handle complex data types like Vector3.

#### Asynchronous Communication

    Refers to the method of data transmission where sending/receiving of messages doesn't block the execution of other tasks. This is crucial in a networked environment like R-TYPE server to maintain performance and responsiveness.

#### Client-Server Model

    The architectural foundation of the game's networking. The server acts as a central authority managing game logic and state, while clients connect to the server to play the game. This model facilitates multiplayer interactions and synchronizes game states among different players.

#### Concurrency and Thread Safety

    Managing the execution of several sequences of operations simultaneously without causing conflicts or data corruption. Thread-safe queues and synchronization mechanisms are used to ensure safe access to shared resources in a multi-threaded environment.

These concepts are foundational to the network architecture of the R-TYPE server, ensuring efficient, reliable, and concurrent operations.

#### Network Implementation UML Diagram

![Network UML](network/network_uml.png)