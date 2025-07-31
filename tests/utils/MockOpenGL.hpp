///////////////////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      [Mock OpenGL Context for Testing]
//  Date: Today lol
//  Description: Mock OpenGL context and functions for unit testing
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <glad/glad.h>
#include <gmock/gmock.h>
#include <map>
#include <vector>
#include <string>

namespace nexo::test {

// Mock OpenGL function implementations
class MockOpenGL {
public:
    static MockOpenGL& getInstance() {
        static MockOpenGL instance;
        return instance;
    }

    // Shader functions
    GLuint createShader(GLenum type) {
        static GLuint nextId = 1;
        GLuint id = nextId++;
        shaders[id] = ShaderInfo{type, "", false};
        return id;
    }

    void shaderSource(GLuint shader, GLsizei count, const GLchar** string, const GLint* length) {
        if (shaders.find(shader) != shaders.end()) {
            shaders[shader].source = std::string(*string);
        }
    }

    void compileShader(GLuint shader) {
        if (shaders.find(shader) != shaders.end()) {
            shaders[shader].compiled = true;
        }
    }

    void getShaderiv(GLuint shader, GLenum pname, GLint* params) {
        if (pname == GL_COMPILE_STATUS && shaders.find(shader) != shaders.end()) {
            *params = shaders[shader].compiled ? GL_TRUE : GL_FALSE;
        }
    }

    void deleteShader(GLuint shader) {
        shaders.erase(shader);
    }

    // Program functions
    GLuint createProgram() {
        static GLuint nextId = 1000;
        GLuint id = nextId++;
        programs[id] = ProgramInfo{};
        return id;
    }

    void attachShader(GLuint program, GLuint shader) {
        if (programs.find(program) != programs.end()) {
            programs[program].attachedShaders.push_back(shader);
        }
    }

    void linkProgram(GLuint program) {
        if (programs.find(program) != programs.end()) {
            programs[program].linked = true;
        }
    }

    void getProgramiv(GLuint program, GLenum pname, GLint* params) {
        if (pname == GL_LINK_STATUS && programs.find(program) != programs.end()) {
            *params = programs[program].linked ? GL_TRUE : GL_FALSE;
        }
    }

    void deleteProgram(GLuint program) {
        programs.erase(program);
    }

    void useProgram(GLuint program) {
        currentProgram = program;
    }

    // Buffer functions
    void genBuffers(GLsizei n, GLuint* buffers) {
        static GLuint nextId = 2000;
        for (GLsizei i = 0; i < n; i++) {
            buffers[i] = nextId++;
            this->buffers[buffers[i]] = BufferInfo{};
        }
    }

    void deleteBuffers(GLsizei n, const GLuint* buffers) {
        for (GLsizei i = 0; i < n; i++) {
            this->buffers.erase(buffers[i]);
        }
    }

    void bindBuffer(GLenum target, GLuint buffer) {
        if (target == GL_ARRAY_BUFFER) {
            currentArrayBuffer = buffer;
        } else if (target == GL_ELEMENT_ARRAY_BUFFER) {
            currentElementBuffer = buffer;
        }
    }

    void bufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage) {
        GLuint buffer = 0;
        if (target == GL_ARRAY_BUFFER) {
            buffer = currentArrayBuffer;
        } else if (target == GL_ELEMENT_ARRAY_BUFFER) {
            buffer = currentElementBuffer;
        }
        
        if (buffer != 0 && buffers.find(buffer) != buffers.end()) {
            buffers[buffer].size = size;
            buffers[buffer].usage = usage;
        }
    }

    // Texture functions
    void genTextures(GLsizei n, GLuint* textures) {
        static GLuint nextId = 3000;
        for (GLsizei i = 0; i < n; i++) {
            textures[i] = nextId++;
            this->textures[textures[i]] = TextureInfo{};
        }
    }

    void deleteTextures(GLsizei n, const GLuint* textures) {
        for (GLsizei i = 0; i < n; i++) {
            this->textures.erase(textures[i]);
        }
    }

    void bindTexture(GLenum target, GLuint texture) {
        if (target == GL_TEXTURE_2D) {
            currentTexture2D = texture;
        }
    }

    // Framebuffer functions
    void genFramebuffers(GLsizei n, GLuint* framebuffers) {
        static GLuint nextId = 4000;
        for (GLsizei i = 0; i < n; i++) {
            framebuffers[i] = nextId++;
            this->framebuffers[framebuffers[i]] = FramebufferInfo{};
        }
    }

    void deleteFramebuffers(GLsizei n, const GLuint* framebuffers) {
        for (GLsizei i = 0; i < n; i++) {
            this->framebuffers.erase(framebuffers[i]);
        }
    }

    void bindFramebuffer(GLenum target, GLuint framebuffer) {
        if (target == GL_FRAMEBUFFER) {
            currentFramebuffer = framebuffer;
        }
    }

    GLenum checkFramebufferStatus(GLenum target) {
        return GL_FRAMEBUFFER_COMPLETE;
    }

    // VAO functions
    void genVertexArrays(GLsizei n, GLuint* arrays) {
        static GLuint nextId = 5000;
        for (GLsizei i = 0; i < n; i++) {
            arrays[i] = nextId++;
            vaos[arrays[i]] = VAOInfo{};
        }
    }

    void deleteVertexArrays(GLsizei n, const GLuint* arrays) {
        for (GLsizei i = 0; i < n; i++) {
            vaos.erase(arrays[i]);
        }
    }

    void bindVertexArray(GLuint array) {
        currentVAO = array;
    }

