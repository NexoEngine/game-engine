/// AssetRef.hpp /////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        24/11/2024
//  Description: Header file for the AssetRef class.
//               An AssetRef is a reference to an asset, allowing for easy
//               access to the asset's data.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Logger.hpp>
#include <memory>
#include <variant>

#include "AssetID.hpp"
#include "AssetLocation.hpp"
#include "GenericAssetRef.hpp"

namespace nexo::assets {
    class IAsset;

    /**
     * @brief Template class representing a lightweight reference to an asset.
     * @tparam TAsset The type of asset data being referenced
     */
    template<typename TAsset>
    class AssetRef final : public GenericAssetRef {
    public:
        using AssetType = TAsset;

        AssetRef() = default;
        explicit(false) AssetRef(std::nullptr_t) : GenericAssetRef(nullptr) {}
        explicit AssetRef(const std::shared_ptr<IAsset>& ptr) : GenericAssetRef(ptr) {}
        explicit AssetRef(const AssetLocation& location) : GenericAssetRef(location) {}
        explicit AssetRef(const AssetID& id) : GenericAssetRef(id) {}

        // Standard copy/move
        AssetRef(const AssetRef&)                = default;
        AssetRef& operator=(const AssetRef&)     = default;
        AssetRef(AssetRef&&) noexcept            = default;
        AssetRef& operator=(AssetRef&&) noexcept = default;
        ~AssetRef()                              = default;

        // -------------------------------------------------------------------------
        // Shadowing Base Methods
        // This is the magic: we redirect the call to the base template impl,
        // passing *this as the "Self" type.
        // -------------------------------------------------------------------------

        [[nodiscard]] bool isValid() { return impl_isValid(*this); }
        [[nodiscard]] bool isValid() const { return impl_isValid(*this); }

        // Lock needs a specific return type for the derived class, so we cast the result
        [[nodiscard]] std::shared_ptr<TAsset> lock() {
            return std::static_pointer_cast<TAsset>(impl_lock(*this));
        }

        [[nodiscard]] std::shared_ptr<TAsset> lock() const {
            return std::static_pointer_cast<TAsset>(impl_lock(*this));
        }

        [[nodiscard]] bool isLoaded() const {
            if (auto ptr = lock()) {
                return ptr->isLoaded();
            }
            return false;
        }

    };

    // -------------------------------------------------------------------------
    // Delayed Template Implementations
    // -------------------------------------------------------------------------

    template<typename TAsset>
    AssetRef<TAsset> GenericAssetRef::as() const
    {
        // We use the base lock() here, effectively doing a reinterpret/dynamic cast flow
        // But typically we want to check if the underlying ptr is valid first.
        // Since we are casting *to* a specific type, we create a new wrapper.

        // Check generic validity first without triggering specific type checks of current object
        // (because current object might be generic)
        if (!std::holds_alternative<std::weak_ptr<IAsset>>(m_variant)) {
            auto assetRef = AssetRef<TAsset>();
            assetRef.m_variant = m_variant;
            return assetRef;
        }


        // It is already a pointer
        auto ptr = std::get<std::weak_ptr<IAsset>>(m_variant).lock();
        if (!ptr) {
            return AssetRef<TAsset>();
        }

        // Try to cast
        auto typedPtr = std::dynamic_pointer_cast<TAsset>(ptr);
        if(typedPtr) {
            return AssetRef<TAsset>(typedPtr);
        }

        return AssetRef<TAsset>();
    }

} // namespace nexo::assets
