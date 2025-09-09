//// HostString.hpp ///////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        04/05/2025
//  Description: char_t * string type wrapper for hostfxr
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <vector>
#include <iterator>

#include <hostfxr.h>

namespace nexo::scripting {

    class HostString {
        public:
            // Rule of Five with deep copy semantics
            HostString() = default;
            HostString(const HostString&) = default;
            HostString& operator=(const HostString&) = default;
            HostString(HostString&&) = default;
            HostString& operator=(HostString&&) = default;

            explicit HostString(nullptr_t) : m_buffer{}
            {}

            // Implicit constructors
            explicit(false) HostString(const std::string& utf8) {
                init_from_utf8(utf8);
            }
            explicit(false) HostString(const char *str) {
                init_from_utf8(str);
            }

            explicit(false) HostString(const std::wstring& wide) {
                init_from_wide(wide);
            }
            explicit(false) HostString(const wchar_t *str) {
                init_from_wide(str);
            }

            // Access raw char_t* for hostfxr APIs
            [[nodiscard]] const char_t* c_str() const noexcept {
                return m_buffer.data();
            }

            // Implicit conversions
            explicit(false) operator std::string() const {
                return to_utf8();
            }

            explicit(false) operator std::wstring() const {
                return to_wide();
            }

            [[nodiscard]] std::string to_utf8() const;
            [[nodiscard]] std::wstring to_wide() const;

            // Standard string interface
            [[nodiscard]] bool empty() const noexcept { return size() == 0; }
            [[nodiscard]] size_t size() const noexcept { return m_buffer.empty() ? 0 : m_buffer.size() - 1; }

            [[nodiscard]] char_t& operator[](const size_t index) noexcept { return m_buffer[index]; }
            [[nodiscard]] const char_t& operator[](const size_t index) const noexcept { return m_buffer[index]; }
            [[nodiscard]] char_t& at(const size_t index) noexcept { return m_buffer.at(index); }
            [[nodiscard]] const char_t& at(const size_t index) const noexcept { return m_buffer.at(index); }

            // Iterators
            [[nodiscard]] auto begin() noexcept { return m_buffer.begin(); }
            [[nodiscard]] auto begin() const noexcept { return m_buffer.begin(); }
            [[nodiscard]] auto cbegin() const noexcept { return begin(); }
            [[nodiscard]] auto end() noexcept { return m_buffer.end() - 1; } // Exclude null terminator
            [[nodiscard]] auto end() const noexcept { return m_buffer.end() - 1; } // Exclude null terminator
            [[nodiscard]] auto cend() const noexcept { return end(); }
            [[nodiscard]] auto rbegin() noexcept { return std::reverse_iterator(end()); }
            [[nodiscard]] auto rbegin() const noexcept { return std::reverse_iterator(end()); }
            [[nodiscard]] auto crbegin() const noexcept { return rbegin(); }
            [[nodiscard]] auto rend() noexcept { return std::reverse_iterator(begin()); }
            [[nodiscard]] auto rend() const noexcept { return std::reverse_iterator(begin()); }
            [[nodiscard]] auto crend() const noexcept { return rend(); }

            // Add operators
            HostString& operator+=(const HostString& other) {
                m_buffer.insert(end(), other.begin(), other.end());
                return *this;
            }

            HostString operator+(const HostString& other) const {
                HostString result = *this; // Copy current object
                result += other;
                return result;
            }

            // Comparison operators
            bool operator==(const HostString& other) const noexcept {
                return m_buffer == other.m_buffer;
            }

            bool operator!=(const HostString& other) const noexcept {
                return !(*this == other);
            }

        private:
            std::vector<char_t> m_buffer = {'\0'};

            void init_from_utf8(const std::string& utf8);
            void init_from_wide(const std::wstring& wide);
    };

} // namespace nexo::scripting
