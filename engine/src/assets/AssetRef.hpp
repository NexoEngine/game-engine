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

#include "Asset.hpp"
#include "AssetCatalogDecl.hpp"
#include "AssetID.hpp"
#include "AssetLocation.hpp"

namespace nexo::assets {
    class IAsset;

    template<typename TAsset>
    class AssetRef;

    using GenericAssetRef = AssetRef<IAsset>;

    /**
     * @brief Template class representing a lightweight reference to an asset.
     * @tparam TAsset The type of asset data being referenced
     */
    template<typename TAsset>
    class AssetRef final {
       public:
        friend class AssetRef;
        using AssetRefVariant = std::variant<AssetID, AssetLocation, std::weak_ptr<IAsset>>;

        AssetRef() : m_variant(std::weak_ptr<IAsset>{})
        {}
        explicit(false) AssetRef(std::nullptr_t) : m_variant(std::weak_ptr<IAsset>{})
        {}
        explicit AssetRef(const std::shared_ptr<IAsset>& ptr) : m_variant(ptr)
        {}
        explicit AssetRef(const AssetLocation& location) : m_variant(location)
        {}
        explicit AssetRef(const AssetID& id) : m_variant(id)
        {}

        // Standard copy/move
        // AssetRef(const AssetRef&)                = default;
        // AssetRef& operator=(const AssetRef&)     = default;
        // AssetRef(AssetRef&&) noexcept            = default;
        // AssetRef& operator=(AssetRef&&) noexcept = default;

        // Converting constructors (allow constructing AssetRef<T> from AssetRef<U> when U* is convertible to T*)
        template<typename U>
            requires std::is_convertible_v<U*, TAsset*>
        AssetRef(const AssetRef<U>& other) {
            m_variant = other.m_variant;
        }

        template<typename U>
            requires std::is_convertible_v<U*, TAsset*>
        AssetRef(AssetRef<U>&& other) noexcept {
            m_variant = std::move(other.m_variant);
        }

        ~AssetRef()                              = default;

        [[nodiscard]] bool operator==(const GenericAssetRef& other) const;
        [[nodiscard]] bool operator!=(const GenericAssetRef& other) const;
        [[nodiscard]] bool operator==(const std::nullptr_t) const;
        [[nodiscard]] bool operator!=(const std::nullptr_t) const;

        AssetRef& operator=(std::nullptr_t);
        AssetRef& operator=(const std::shared_ptr<TAsset>& ptr);

        // Converting assignment (Derived -> Base)
        template<typename U, typename = std::enable_if_t<std::is_convertible_v<U*, TAsset*>>>
        AssetRef& operator=(const AssetRef<U>& other);

        // Conversion to GenericAssetRef (implicit operator to help copy-initialization sites)
        operator GenericAssetRef() const noexcept {
            return GenericAssetRef(*this);
        }

        [[nodiscard]] bool isValid() const;

        explicit operator bool() const;

        [[nodiscard]] std::shared_ptr<TAsset> lock() const;

        [[nodiscard]] bool isLoaded() const;

        void load() const;
        void unload() const;

        /**
         * @brief Check if the reference currently holds a weak_ptr to the asset
         * @return True if it holds a weak_ptr, false otherwise
         */
        [[nodiscard]] bool holdsWeakPtr() const;

        /**
         * @brief Check if the reference currently holds an AssetID
         * @return True if it holds an AssetID, false otherwise
         */
        [[nodiscard]] bool holdsAssetID() const;

        /**
         * @brief Check if the reference currently holds an AssetLocation
         * @return True if it holds an AssetLocation, false otherwise
         */
        [[nodiscard]] bool holdsAssetLocation() const;

        /**
         * @brief Check if the asset reference is unresolved/cold (i.e., holds an AssetID or AssetLocation)
         * @return True if the asset reference is unresolved, false otherwise
         */
        [[nodiscard]] bool isUnresolved() const;

        /**
         * @brief Cast to a typed asset reference
         * @tparam OAsset The asset type to cast to
         * @return A typed AssetRef
         */
        template<typename OAsset>
        [[nodiscard]] AssetRef<OAsset> as() const;

       protected:
        bool resolveWeakPtr() const;

        mutable AssetRefVariant m_variant;
    };


    template<typename TAsset>
    bool AssetRef<TAsset>::operator!=(const GenericAssetRef& other) const
    {
        return !(*this == other);
    }

    template<typename TAsset>
    bool AssetRef<TAsset>::operator==(const std::nullptr_t) const
    {
        return !isValid();
    }

    template<typename TAsset>
    bool AssetRef<TAsset>::operator!=(const std::nullptr_t) const
    {
        return isValid();
    }

    template<typename TAsset>
    AssetRef<TAsset>& AssetRef<TAsset>::operator=(std::nullptr_t)
    {
        m_variant = std::weak_ptr<TAsset>{};
        return *this;
    }

