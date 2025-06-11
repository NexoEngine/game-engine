//// CsClass.cpp //////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        05/06/2025
//  Description: Source file for the C# class representation in the scripting system
//
///////////////////////////////////////////////////////////////////////////////

#include "CsClass.hpp"

namespace nexo::scripting {
    std::unique_ptr<CsClass> CsClass::create(const std::shared_ptr<CsAssembly>& assembly, const HostString& namespaceName, const HostString& name)
    {
        return nullptr;
    }

    CsClass::CsClass(const std::shared_ptr<CsAssembly>& assembly, const HostString& namespaceName, const HostString& name)
    : m_namespace(namespaceName), m_name(name)
    {

    }


}
