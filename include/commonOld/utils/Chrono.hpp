/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Chrono
*/

#pragma once

#include <chrono>
#include "IClock.hpp"

#define NANO_TO_MILLI 1'000'000.0

namespace common::utils {
    class Chrono : public IClock {
        public:
            Chrono() { restart(); };
            ~Chrono() = default;
            void restart() {
                _start_time = _clock.now();
            };
            double getElapsedTime() const {
                auto elapsed_time_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(_clock.now() - _start_time).count();
                return static_cast<double>(elapsed_time_ns) / NANO_TO_MILLI;
            };
        private:
            std::chrono::high_resolution_clock _clock;
            std::chrono::high_resolution_clock::time_point _start_time;
    };
};
