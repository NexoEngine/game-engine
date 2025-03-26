//// Shader.test.cpp //////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        23/11/2024
//  Description: Test file for the shader class
//
///////////////////////////////////////////////////////////////////////////////
#include "gtest/gtest.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>

#include "../utils/comparison.hpp"
#include "contexts/opengl.hpp"
#include "opengl/OpenGlShader.hpp"
#include "renderer/RendererExceptions.hpp"

namespace nexo::renderer {

    class ShaderTest : public OpenGLTest {
        protected:
            std::string vertexShaderSource = R"(
                #version 450 core
                layout(location = 0) in vec3 aPosition;
                uniform mat4 uModel;
                void main() {
                    gl_Position = uModel * vec4(aPosition, 1.0);
                }
            )";

            std::string fragmentShaderSource = R"(
                #version 450 core
                out vec4 color;
                uniform vec4 uColor;
                void main() {
                    color = uColor;
                }
            )";

            std::string temporaryShaderFilePath = "test_shader.glsl";

            void createTemporaryShaderFile(const std::string &content)
            {
                std::ofstream file(temporaryShaderFilePath);
                file << content;
                file.close();
            }

            void deleteTemporaryShaderFile()
            {
                std::remove(temporaryShaderFilePath.c_str());
            }
    };

    TEST_F(ShaderTest, ShaderCreationFromSource)
    {
        OpenGlShader shader("TestShader", vertexShaderSource, fragmentShaderSource);

        // Validate that the shader is bound
        EXPECT_NO_THROW(shader.bind());
        GLint currentProgram = 0;
        glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
        EXPECT_EQ(static_cast<GLuint>(currentProgram), shader.getProgramId());

        // Validate the unbinding
        shader.unbind();
        glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
        EXPECT_EQ(currentProgram, 0);
    }

    TEST_F(ShaderTest, ShaderCreationFromFile)
    {
        const std::string shaderFileContent = R"(
            #type vertex
            #version 450 core
            layout(location = 0) in vec3 aPosition;
            void main() {
                gl_Position = vec4(aPosition, 1.0);
            }
            #type fragment
            #version 450 core
            out vec4 color;
            void main() {
                color = vec4(1.0, 0.0, 0.0, 1.0);
            }
        )";

        createTemporaryShaderFile(shaderFileContent);

        OpenGlShader shader(temporaryShaderFilePath);

        // Validate that the shader is bound
        EXPECT_NO_THROW(shader.bind());
        GLint currentProgram = 0;
        glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
        EXPECT_EQ(static_cast<GLuint>(currentProgram), shader.getProgramId());

        // Validate that the shader is unbound
        shader.unbind();
        glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
        EXPECT_EQ(currentProgram, 0);

        deleteTemporaryShaderFile();
    }

    TEST_F(ShaderTest, InvalidShaderFile)
    {
        EXPECT_THROW(OpenGlShader("non_existing_file.glsl"), FileNotFoundException);
    }

    TEST_F(ShaderTest, InvalidShaderSource)
    {
        const std::string invalidShaderSource = R"(
            #type vertex
            #version 450 core
            layout(location = 0) in vec3 aPosition;
            void main() {
                gl_Position = vec4(aPosition  // Missing closing parenthesis
            }
        )";

        createTemporaryShaderFile(invalidShaderSource);

        // Validate shader compiling failure
        EXPECT_THROW(OpenGlShader shader(temporaryShaderFilePath), ShaderCreationFailed);

        deleteTemporaryShaderFile();
    }

    TEST_F(ShaderTest, SetUniforms)
    {
        // Vertex and fragment shader sources
        std::string vertexShaderSourceTestUniforms = R"(
            #version 450 core
            layout(location = 0) in vec3 aPosition;
            uniform float uFloat;
            uniform vec3 uVec3;
            uniform vec4 uVec4;
            uniform int uInt;
            uniform mat4 uModel;
            uniform sampler2D uIntArray[3];

            void main() {
                vec4 temp = uVec4 + vec4(uVec3, 1.0) * uFloat + vec4(float(uInt), 0.0, 0.0, 0.0);
                vec4 sampleSum = texture(uIntArray[0], vec2(0.5, 0.5)) +
                                 texture(uIntArray[1], vec2(0.5, 0.5)) +
                                 texture(uIntArray[2], vec2(0.5, 0.5));
                gl_Position = uModel * vec4(aPosition, 1.0) + temp + sampleSum;
            }
        )";
        std::string fragmentShaderSourceTestUniforms = R"(
            #version 450 core
            out vec4 color;
            void main() {
                color = vec4(1.0, 0.0, 0.0, 1.0);
            })";

        // Uniforms values
        constexpr unsigned int nbUniforms = 6;
        const char *uniformsName[nbUniforms] = {"uFloat", "uVec3", "uVec4", "uInt", "uModel", "uIntArray"};
        constexpr GLenum uniformsType[nbUniforms] = {GL_FLOAT, GL_FLOAT_VEC3, GL_FLOAT_VEC4, GL_INT, GL_FLOAT_MAT4, GL_SAMPLER_2D};
        constexpr int uniformsSize[nbUniforms] ={1, 1, 1, 1, 1, 3};

        OpenGlShader shader("TestShader", vertexShaderSourceTestUniforms, fragmentShaderSourceTestUniforms);
        shader.bind();

        /////////////////////// BASE UNIFORM CHECKUP ///////////////////////////

        // Validate number of uniforms
        GLint numUniforms = 0;
        glGetProgramiv(shader.getProgramId(), GL_ACTIVE_UNIFORMS, &numUniforms);
        EXPECT_EQ(numUniforms, nbUniforms);

        for (GLint i = 0; i < numUniforms; ++i) {
            char name[256];
            GLsizei length = 0;
            GLint size = 0;
            GLenum type = 0;

            glGetActiveUniform(shader.getProgramId(), i, sizeof(name), &length, &size, &type, name);

            // The uniform is not an array
            if (std::string(name).find('[') == std::string::npos) {
                // Validate name
                EXPECT_EQ(std::string(name), uniformsName[i]);
                // Validate size (should be always one)
                EXPECT_EQ(size, uniformsSize[i]);
                // Validate type
                EXPECT_EQ(type, uniformsType[i]);
            } else {
                // Retrieve the base name of the array
                std::string baseName = std::string(name).substr(0, std::string(name).find('['));
                // Validate name
                EXPECT_EQ(baseName, uniformsName[i]);
                // Validate size
                EXPECT_EQ(size, uniformsSize[i]);
                // Validate type
                EXPECT_EQ(type, uniformsType[i]);
            }
        }

        ////////////////////// ASSIGNING UNIFORM CHECKUP //////////////////////////

        ////// Validate float uniform //////
        const float floatValue = 1.0f;
        EXPECT_EQ(shader.setUniformFloat("uFloat", floatValue), 1);
        // Validate location retrieval
        GLint floatUniformLocation = glGetUniformLocation(shader.getProgramId(), "uFloat");
        ASSERT_NE(floatUniformLocation, -1); // Ensure the uniform location is valid
        // Validate uniform value
        GLfloat queriedFloatValue = 0.0f;
        glGetUniformfv(shader.getProgramId(), floatUniformLocation, &queriedFloatValue);
        EXPECT_EQ(queriedFloatValue, floatValue);

        ////// Validate float vec3 uniform //////
        glm::vec3 vec3Value(1.0f, 2.0f, 3.0f);
        EXPECT_EQ(shader.setUniformFloat3("uVec3", vec3Value), 1);
        // Validate location retrieval
        GLint vec3UniformLocation = glGetUniformLocation(shader.getProgramId(), "uVec3");
        ASSERT_NE(vec3UniformLocation, -1);
        // Validate uniform value
        GLfloat queriedVec3Value[3] = {0.0f};
        glGetUniformfv(shader.getProgramId(), vec3UniformLocation, queriedVec3Value);
        EXPECT_VEC3_NEAR(glm::vec3(queriedVec3Value[0], queriedVec3Value[1], queriedVec3Value[2]), vec3Value, 0.01f);

        ////// Validate float vec4 uniform //////
        glm::vec4 vec4Value(1.0f, 2.0f, 3.0f, 4.0f);
        EXPECT_EQ(shader.setUniformFloat4("uVec4", vec4Value), 1);
        // Validate location retrieval
        GLint vec4UniformLocation = glGetUniformLocation(shader.getProgramId(), "uVec4");
        ASSERT_NE(vec4UniformLocation, -1);
        // Validate uniform value
        GLfloat queriedVec4Value[4] = {0.0f};
        glGetUniformfv(shader.getProgramId(), vec4UniformLocation, queriedVec4Value);
        EXPECT_VEC4_NEAR(glm::vec4(queriedVec4Value[0], queriedVec4Value[1], queriedVec4Value[2], queriedVec4Value[3]),
                  vec4Value, 0.01f);

        ////// Validate float mat4x4 uniform //////
        glm::mat4 matrixValue(1.0f);
        EXPECT_EQ(shader.setUniformMatrix("uModel", matrixValue), 1);
        // Validate location retrieval
        GLint matrixUniformLocation = glGetUniformLocation(shader.getProgramId(), "uModel");
        ASSERT_NE(matrixUniformLocation, -1);
        // Validate uniform value
        GLfloat queriedMatrixValue[16] = {0.0f};
        glGetUniformfv(shader.getProgramId(), matrixUniformLocation, queriedMatrixValue);
        EXPECT_MAT4_NEAR(glm::make_mat4(queriedMatrixValue), matrixValue, 0.01f);

        ////// Validate int uniform //////
        const int intValue = 1;
        EXPECT_EQ(shader.setUniformInt("uInt", intValue), 1);
        // Validate location retrieval
        GLint intUniformLocation = glGetUniformLocation(shader.getProgramId(), "uInt");
        ASSERT_NE(intUniformLocation, -1);
        // Validate uniform value
        GLint queriedIntValue = 0;
        glGetUniformiv(shader.getProgramId(), intUniformLocation, &queriedIntValue);
        EXPECT_EQ(queriedIntValue, intValue);

        ////// Validate int array uniform //////
        const int intArrayValues[] = {1, 2, 3};
        EXPECT_EQ(shader.setUniformIntArray("uIntArray", intArrayValues, 3), 1);

        for (int i = 0; i < 3; ++i) {
            // Validate location retrieval
            std::string elementName = "uIntArray[" + std::to_string(i) + "]";
            GLint location = glGetUniformLocation(shader.getProgramId(), elementName.c_str());
            ASSERT_NE(location, -1);
            // Validate uniform value
            GLint queriedValue = 0;
            glGetUniformiv(shader.getProgramId(), location, &queriedValue);
            EXPECT_EQ(queriedValue, intArrayValues[i]) << "Mismatch for " << elementName;
        }
        shader.unbind();
    }

    TEST_F(ShaderTest, GetShaderName)
    {
        OpenGlShader shader("TestShader", vertexShaderSource, fragmentShaderSource);
        EXPECT_EQ(shader.getName(), "TestShader");
    }

    TEST_F(ShaderTest, InvalidUniformName)
    {
        OpenGlShader shader("TestShader", vertexShaderSource, fragmentShaderSource);
        shader.bind();

        // Validate failure on invalid uniform name
        EXPECT_EQ(shader.setUniformFloat("invalidUniformFloat", 1.0f), 0);
        EXPECT_EQ(shader.setUniformFloat3("invalidUniformFloat3", glm::vec3(1.0f)), 0);
        EXPECT_EQ(shader.setUniformFloat4("invalidUniformFloat4", glm::vec4(1.0f)), 0);
        EXPECT_EQ(shader.setUniformInt("invalidUniformInt", 1), 0);
        EXPECT_EQ(shader.setUniformMatrix("invalidUniformFloatMatrix4x4", glm::mat4(1.0f)), 0);
        int testArray[] = {1, 2, 3};
        EXPECT_EQ(shader.setUniformIntArray("invalidUniformIntArray", testArray, 3), 0);
        shader.unbind();
    }

}
