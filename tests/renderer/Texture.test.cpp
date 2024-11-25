//// Texture.test.cpp /////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        24/11/2024
//  Description: Test file for the texture class
//
//////////////////////////////////////////////////////////////////////////////
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <glad/glad.h>
#include <fstream>
#include <RendererExceptions.hpp>

#include "opengl/OpenGlTexture2D.hpp"
#include "renderer/Texture.hpp"
#include "contexts/opengl.hpp"

namespace nexo::renderer {
    class OpenGlTexture2DTest : public OpenGLTest {
        protected:
            std::string temporaryTextureFilePath = "test_texture.png";

            void createTemporaryTextureFile()
            {
                // Open a binary file for writing
                std::ofstream file(temporaryTextureFilePath, std::ios::binary);

                // Minimal PNG file: Signature, IHDR, IDAT, and IEND chunks
                const unsigned char pngData[] = {
                    // PNG signature
                    0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A,

                    // IHDR chunk
                    0x00, 0x00, 0x00, 0x0D, // Chunk length
                    0x49, 0x48, 0x44, 0x52, // Chunk type "IHDR"
                    0x00, 0x00, 0x00, 0x02, // Width: 2 pixels
                    0x00, 0x00, 0x00, 0x02, // Height: 2 pixels
                    0x08, // Bit depth: 8
                    0x02, // Color type: RGB
                    0x00, // Compression: Deflate
                    0x00, // Filter: None
                    0x00, // Interlace: None
                    0x5C, 0x72, 0xA8, 0x66, // CRC

                    // IDAT chunk (compressed image data for a 2x2 red image)
                    0x00, 0x00, 0x00, 0x0C, // Chunk length
                    0x49, 0x44, 0x41, 0x54, // Chunk type "IDAT"
                    0x78, 0x9C, // Zlib compression header
                    0x63, 0x60, 0xA0, 0x04, // Red pixel data (compressed)
                    0x00, 0x00, 0x05, 0x00, 0x01, // Zlib checksum
                    0x2C, 0x9F, 0xD3, 0x73, // CRC

                    // IEND chunk
                    0x00, 0x00, 0x00, 0x00, // Chunk length
                    0x49, 0x45, 0x4E, 0x44, // Chunk type "IEND"
                    0xAE, 0x42, 0x60, 0x82 // CRC
                };

                // Write the PNG data to the file
                file.write(reinterpret_cast<const char *>(pngData), sizeof(pngData));
                file.close();
            }

            void deleteTemporaryTextureFile()
            {
                std::remove(temporaryTextureFilePath.c_str());
            }
    };

    TEST_F(OpenGlTexture2DTest, CreateTextureFromDimensions) {
        OpenGlTexture2D texture1(256, 520);
        OpenGlTexture2D texture2(520, 256);

        // Validate that each texture is unique
        EXPECT_NE(texture1.getId(), texture2.getId());

        // Validate dimensions are correctly stored
        EXPECT_EQ(texture1.getWidth(), 256);
        EXPECT_EQ(texture1.getHeight(), 520);
        EXPECT_EQ(texture2.getWidth(), 520);
        EXPECT_EQ(texture2.getHeight(), 256);

        // Validate texture binding
        texture1.bind(0);
        GLint boundTexture = 0;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundTexture);
        EXPECT_EQ(static_cast<unsigned int>(boundTexture), texture1.getId());
        texture1.unbind();
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundTexture);
        EXPECT_EQ(static_cast<unsigned int>(boundTexture), 0);

        texture2.bind();
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundTexture);
        EXPECT_EQ(static_cast<unsigned int>(boundTexture), texture2.getId());
        texture2.unbind();
    }

    TEST_F(OpenGlTexture2DTest, CreateTextureFromFile) {
        //TODO: make this test with a real texture file
        // createTemporaryTextureFile();
        // OpenGlTexture2D texture(temporaryTextureFilePath);
        //
        // // Validate dimensions
        // EXPECT_GT(texture.getWidth(), 0);
        // EXPECT_GT(texture.getHeight(), 0);
        // deleteTemporaryTextureFile();
    }

    TEST_F(OpenGlTexture2DTest, CreateTextureFromInvalidFile) {
        EXPECT_THROW(OpenGlTexture2D texture("InvalidFile");, FileNotFoundException);
    }

    TEST_F(OpenGlTexture2DTest, SetDataValidSize) {
        unsigned int width = 128;
        unsigned int height = 128;
        OpenGlTexture2D texture(width, height);

        // Validate setting data with correct size
        std::vector<unsigned char> data(width * height * 4, 255); // RGBA white
        EXPECT_NO_THROW(texture.setData(data.data(), data.size()));

        // Validate that data is correctly uploaded
        texture.bind(0);
        std::vector<unsigned char> retrievedData(data.size(), 0);
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, retrievedData.data());

        EXPECT_EQ(data, retrievedData);
    }

    TEST_F(OpenGlTexture2DTest, SetDataInvalidSize) {
        unsigned int width = 128;
        unsigned int height = 128;
        OpenGlTexture2D texture(width, height);

        // Create invalid data (size mismatch)
        std::vector<unsigned char> invalidData(width * height * 3, 255); // Missing alpha channel
        EXPECT_THROW(texture.setData(invalidData.data(), invalidData.size()), TextureSizeMismatch);
    }

    TEST_F(OpenGlTexture2DTest, BindTextureToSlot) {
        unsigned int width = 64;
        unsigned int height = 64;
        OpenGlTexture2D texture(width, height);

        unsigned int slot = 5;
        texture.bind(slot);

        // Validate texture binding to specific slot
        GLint activeTexture = 0;
        glGetIntegerv(GL_ACTIVE_TEXTURE, &activeTexture);
        EXPECT_EQ(activeTexture, GL_TEXTURE0 + slot);

        GLint boundTexture = 0;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundTexture);
        EXPECT_EQ(static_cast<unsigned int>(boundTexture), texture.getId());
    }

    TEST_F(OpenGlTexture2DTest, TextureEqualityOperator) {
        unsigned int width = 64;
        unsigned int height = 64;
        OpenGlTexture2D texture1(width, height);
        OpenGlTexture2D texture2(width, height);

        // Validate equality operator
        EXPECT_FALSE(texture1 == texture2); // Different textures
        EXPECT_TRUE(texture1 == texture1); // Same texture
    }
}
