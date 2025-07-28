//// Timestep.hpp /////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for the timestep class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

namespace nexo {
    class Timestep {
        public:
            explicit(false) Timestep(const double time = 0.0f) : m_time(time) {}

            explicit operator float() const { return m_time; }
            explicit operator double() const { return m_time; }

            [[nodiscard]] double getSeconds() const {return m_time; }
            [[nodiscard]] double getMilliseconds() const { return m_time * 1000.0f; }

        private:
            double m_time = 0.0f;
    };
}