    // Error handling
    GLenum getError() {
        GLenum error = lastError;
        lastError = GL_NO_ERROR;
        return error;
    }

    void setError(GLenum error) {
        lastError = error;
    }

    // Reset state for clean tests
    void reset() {
        shaders.clear();
        programs.clear();
        buffers.clear();
        textures.clear();
        framebuffers.clear();
        vaos.clear();
        currentProgram = 0;
        currentArrayBuffer = 0;
        currentElementBuffer = 0;
        currentTexture2D = 0;
        currentFramebuffer = 0;
        currentVAO = 0;
        lastError = GL_NO_ERROR;
    }

private:
    struct ShaderInfo {
        GLenum type;
        std::string source;
        bool compiled;
    };

    struct ProgramInfo {
        std::vector<GLuint> attachedShaders;
        bool linked = false;
    };

    struct BufferInfo {
        GLsizeiptr size = 0;
        GLenum usage = 0;
    };

    struct TextureInfo {
        GLsizei width = 0;
        GLsizei height = 0;
    };

    struct FramebufferInfo {
        std::map<GLenum, GLuint> attachments;
    };

    struct VAOInfo {
        std::map<GLuint, bool> enabledAttribs;
    };

    std::map<GLuint, ShaderInfo> shaders;
    std::map<GLuint, ProgramInfo> programs;
    std::map<GLuint, BufferInfo> buffers;
    std::map<GLuint, TextureInfo> textures;
    std::map<GLuint, FramebufferInfo> framebuffers;
    std::map<GLuint, VAOInfo> vaos;

    GLuint currentProgram = 0;
    GLuint currentArrayBuffer = 0;
    GLuint currentElementBuffer = 0;
    GLuint currentTexture2D = 0;
    GLuint currentFramebuffer = 0;
    GLuint currentVAO = 0;
    GLenum lastError = GL_NO_ERROR;
};

// Override OpenGL functions with mock implementations
#define MOCK_GL_FUNCTIONS \
    extern "C" { \
        GLuint glCreateShader(GLenum type) { return MockOpenGL::getInstance().createShader(type); } \
        void glShaderSource(GLuint shader, GLsizei count, const GLchar** string, const GLint* length) { MockOpenGL::getInstance().shaderSource(shader, count, string, length); } \
        void glCompileShader(GLuint shader) { MockOpenGL::getInstance().compileShader(shader); } \
        void glGetShaderiv(GLuint shader, GLenum pname, GLint* params) { MockOpenGL::getInstance().getShaderiv(shader, pname, params); } \
        void glDeleteShader(GLuint shader) { MockOpenGL::getInstance().deleteShader(shader); } \
        GLuint glCreateProgram() { return MockOpenGL::getInstance().createProgram(); } \
        void glAttachShader(GLuint program, GLuint shader) { MockOpenGL::getInstance().attachShader(program, shader); } \
        void glLinkProgram(GLuint program) { MockOpenGL::getInstance().linkProgram(program); } \
        void glGetProgramiv(GLuint program, GLenum pname, GLint* params) { MockOpenGL::getInstance().getProgramiv(program, pname, params); } \
        void glDeleteProgram(GLuint program) { MockOpenGL::getInstance().deleteProgram(program); } \
        void glUseProgram(GLuint program) { MockOpenGL::getInstance().useProgram(program); } \
        void glGenBuffers(GLsizei n, GLuint* buffers) { MockOpenGL::getInstance().genBuffers(n, buffers); } \
        void glDeleteBuffers(GLsizei n, const GLuint* buffers) { MockOpenGL::getInstance().deleteBuffers(n, buffers); } \
        void glBindBuffer(GLenum target, GLuint buffer) { MockOpenGL::getInstance().bindBuffer(target, buffer); } \
        void glBufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage) { MockOpenGL::getInstance().bufferData(target, size, data, usage); } \
        void glGenTextures(GLsizei n, GLuint* textures) { MockOpenGL::getInstance().genTextures(n, textures); } \
        void glDeleteTextures(GLsizei n, const GLuint* textures) { MockOpenGL::getInstance().deleteTextures(n, textures); } \
        void glBindTexture(GLenum target, GLuint texture) { MockOpenGL::getInstance().bindTexture(target, texture); } \
        void glGenFramebuffers(GLsizei n, GLuint* framebuffers) { MockOpenGL::getInstance().genFramebuffers(n, framebuffers); } \
        void glDeleteFramebuffers(GLsizei n, const GLuint* framebuffers) { MockOpenGL::getInstance().deleteFramebuffers(n, framebuffers); } \
        void glBindFramebuffer(GLenum target, GLuint framebuffer) { MockOpenGL::getInstance().bindFramebuffer(target, framebuffer); } \
        GLenum glCheckFramebufferStatus(GLenum target) { return MockOpenGL::getInstance().checkFramebufferStatus(target); } \
        void glGenVertexArrays(GLsizei n, GLuint* arrays) { MockOpenGL::getInstance().genVertexArrays(n, arrays); } \
        void glDeleteVertexArrays(GLsizei n, const GLuint* arrays) { MockOpenGL::getInstance().deleteVertexArrays(n, arrays); } \
        void glBindVertexArray(GLuint array) { MockOpenGL::getInstance().bindVertexArray(array); } \
        GLenum glGetError() { return MockOpenGL::getInstance().getError(); } \
    }

// Base class for tests that need mock OpenGL
class OpenGLTest : public ::testing::Test {
protected:
    void SetUp() override {
        MockOpenGL::getInstance().reset();
    }

    void TearDown() override {
        MockOpenGL::getInstance().reset();
    }
};

} // namespace nexo::test