//// GenericAssetRef.cpp //////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        24/11/2024
//  Description: Source file for the GenericAssetRef class
//
///////////////////////////////////////////////////////////////////////////////

#include "AssetRef.hpp"
#include "AssetCatalog.hpp"

namespace nexo::assets {

    bool GenericAssetRef::isValid()
    {
        return impl_isValid(*this);
    }

    bool GenericAssetRef::isValid() const
    {
        return impl_isValid(*this);
    }

    std::shared_ptr<IAsset> GenericAssetRef::lock()
    {
        return impl_lock(*this);
    }

    std::shared_ptr<IAsset> GenericAssetRef::lock() const
    {
        return impl_lock(*this);
    }

    GenericAssetRef::operator bool() const
    {
        return isValid();
    }

    GenericAssetRef GenericAssetRef::null()
    {
        return {};
    }

    bool GenericAssetRef::operator!=(const GenericAssetRef& other) const
    {
        return !(*this == other);
    }

    bool GenericAssetRef::operator==(const std::nullptr_t) const
    {
        return !isValid();
    }

    bool GenericAssetRef::operator!=(const std::nullptr_t) const
    {
        return isValid();
    }

    bool GenericAssetRef::holdsWeakPtr() const
    {
        return std::holds_alternative<std::weak_ptr<IAsset>>(m_variant);
    }

    bool GenericAssetRef::holdsAssetID() const
    {
        return std::holds_alternative<AssetID>(m_variant);
    }

    bool GenericAssetRef::holdsAssetLocation() const
    {
        return std::holds_alternative<AssetLocation>(m_variant);
    }

    GenericAssetRef& GenericAssetRef::operator=(const std::shared_ptr<IAsset>& ptr)
    {
        m_variant = ptr;
        return *this;
    }
    GenericAssetRef& GenericAssetRef::operator=(std::nullptr_t)
    {
        m_variant = std::weak_ptr<IAsset>{};
        return *this;
    }

    GenericAssetRef& GenericAssetRef::operator=(const AssetRef<IAsset>& assetRef)
    {
        m_variant = assetRef.m_variant;
        return *this;
    }

    bool GenericAssetRef::operator==(const GenericAssetRef& other) const
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

            default: return false;
        }
    }

    void GenericAssetRef::load() const
    {
        if (auto ptr = lock()) {
            // TODO: Implement reloadAsset in AssetCatalog if desired
            LOG(NEXO_WARN, "GenericAssetRef::load() no-op: resolution is typed and should be done via AssetRef<T>.");
        }
    }

    void GenericAssetRef::unload() const
    {
        if (auto ptr = lock()) {
            // TODO: Implement unloadAsset in AssetCatalog if desired
            LOG(NEXO_WARN, "GenericAssetRef::unload() no-op: resolution is typed and should be done via AssetRef<T>.");
        }
    }

    bool NEXO_FORCE_INLINE GenericAssetRef::isUnresolved() const
    {
        return !holdsWeakPtr();
    }

} // namespace nexo::assets
