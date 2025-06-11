//// CsAssembly.hpp ///////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        05/06/2025
//  Description: Header file for the C# assembly representation in the scripting system
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>

#include "scripting/native/HostString.hpp"

namespace nexo::scripting {

    class CsAssembly {
        public:
            static std::shared_ptr<CsAssembly> create(const HostString &assemblyPath);

            const HostString &getAssemblyPath() const { return m_assembly_path; }

            // Add more methods as needed for assembly representation

        private:
            explicit CsAssembly(const HostString &assemblyPath);

            HostString m_assembly_path;
    };

} // namespace nexo::scripting