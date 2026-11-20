#pragma once

#include <memory>

namespace opensim_next::renderer {

// GLSL 1.20 program for an OpenGL 2.1 compatibility context.
// The class must be shut down while its owning GL context is current.
class OpenGLBasicShader final {
public:
    OpenGLBasicShader();
    ~OpenGLBasicShader();

    OpenGLBasicShader(const OpenGLBasicShader&) = delete;
    OpenGLBasicShader& operator=(const OpenGLBasicShader&) = delete;
    OpenGLBasicShader(OpenGLBasicShader&&) = delete;
    OpenGLBasicShader& operator=(OpenGLBasicShader&&) = delete;

    [[nodiscard]] bool initialize();
    void bind() const;
    void unbind() const;
    void shutdown() noexcept;
    [[nodiscard]] bool isReady() const noexcept;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace opensim_next::renderer
