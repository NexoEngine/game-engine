//// AssetImporterBase.hpp ////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        05/12/2024
//  Description: Header file for the AssetImporterBase class
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Asset.hpp"
#include "AssetImporterContext.hpp"
#include "AssetImporterInput.hpp"

#include <fstream>

namespace nexo::assets {

    /**
     * @class AssetImporterBase
     *
     * @brief Interface for importing assets into the engine.
     */
    class AssetImporterBase {
        public:
            /**
 * @brief Default constructor for AssetImporterBase.
 *
 * Constructs a new instance of AssetImporterBase with default initialization.
 */
AssetImporterBase() = default;
            /**
 * @brief Default virtual destructor for AssetImporterBase.
 *
 * This destructor ensures that derived asset importer objects are properly
 * destroyed, enabling safe and complete resource cleanup when an instance is
 * deleted through a base class pointer.
 */
virtual ~AssetImporterBase() = default;

            /**
             * @brief Checks if the importer can read the file at the given path.
             *
             * Implementations should open the file and check if related importer is compatible.
             * @param inputVariant The input variant to check. Can be a file path or memory buffer.
             * @return True if the importer can read the file, false otherwise.
             */
            virtual bool canRead(const ImporterInputVariant& inputVariant) = 0;

            /**
             * @brief Imports an asset from a file.
             *
             * This method should be overridden by the derived class to do the actual import.
             * @warning Implementation MUST set the main asset data using AssetImporterContext::setMainAssetData() before return
             *
             * @param ctx The context for the import.
             */
            virtual void importImpl(AssetImporterContext& ctx) = 0;

            /**
             * @brief Executes a safe asset import operation.
             *
             * This method wraps the implementation-specific import routine (importImpl()) to perform asset import while ensuring robust error handling. 
             * It invokes importImpl() with the provided context, then verifies that the main asset data has been set; if not, it logs an error. 
             * Any exceptions thrown during the import process are caught and logged along with the asset path from the context. 
             * This method is non-overridable and should remain unchanged in derived classes.
             *
             * @param ctx Import context carrying asset details and used for storing the main asset data.
             */
            void import(AssetImporterContext& ctx) noexcept
            {
                try {
                    importImpl(ctx);
                    if (ctx.getMainAsset() == nullptr) {
                        LOG(NEXO_ERROR, "Importer did not set main asset data in context");
                        return;
                    }
                } catch (const std::exception& e) {
                    // Log the error
                    LOG(NEXO_ERROR, "Failed to import asset from file '{}': {}", ctx.location.getPath(), e.what());
                }
            }
    };

} // namespace nexo::assets
