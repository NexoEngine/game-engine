//// Timestep.hpp /////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
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
            Timestep(const float time = 0.0f) : m_time(time) {};

            operator float() const {return m_time; };

            [[nodiscard]] float getSeconds() const {return m_time; };
            [[nodiscard]] float getMilliseconds() const { return m_time * 1000.0f; };

        private:
            float m_time = 0.0f;
    };
}
