//// AssetImporterInput.hpp ///////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        14/03/2025
//  Description: Header file for the importer's input structures
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <variant>
#include <vector>
#include <filesystem>

namespace nexo::assets {

    // Import from file, importer should open and read the file
    struct ImporterFileInput {
        std::filesystem::path filePath; //< File path to import
    };

    // Import from memory buffer, importer should read from the buffer
    struct ImporterMemoryInput {
        std::vector<uint8_t> memoryData;          //< Memory buffer
        std::optional<std::string> fileExtension; //< For format detection with memory sources (MUST start with a dot, e.g.: .png)
    };

    using ImporterInputVariant = std::variant<ImporterFileInput, ImporterMemoryInput>;

} // namespace nexo::assets
