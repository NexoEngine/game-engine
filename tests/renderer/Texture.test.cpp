//// Texture.test.cpp /////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
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
#include "flattenedAssets/testLogo.hpp"

namespace nexo::renderer {
    class OpenGlTexture2DTest : public OpenGLTest {
        protected:
            std::string temporaryTextureFilePath = "test_texture.png";

            void createTemporaryTextureFile()
            {
                std::ofstream file(temporaryTextureFilePath, std::ios::binary);
                file.write(reinterpret_cast<const char *>(testLogo.data()), testLogo.size());
                file.close();
            }

            void deleteTemporaryTextureFile()
            {
                std::remove(temporaryTextureFilePath.c_str());
            }
    };

    TEST_F(OpenGlTexture2DTest, CreateTextureFromDimensions) {
        NxOpenGlTexture2D texture1(256, 520);
        NxOpenGlTexture2D texture2(520, 256);

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
        createTemporaryTextureFile();
        NxOpenGlTexture2D texture(temporaryTextureFilePath);

        // Validate dimensions
        std::cout << texture.getWidth() << std::endl;
        std::cout << texture.getHeight() << std::endl;
        EXPECT_GT(texture.getWidth(), 0);
        EXPECT_GT(texture.getHeight(), 0);
        deleteTemporaryTextureFile();
    }

    TEST_F(OpenGlTexture2DTest, CreateTextureFromInvalidFile) {
        EXPECT_THROW(NxOpenGlTexture2D texture("InvalidFile");, NxFileNotFoundException);
    }

    TEST_F(OpenGlTexture2DTest, SetDataValidSize) {
        unsigned int width = 128;
        unsigned int height = 128;
        NxOpenGlTexture2D texture(width, height);

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
        NxOpenGlTexture2D texture(width, height);

        // Create invalid data (size mismatch)
        std::vector<unsigned char> invalidData(width * height * 3, 255); // Missing alpha channel
        EXPECT_THROW(texture.setData(invalidData.data(), invalidData.size()), NxTextureSizeMismatch);
    }

    TEST_F(OpenGlTexture2DTest, BindTextureToSlot) {
        unsigned int width = 64;
        unsigned int height = 64;
        NxOpenGlTexture2D texture(width, height);

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
        NxOpenGlTexture2D texture1(width, height);
        NxOpenGlTexture2D texture2(width, height);

        // Validate equality operator
        EXPECT_FALSE(texture1 == texture2); // Different textures
        EXPECT_TRUE(texture1 == texture1); // Same texture
    }
}
