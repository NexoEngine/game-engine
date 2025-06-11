//// CsClass.hpp //////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        05/06/2025
//  Description: Header file for the C# class representation in the scripting system
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>

#include "scripting/native/HostString.hpp"

#include "CsAssembly.hpp"

namespace nexo::scripting {

    class CsClass {
        public:
            static std::shared_ptr<CsClass> create(const std::shared_ptr<CsAssembly>& assembly, const HostString &namespaceName, const HostString &name);



            const HostString &getNamespace() const { return m_namespace; }
            const HostString &getName() const { return m_name; }

            // Add more methods as needed for class representation


        private:
            explicit CsClass(const std::shared_ptr<CsAssembly>& assembly, const HostString &namespaceName, const HostString &name);

            HostString m_namespace;
            HostString m_name;
    };

} // namespace nexo::scripting