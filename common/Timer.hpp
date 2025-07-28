//// Timer.hpp ////////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for the timer class
//
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include <chrono>
#include <utility>

namespace nexo {

    struct ProfileResult {
        std::string name;
        long long time;
    };

    template<typename Fn>
    class Timer {
        public:
            explicit Timer(std::string name, Fn &&func) : m_name(std::move(name)), m_stopped(false), m_func(func)
            {
                m_start = std::chrono::high_resolution_clock::now();
            };

            ~Timer()
            {
                if (!m_stopped)
                    stop();
            }

            void stop()
            {
                const auto endTime = std::chrono::high_resolution_clock::now();
                const long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_start).
                        time_since_epoch().count();
                const long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTime).
                        time_since_epoch().count();

                m_stopped = true;
                const long long duration = (end - start) * 0.001f;

                m_func(m_func, duration);
            }

        private:
            std::string m_name;
            std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
            bool m_stopped;
            Fn m_func;
    };
}

