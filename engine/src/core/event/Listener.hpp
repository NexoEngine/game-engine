//// Listener.hpp /////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for the listener class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <iostream>
#include <utility>

namespace nexo::event {
    class AEvent;
}

namespace nexo::event {

    class BaseListener {
        public:
            BaseListener() = default;
            explicit BaseListener(std::string name) : m_name(std::move(name)) {};
            virtual ~BaseListener() = default;

            [[nodiscard]] std::string getListenerName() const {return m_name; };
        private:
            std::string m_name;
    };

    template<class T>
    class Listener {
        public:
            virtual ~Listener() = default;

            virtual void handleEvent(T &) = 0;
    };

    template<typename... EventTypes>
    class Listens : public BaseListener, public Listener<EventTypes>... {
        public:
            explicit Listens(const std::string &name = "") : BaseListener(name) {};
    };
}
