/*
** EPITECH PROJECT, 2023
** server
** File description:
** Message.hpp
*/

#pragma once

#include "common.hpp"

namespace rtype::net {
    /**
     * @brief Message Header is sent at start of all messages. The template allows us
     * to use "enum class" to ensure that the messages are valid at compile time
     *
     * @tparam T The enum class that represents the type of message
     */
    template<typename T>
    struct MessageHeader {
        T id{};
        uint32_t size = 0;
    };

    /**
     * @brief Message Body contains a header and a std::vector, containing raw bytes
     * of infomation. This way the message can be variable length, but the size
     * in the header must be updated.
     *
     * @tparam T The enum class that represents the type of message
     */
    template<typename T>
    struct Message {
        MessageHeader<T> header{};
        std::vector<uint8_t> body;

        /**
         * @brief returns size of entire message packet in bytes
         *
         * @return size_t
         */
        [[nodiscard]] size_t size() const;

        /**
         * @brief Override for std::cout compatibility - produces friendly description of message
         *
         * @param os The output stream
         * @param msg The message to be printed
         * @return std::ostream& The output stream
         */
        friend std::ostream& operator <<(std::ostream& os, const Message<T>& msg)
        {
            os << "ID:" << int(msg.header.id) << " Size:" << msg.header.size;
            return os;
        }

        /**
         * @brief Pushes any POD-like data into the message buffer
         *
         * @tparam DataType The data type of the data to be pushed
         * @param data The data to be pushed
         * @return Message<T>& The message with the data pushed
         */
        template<typename DataType>
        friend Message<T>& operator<<(Message<T>& msg, const DataType& data)
        {
            // Create a stream
            std::stringstream ss;

            // Create a boost archive from the stream
            boost::archive::binary_oarchive oa(ss);

            // Serialize the data into the archive
            oa << data;

            // Convert the serialized data to a vector of bytes and append it to the message body
            std::string s = ss.str();
            std::vector<uint8_t> bytes(s.begin(), s.end());
            msg.body.insert(msg.body.end(), bytes.begin(), bytes.end());

            // Update the message header size
            msg.header.size = msg.body.size();

            return msg;
        }

        /**
         * @brief Pulls any POD-like data form the message buffer
         *
         * @tparam DataType The data type of the data to be pulled
         * @param data The data to be pulled
         * @return Message<T>&
         */
        template<typename DataType>
        friend Message<T>& operator>>(Message<T>& msg, DataType& data)
        {
            // Create a stream from the message body
            std::string s(msg.body.begin(), msg.body.end());
            std::stringstream ss(s);

            // Create a boost archive from the stream
            boost::archive::binary_iarchive ia(ss);

            try {
                // Deserialize the data from the archive
                ia >> data;
            } catch (std::exception& e) {
                // TODO: find a better way of handling this error
                std::cerr << "ERROR When deserializing data!: " << e.what() << std::endl;
            }

            return msg;
        }

        template<class Archive>
        void serialize(Archive & ar, [[maybe_unused]] const unsigned int version) {
            ar & header;
            ar & body;
        }
    };

    template<typename T>
    size_t Message<T>::size() const
    {
        return body.size();
    }

    // Forward declare the connection
    template<typename T>
    class Connection;

    /**
     * @brief An "owned" message is identical to a regular message, but it is associated with
     * a connection. On a server, the owner would be the client that sent the message,
     * on a client the owner would be the server.
     *
     * @tparam T The enum class that represents the type of message
     */
    template<typename T>
    struct OwnedMessage {
        std::shared_ptr<Connection<T>> remote = nullptr;
        Message<T> msg;

        /**
         * @brief Override for std::cout compatibility - produces friendly description of message
         *
         * @param os The output stream
         * @param msg The message to be printed
         * @return std::ostream&
         */
        friend std::ostream& operator<<(std::ostream& os, const OwnedMessage<T>& msg)
        {
            os << msg.msg;
            return os;
        }
    };
}