    template<typename TAsset>
    AssetRef<TAsset>& AssetRef<TAsset>::operator=(const std::shared_ptr<TAsset>& ptr)
    {
        m_variant = ptr;
        return *this;
    }

    template<typename TAsset>
    template<typename U, typename>
    AssetRef<TAsset>& AssetRef<TAsset>::operator=(const AssetRef<U>& other)
    {
        m_variant = other.m_variant;
        return *this;
    }

    template<typename TAsset>
    bool AssetRef<TAsset>::isValid() const
    {
        if (!resolveWeakPtr()) [[unlikely]]
            return false;

        return !std::get<std::weak_ptr<IAsset>>(m_variant).expired();
    }

    template<typename TAsset>
    AssetRef<TAsset>::operator bool() const
    {
        return isValid();
    }

    template<typename TAsset>
    std::shared_ptr<TAsset> AssetRef<TAsset>::lock() const
    {
        if (!resolveWeakPtr()) [[unlikely]]
            return nullptr;

        const auto ptr = std::get<std::weak_ptr<IAsset>>(m_variant).lock();

        return std::static_pointer_cast<TAsset>(ptr);
    }

    template<typename TAsset>
    bool AssetRef<TAsset>::isLoaded() const
    {
        if (auto ptr = lock()) {
            return ptr->isLoaded();
        }
        return false;
    }

    template<typename TAsset>
    void AssetRef<TAsset>::load() const
    {
        if (auto ptr = lock()) {
            LOG(NEXO_INFO, "Loading asset '{}'", ptr->getMetadata().location.getFullLocation());
            LOG(NEXO_ERROR, "Asset loading not implemented yet!");
        }
    }

    template<typename TAsset>
    void AssetRef<TAsset>::unload() const
    {
        if (auto ptr = lock()) {
            LOG(NEXO_INFO, "Unloading asset '{}'", ptr->getMetadata().location.getFullLocation());
            LOG(NEXO_ERROR, "Asset unloading not implemented yet!");
        }
    }

    template<typename TAsset>
    bool AssetRef<TAsset>::holdsWeakPtr() const
    {
        return std::holds_alternative<std::weak_ptr<IAsset>>(m_variant);
    }

    template<typename TAsset>
    bool AssetRef<TAsset>::holdsAssetID() const
    {
        return std::holds_alternative<AssetID>(m_variant);
    }

    template<typename TAsset>
    bool AssetRef<TAsset>::holdsAssetLocation() const
    {
        return std::holds_alternative<AssetLocation>(m_variant);
    }

    template<typename TAsset>
    bool AssetRef<TAsset>::isUnresolved() const
    {
        return !holdsWeakPtr();
    }

    template<typename TAsset>
    template<typename OAsset>
    AssetRef<OAsset> AssetRef<TAsset>::as() const
    {
        if (!std::holds_alternative<std::weak_ptr<IAsset>>(m_variant)) {
            auto assetRef      = AssetRef<OAsset>();
            assetRef.m_variant = m_variant;
            return assetRef;
        }

        // It is already a pointer
        auto ptr = std::get<std::weak_ptr<IAsset>>(m_variant).lock();
        if (!ptr) {
            return AssetRef<OAsset>();
        }

        auto typedPtr = std::dynamic_pointer_cast<OAsset>(ptr);
        if (typedPtr) {
            return AssetRef<OAsset>(typedPtr);
        }

        return AssetRef<OAsset>();
    }

    template<typename TAsset>
    bool AssetRef<TAsset>::resolveWeakPtr() const
    {
        if (holdsWeakPtr()) {
            return true;
        }

        GenericAssetRef catalogResult;
        switch (m_variant.index()) {
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

        if constexpr (std::is_same_v<TAsset, IAsset>) {
            // CASE: GenericAssetRef
            m_variant = catalogResult.m_variant;
        } else {
            // CASE: AssetRef<T>
            if (auto castedPtr = std::dynamic_pointer_cast<TAsset>(lockPtr)) {
                m_variant = catalogResult.m_variant;
                return true;
            }
        }

        return false;
    }

    template<typename TAsset>
    bool AssetRef<TAsset>::operator==(const GenericAssetRef& other) const
    {
        if (this == &other) return true;

        if (m_variant.index() != other.m_variant.index()) return false;

        switch (m_variant.index()) {
            case 0: // AssetID
                return std::get<AssetID>(m_variant) == std::get<AssetID>(other.m_variant);

            case 1: // AssetLocation
                return std::get<AssetLocation>(m_variant) == std::get<AssetLocation>(other.m_variant);

            case 2: { // weak_ptr<IAsset>
                const auto& w1 = std::get<std::weak_ptr<IAsset>>(m_variant);
                const auto& w2 = std::get<std::weak_ptr<IAsset>>(other.m_variant);

                // Compare Control Block addresses directly.
                return !w1.owner_before(w2) && !w2.owner_before(w1);
            }

            default:
                return false;
        }
    }

} // namespace nexo::assets

#include "AssetRef.serializer.hpp"
