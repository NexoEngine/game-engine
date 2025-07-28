//// AssetImporter.cpp ////////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
//
//  Author:      Guillaume HEIN
//  Date:        05/03/2025
//  Description: Registry for asset importers implementation
//
///////////////////////////////////////////////////////////////////////////////

#include "AssetImporter.hpp"
#include "AssetImporterBase.hpp"
#include "AssetCatalog.hpp"

#include "Assets/Model/Model.hpp"
#include "Assets/Model/ModelImporter.hpp"
#include "Assets/Texture/Texture.hpp"
#include "Assets/Texture/TextureImporter.hpp"

#include <boost/uuid/random_generator.hpp>

namespace nexo::assets {
    AssetImporter::AssetImporter()
    {
        registerImporter<Texture, TextureImporter>(100);
        registerImporter<Model, ModelImporter>(90);
    }

    AssetImporter::~AssetImporter()
    {
        for (const auto& importers: m_importers | std::views::values) {
            for (const auto& importer: importers) {
                delete importer;
            }
        }
    }

    GenericAssetRef AssetImporter::importAssetAuto(const AssetLocation& location, const ImporterInputVariant& inputVariant)
    {
        // Temp fix to use all importers in priority order
        // TODO: change the way we store importers, maybe stop using a map
        std::vector<AssetImporterBase *> allImporters;
        std::vector<ImporterDetails> allImportersDetails;
        for (const auto& typeIdx: m_importers | std::views::keys) {
            const auto& importers = m_importers.at(typeIdx);
            const auto& importerDetails = m_importersDetails.at(typeIdx);
            for (int importerIdx = 0; importerIdx < static_cast<int>(importers.size()); ++importerIdx) {
                const auto& details = importerDetails[importerIdx];
                const int priority = details.priority;
                size_t k = 0;
                for (; k < allImporters.size() && priority <= allImportersDetails[k].priority ; ++k);
                allImporters.insert(allImporters.begin() + static_cast<long>(k), importers[importerIdx]);
                allImportersDetails.insert(allImportersDetails.begin() + static_cast<long>(k), details);
            }
        }

        if (const auto asset = importAssetTryImporters(location, inputVariant, allImporters))
            return asset;
        return GenericAssetRef::null();
    }

    GenericAssetRef AssetImporter::importAssetUsingImporter(const AssetLocation& location,
        const ImporterInputVariant& inputVariant, AssetImporterBase* importer) const
    {
        AssetImporterContext* ctx = m_customCtx;
        AssetImporterContext ctxOnStack;
        if (!m_customCtx) {
            ctx = &ctxOnStack;
            ctx->input = inputVariant;
            ctx->location = location;
        }


        importer->import(*ctx);
        auto asset = ctx->releaseMainAsset();
        if (!asset)
            return GenericAssetRef::null();
        if (asset->getID().is_nil())
            asset->m_metadata.id = boost::uuids::random_generator()();
        if (asset->m_metadata.location == AssetLocation("default"))
            asset->m_metadata.location = location;

        return AssetCatalog::getInstance().registerAsset(location, std::move(asset));
    }

    GenericAssetRef AssetImporter::importAssetTryImporters(const AssetLocation& location,
        const ImporterInputVariant& inputVariant, const std::vector<AssetImporterBase*>& importers) const
    {
        std::vector<AssetImporterBase *> untriedImporters;
        for (const auto& importer : importers) {
            if (importer->canRead(inputVariant)) {
                auto asset = importAssetUsingImporter(location, inputVariant, importer);
                if (asset)
                    return asset;
            } else {
                untriedImporters.push_back(importer);
            }
        }
        // If "compatibles" importers failed, try even "incompatibles" ones
        for (const auto& importer : untriedImporters) {
            auto asset = importAssetUsingImporter(location, inputVariant, importer);
            if (asset)
                return asset;
        }
        return GenericAssetRef::null();
    }

    const std::vector<AssetImporterBase *>& AssetImporter::
    getImportersForType(const std::type_index& typeIdx) const
    {
        if (const auto it = m_importers.find(typeIdx) ; it == m_importers.end()) {
            static std::vector<AssetImporterBase *> empty;
            return empty;
        }
        return m_importers.at(typeIdx);
    }

    bool AssetImporter::hasImportersForType(const std::type_index& typeIdx) const
    {
        const auto it = m_importers.find(typeIdx);
        return (it != m_importers.end() && !it->second.empty());
    }

    void AssetImporter::unregisterAllImportersForType(const std::type_index& typeIdx)
    {
        m_importers.erase(typeIdx);
        m_importersDetails.erase(typeIdx);
    }

} // namespace nexo::assets
