/// GenericAssetRef.hpp ///////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        20/11/2025
//  Description: Header file for the GenericAssetRef class.
//               An AssetRef is a reference to an asset, allowing for easy
//               access to the asset's data. The AssetData memory is handled
//               by the AssetCatalog, and the AssetRef is a lightweight
//               reference to the asset.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>
#include <utility>
#include <variant>

#include "AssetCatalog.hpp"
#include "AssetID.hpp"
#include "AssetLocation.hpp"
#include "Inline.hpp"

namespace nexo::assets {
    class IAsset;

    enum class AssetType;

    template<typename TAssetData, AssetType TAssetType>
    class Asset;

    template<typename TAsset>
    class AssetRef;

    /**
     * @brief A non-templated asset reference for generic asset storage
     */
    class GenericAssetRef {
       public:
        using AssetType       = IAsset;
        using AssetRefVariant = std::variant<AssetID, AssetLocation, std::weak_ptr<IAsset>>;

        GenericAssetRef() = default;

        explicit(false) GenericAssetRef(std::nullptr_t) : m_variant(std::weak_ptr<IAsset>{})
        {}
        explicit GenericAssetRef(const std::shared_ptr<IAsset>& ptr) : m_variant(ptr)
        {}
        explicit GenericAssetRef(const AssetLocation& location) : m_variant(location)
        {}
        explicit GenericAssetRef(const AssetID& id) : m_variant(id)
        {}

        // Standard copy/move
        GenericAssetRef(const GenericAssetRef&)                = default;
        GenericAssetRef& operator=(const GenericAssetRef&)     = default;
        GenericAssetRef(GenericAssetRef&&) noexcept            = default;
        GenericAssetRef& operator=(GenericAssetRef&&) noexcept = default;
        ~GenericAssetRef()                                     = default;

        GenericAssetRef& operator=(const std::shared_ptr<IAsset>& ptr);
        GenericAssetRef& operator=(std::nullptr_t);
        GenericAssetRef& operator=(const AssetRef<IAsset>& assetRef);

        // -------------------------------------------------------------------------
        // Public Interface (Base)
        // These simply call the implementation passing *this
        // -------------------------------------------------------------------------

        [[nodiscard]] bool isValid();
        [[nodiscard]] bool isValid() const;

        [[nodiscard]] std::shared_ptr<IAsset> lock();
        [[nodiscard]] std::shared_ptr<IAsset> lock() const;

        explicit operator bool() const;

        [[nodiscard]] static GenericAssetRef null();

        [[nodiscard]] bool operator==(const GenericAssetRef& other) const;
        [[nodiscard]] bool operator!=(const GenericAssetRef& other) const;
        [[nodiscard]] bool operator==(const std::nullptr_t) const;
        [[nodiscard]] bool operator!=(const std::nullptr_t) const;

        void load() const;
        void unload() const;

        [[nodiscard]] bool holdsWeakPtr() const;

        [[nodiscard]] bool holdsAssetID() const;

        [[nodiscard]] bool holdsAssetLocation() const;

        [[nodiscard]] bool NEXO_FORCE_INLINE isUnresolved() const;

        /**
         * @brief Cast to a typed asset reference
         * @tparam TAsset The asset type to cast to
         * @return A typed AssetRef
         */
        template<typename TAsset>
        [[nodiscard]] AssetRef<TAsset> as() const;

       protected:
        /**
         * @brief Base resolution logic.
         * Can be hidden or shadowed by derived classes.
         */
        template<typename Self>
        bool resolveWeakPtr(Self& self) const;

        // -------------------------------------------------------------------------
        // Template Implementation, called by derived classes
        // -------------------------------------------------------------------------

        template<typename Self>
        [[nodiscard]] bool impl_isValid(Self& self) const;

        template<typename Self>
        [[nodiscard]] std::shared_ptr<IAsset> impl_lock(Self& self) const;

        /**
         * @brief Internal variant holding either an AssetID, an AssetLocation or a weak_ptr to the asset
         * This allows for lazy loading of assets by storing only the AssetID/AssetLocation until the asset is needed.
         */
        mutable AssetRefVariant m_variant;
    };

    template<typename Self>
    bool GenericAssetRef::resolveWeakPtr(Self& self) const
    {
        using TargetType = typename Self::AssetType;

        if (self.holdsWeakPtr()) {
            return true;
        }

        GenericAssetRef catalogResult;
        switch (self.m_variant.index()) {
            case 0: // AssetID
                catalogResult = AssetCatalog::getInstance().getAsset(std::get<AssetID>(m_variant));
                break;
            case 1: // AssetLocation
                catalogResult = AssetCatalog::getInstance().getAsset(std::get<AssetLocation>(m_variant));
                break;
            default:
                return false;
        }

        if (!catalogResult.holdsWeakPtr()) {
            return false;
        }

        auto lockPtr = std::get<std::weak_ptr<IAsset>>(catalogResult.m_variant).lock();
        if (!lockPtr) {
            return false;
        }

        if constexpr (std::is_same_v<TargetType, IAsset>) {
            // CASE: GenericAssetRef
            self.m_variant = catalogResult.m_variant;
        } else {
            // CASE: AssetRef<T>
            if (auto castedPtr = std::dynamic_pointer_cast<TargetType>(lockPtr)) {
                self.m_variant = catalogResult.m_variant;
                return true;
            }
        }

        return false;
    }

    template<typename Self>
    bool GenericAssetRef::impl_isValid(Self& self) const
    {
        if (!self.resolveWeakPtr(self)) [[unlikely]]
            return false;

        return !std::get<std::weak_ptr<IAsset>>(m_variant).expired();
    }

    template<typename Self>
    std::shared_ptr<IAsset> GenericAssetRef::impl_lock(Self& self) const
    {
        if (!self.resolveWeakPtr(self)) [[unlikely]]
            return nullptr;

        return std::get<std::weak_ptr<IAsset>>(m_variant).lock();
    }

} // namespace nexo::assets
