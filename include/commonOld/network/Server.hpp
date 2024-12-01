/*
** EPITECH PROJECT, 2023
** server
** File description:
** Server.hpp
*/

#pragma once

#include "common.hpp"
#include "TsQueue.hpp"
#include "Message.hpp"
#include "Connection.hpp"
#include <unordered_map>

#include <unordered_map>

namespace rtype::net {
    template<typename T>
    class ServerInterface {
        public:
            /**
             * @brief Construct a new Server Interface object
             * @param port Port number to listen on
             */
            explicit ServerInterface(uint16_t port);

            virtual ~ServerInterface();

            /**
             * @brief Start the server
             * @return bool True if server started successfully, false otherwise
             */
            bool start();

            /**
             * @brief Stop the server
             */
            void stop();

            /**
             * @brief Instruct asio to wait for connection
             * @note Asynchronous
             */
            void waitForClientConnection();

            /**
             * @brief Send a message to a specific client
             * @param client Client to send message to
             * @param msg Message to send
             */
            void messageClient(std::shared_ptr<Connection<T>> client, const Message<T>& msg);

            /**
             * @brief Send a message to all clients
             * @param msg Message to send
             * @param pIgnoreClient Client to ignore
             */
            void messageAllClients(const Message<T>& msg, std::shared_ptr<Connection<T>> pIgnoreClient = nullptr);

            /**
             * @brief Update the server
             * @param nMaxMessages Maximum number of messages to process
             * @param bWait Wait for message to arrive (prevent thread from using all ressources)
             */
            void update(size_t nMaxMessages = -1, bool bWait = false);

            /**
             * @brief Get connection object via its network ID
             * @param nID Network ID
             * @return std::shared_ptr<Connection<T>> Connection object
             */
            std::shared_ptr<Connection<T>> getConnectionById(uint32_t nID);

        protected:
            // Function to be overriden by subclass

            /**
             * @brief Called when a client connects
             * @param client Client that connected
             * @return bool True if connection is allowed, false otherwise
             */
            virtual bool onClientConnect(std::shared_ptr<Connection<T>> client);

            /**
             * @brief Called when a client disconnects
             * @param client Client that disconnected
             */
            virtual void onClientDisconnect(std::shared_ptr<Connection<T>> client);

            /**
             * @brief Called when a message arrives
             * @param client Client that sent the message
             * @param msg Message that arrived
             */
            virtual void onMessage(std::shared_ptr<Connection<T>> client, Message<T>& msg);

        protected:
            TsQueue<OwnedMessage<T>> m_qMessagesIn;

            std::deque<std::shared_ptr<Connection<T>>> m_deqConnections;
            std::unordered_map<uint32_t, std::shared_ptr<Connection<T>>> m_mapConnections;

            asio::io_context m_asioContext;
            std::thread m_threadContext;

            /**
             * @brief Acceptor object to wait for new connections
             */
            asio::ip::tcp::acceptor m_asioAcceptor;

            uint32_t nIDCounter = 10000;
    };

    template<typename T>
    ServerInterface<T>::ServerInterface(uint16_t port)
        : m_asioAcceptor(m_asioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
    {
    }

    template<typename T>
    ServerInterface<T>::~ServerInterface()
    {
        stop();
    }

    template<typename T>
    bool ServerInterface<T>::start()
    {
        try {
            waitForClientConnection();

            m_threadContext = std::thread([this]() {
                m_asioContext.run();
            });
        } catch (std::exception& e) {
            std::cerr << "[SERVER] Exception: " << e.what() << "\n";
            return false;
        }

        std::cout << "[SERVER] Started!\n";
        return true;
    }

    template<typename T>
    void ServerInterface<T>::stop()
    {
        std::cout << "[SERVER] Stopping server...\n";
        m_asioContext.stop();

        if (m_threadContext.joinable()) m_threadContext.join();

        std::cout << "[SERVER] Stopped!\n";
    }

    template<typename T>
    void ServerInterface<T>::waitForClientConnection()
    {
        m_asioAcceptor.async_accept(
            [this](std::error_code ec, asio::ip::tcp::socket socket) {
                if (ec) {
                    std::cout << "[SERVER] New Connection Error: " << ec.message() << "\n";
                    waitForClientConnection();
                    return;
                }
                std::cout << "[SERVER] New Connection: " << socket.remote_endpoint() << "\n";

                std::shared_ptr<Connection<T>> newconn =
                    std::make_shared<Connection<T>>(Connection<T>::owner::server,
                        m_asioContext, std::move(socket), m_qMessagesIn);

                if (onClientConnect(newconn)) {
                    m_deqConnections.push_back(std::move(newconn));
                    m_mapConnections.emplace(nIDCounter, m_deqConnections.back());

                    m_deqConnections.back()->connectToClient(nIDCounter);

                    nIDCounter++;

                    std::cout << "[" << m_deqConnections.back()->getID() << "] Connection Approved\n";
                } else {
                    std::cout << "[-----] Connection Denied\n";
                }
                waitForClientConnection();
            });
    }

    template<typename T>
    void ServerInterface<T>::messageClient(std::shared_ptr<Connection<T>> client, const Message<T>& msg)
    {
        if (!client || !client->isConnected()) {
            onClientDisconnect(client);

            if (client)
                m_mapConnections.erase(client->getID());
            client.reset();
            m_deqConnections.erase(
                std::remove(m_deqConnections.begin(), m_deqConnections.end(), client), m_deqConnections.end());
            return;
        }
        client->send(msg);
    }

    template<typename T>
    void ServerInterface<T>::messageAllClients(const Message<T>& msg, std::shared_ptr<Connection<T>> pIgnoreClient)
    {
        bool bInvalidClientExists = false;

        for (auto& client: m_deqConnections) {
            if (!client || !client->isConnected()) {
                onClientDisconnect(client);

                if (client)
                    m_mapConnections.erase(client->getID());
                client.reset();

                bInvalidClientExists = true;
                continue;
            }
            if (client != pIgnoreClient)
                client->send(msg);
        }

        if (bInvalidClientExists) {
            m_deqConnections.erase(
                std::remove(m_deqConnections.begin(),
                    m_deqConnections.end(),
                    nullptr),
                m_deqConnections.end());
        }
    }

    template<typename T>
    void ServerInterface<T>::update(size_t nMaxMessages, bool bWait)
    {
        if (bWait) m_qMessagesIn.wait();

        size_t nMessageCount = 0;
        while (nMessageCount < nMaxMessages && !m_qMessagesIn.empty()) {
            auto msg = m_qMessagesIn.pop_front();

            onMessage(msg.remote, msg.msg);

            nMessageCount++;
        }
    }

    template<typename T>
    std::shared_ptr<Connection<T>> ServerInterface<T>::getConnectionById(uint32_t nID)
    {
        auto it = m_mapConnections.find(nID);
        if (it != m_mapConnections.end())
            return it->second;
        return nullptr;
    }

    template<typename T>
    bool ServerInterface<T>::onClientConnect(std::shared_ptr<Connection<T>> client)
    {
        return false;
    }

    template<typename T>
    void ServerInterface<T>::onClientDisconnect(std::shared_ptr<Connection<T>> client)
    {
    }

    template<typename T>
    void ServerInterface<T>::onMessage(std::shared_ptr<Connection<T>> client, Message<T>& msg)
    {
    }
}
