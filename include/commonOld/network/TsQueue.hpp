/*
** EPITECH PROJECT, 2023
** server
** File description:
** TsQueue.hpp
*/

#pragma once

#include "common.hpp"

namespace rtype::net {
    template<typename T>
    class TsQueue {
        public:
            TsQueue() = default;

            TsQueue(const TsQueue<T>&) = delete;

            virtual ~TsQueue();

        public:
            /**
             * @brief Returns and maintains item at front of Queue
             * @return const T&
             */
            const T& front();

            /**
             * @brief Returns and maintains item at back of Queue
             * @return const T&
             */
            const T& back();

            /**
             * @brief Removes and returns item from front of Queue
             * @return T
             */
            T pop_front();

            /**
             * @brief Removes and returns item from back of Queue
             * @return T
             */
            T pop_back();

            /**
             * @brief Adds an item to back of Queue
             * @param item
             */
            void push_back(const T& item);

            /**
             * @brief Adds an item to front of Queue
             * @param item
             */
            void push_front(const T& item);

            /**
             * @brief Returns true if Queue has no items
             * @return bool
             */
            bool empty();

            /**
             * @brief Returns number of items in Queue
             * @return size_t
             */
            size_t count();

            /**
             * @brief Clears Queue
             */
            void clear();

            /**
             * @brief Make thread sleep until item added to Queue
             */
            void wait();

            /**
             * @brief Make thread sleep until Queue is empty
             */
            void waitUntilEmpty();

        protected:
            std::mutex muxQueue;
            std::deque<T> deqQueue;
            std::condition_variable cvBlocking;
            std::mutex muxBlocking;
    };

    template<typename T>
    TsQueue<T>::~TsQueue()
    {
        clear();
    }

    template<typename T>
    const T& TsQueue<T>::front()
    {
        std::scoped_lock lock(muxQueue);
        return deqQueue.front();
    }

    template<typename T>
    const T& TsQueue<T>::back()
    {
        std::scoped_lock lock(muxQueue);
        return deqQueue.back();
    }

    template<typename T>
    T TsQueue<T>::pop_front()
    {
        std::scoped_lock lock(muxQueue);
        auto t = std::move(deqQueue.front());
        deqQueue.pop_front();
        return t;
    }

    template<typename T>
    T TsQueue<T>::pop_back()
    {
        std::scoped_lock lock(muxQueue);
        auto t = std::move(deqQueue.back());
        deqQueue.pop_back();
        return t;
    }

    template<typename T>
    void TsQueue<T>::push_back(const T& item)
    {
        std::scoped_lock lock(muxQueue);
        deqQueue.emplace_back(std::move(item));

        std::unique_lock<std::mutex> ul(muxBlocking);
        cvBlocking.notify_one();
    }

    template<typename T>
    void TsQueue<T>::push_front(const T& item)
    {
        std::scoped_lock lock(muxQueue);
        deqQueue.emplace_front(std::move(item));

        std::unique_lock<std::mutex> ul(muxBlocking);
        cvBlocking.notify_one();
    }

    template<typename T>
    bool TsQueue<T>::empty()
    {
        std::scoped_lock lock(muxQueue);
        return deqQueue.empty();
    }

    template<typename T>
    size_t TsQueue<T>::count()
    {
        std::scoped_lock lock(muxQueue);
        return deqQueue.size();
    }

    template<typename T>
    void TsQueue<T>::clear()
    {
        std::scoped_lock lock(muxQueue);
        deqQueue.clear();
    }

    template<typename T>
    void TsQueue<T>::wait()
    {
        while (empty()) {
            std::unique_lock<std::mutex> ul(muxBlocking);
            cvBlocking.wait(ul);
        }
    }

    template<typename T>
    void TsQueue<T>::waitUntilEmpty()
    {
        while (!empty()) {
            std::unique_lock<std::mutex> ul(muxBlocking);
            cvBlocking.wait(ul);
        }
    }
}
