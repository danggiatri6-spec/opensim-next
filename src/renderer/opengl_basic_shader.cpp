#include "opensim_next/renderer/opengl_basic_shader.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

#include <array>
#include <string>
#include <vector>

namespace opensim_next::renderer {
namespace {
using CreateShaderProc = GLuint (APIENTRY *)(GLenum);
using ShaderSourceProc = void (APIENTRY *)(GLuint, GLsizei, const GLchar* const*, const GLint*);
using CompileShaderProc = void (APIENTRY *)(GLuint);
using GetShaderivProc = void (APIENTRY *)(GLuint, GLenum, GLint*);
using GetShaderInfoLogProc = void (APIENTRY *)(GLuint, GLsizei, GLsizei*, GLchar*);
using DeleteShaderProc = void (APIENTRY *)(GLuint);
using CreateProgramProc = GLuint (APIENTRY *)();
using AttachShaderProc = void (APIENTRY *)(GLuint, GLuint);
using LinkProgramProc = void (APIENTRY *)(GLuint);
using GetProgramivProc = void (APIENTRY *)(GLuint, GLenum, GLint*);
using GetProgramInfoLogProc = void (APIENTRY *)(GLuint, GLsizei, GLsizei*, GLchar*);
using UseProgramProc = void (APIENTRY *)(GLuint);
using DeleteProgramProc = void (APIENTRY *)(GLuint);

constexpr GLenum kVertexShader = 0x8B31;
constexpr GLenum kFragmentShader = 0x8B30;
constexpr GLenum kCompileStatus = 0x8B81;
constexpr GLenum kLinkStatus = 0x8B82;
constexpr GLenum kInfoLogLength = 0x8B84;

struct ShaderFunctions {
    CreateShaderProc createShader{};
    ShaderSourceProc shaderSource{};
    CompileShaderProc compileShader{};
    GetShaderivProc getShaderiv{};
    GetShaderInfoLogProc getShaderInfoLog{};
    DeleteShaderProc deleteShader{};
    CreateProgramProc createProgram{};
    AttachShaderProc attachShader{};
    LinkProgramProc linkProgram{};
    GetProgramivProc getProgramiv{};
    GetProgramInfoLogProc getProgramInfoLog{};
    UseProgramProc useProgram{};
    DeleteProgramProc deleteProgram{};

    [[nodiscard]] bool load() {
#define LOAD_GL(member, symbol) member = reinterpret_cast<decltype(member)>(SDL_GL_GetProcAddress(symbol))
        LOAD_GL(createShader, "glCreateShader");
        LOAD_GL(shaderSource, "glShaderSource");
        LOAD_GL(compileShader, "glCompileShader");
        LOAD_GL(getShaderiv, "glGetShaderiv");
        LOAD_GL(getShaderInfoLog, "glGetShaderInfoLog");
        LOAD_GL(deleteShader, "glDeleteShader");
        LOAD_GL(createProgram, "glCreateProgram");
        LOAD_GL(attachShader, "glAttachShader");
        LOAD_GL(linkProgram, "glLinkProgram");
        LOAD_GL(getProgramiv, "glGetProgramiv");
        LOAD_GL(getProgramInfoLog, "glGetProgramInfoLog");
        LOAD_GL(useProgram, "glUseProgram");
        LOAD_GL(deleteProgram, "glDeleteProgram");
#undef LOAD_GL
        return createShader && shaderSource && compileShader && getShaderiv
            && getShaderInfoLog && deleteShader && createProgram && attachShader
            && linkProgram && getProgramiv && getProgramInfoLog && useProgram
            && deleteProgram;
    }
};

constexpr const char* kVertexSource = R"GLSL(#version 120
varying float vLight;
varying vec4 vColor;

void main()
{
    vec3 normalEye = normalize(gl_NormalMatrix * gl_Normal);
    vec3 lightDirection = normalize(vec3(0.35, -0.45, 0.82));
    float diffuse = max(dot(normalEye, lightDirection), 0.0);
    vLight = 0.28 + 0.72 * diffuse;
    vColor = gl_Color;
    gl_Position = ftransform();
}
)GLSL";

constexpr const char* kFragmentSource = R"GLSL(#version 120
varying float vLight;
varying vec4 vColor;

void main()
{
    gl_FragColor = vec4(vColor.rgb * vLight, vColor.a);
}
)GLSL";

GLuint compileShader(const ShaderFunctions& gl, GLenum type, const char* source) {
    const GLuint shader = gl.createShader(type);
    if (shader == 0) return 0;
    gl.shaderSource(shader, 1, &source, nullptr);
    gl.compileShader(shader);
    GLint status = 0;
    gl.getShaderiv(shader, kCompileStatus, &status);
    if (status == 0) {
        GLint length = 0;
        gl.getShaderiv(shader, kInfoLogLength, &length);
        std::vector<GLchar> log(static_cast<std::size_t>(length > 1 ? length : 1));
        GLsizei written = 0;
        gl.getShaderInfoLog(shader, static_cast<GLsizei>(log.size()), &written, log.data());
        SDL_Log("OpenSim-next GLSL compile failed: %s", log.data());
        gl.deleteShader(shader);
        return 0;
    }
    return shader;
}
} // namespace

struct OpenGLBasicShader::Impl {
    ShaderFunctions functions;
    GLuint program{};
    bool ready{false};
};

OpenGLBasicShader::OpenGLBasicShader() : impl_(std::make_unique<Impl>()) {}
OpenGLBasicShader::~OpenGLBasicShader() { shutdown(); }

bool OpenGLBasicShader::initialize() {
    if (!impl_ || impl_->ready || !impl_->functions.load()) return false;
    const GLuint vertex = compileShader(impl_->functions, kVertexShader, kVertexSource);
    if (vertex == 0) return false;
    const GLuint fragment = compileShader(impl_->functions, kFragmentShader, kFragmentSource);
    if (fragment == 0) {
        impl_->functions.deleteShader(vertex);
        return false;
    }

    const GLuint program = impl_->functions.createProgram();
    if (program == 0) {
        impl_->functions.deleteShader(vertex);
        impl_->functions.deleteShader(fragment);
        return false;
    }
    impl_->functions.attachShader(program, vertex);
    impl_->functions.attachShader(program, fragment);
    impl_->functions.linkProgram(program);
    impl_->functions.deleteShader(vertex);
    impl_->functions.deleteShader(fragment);

    GLint status = 0;
    impl_->functions.getProgramiv(program, kLinkStatus, &status);
    if (status == 0) {
        GLint length = 0;
        impl_->functions.getProgramiv(program, kInfoLogLength, &length);
        std::vector<GLchar> log(static_cast<std::size_t>(length > 1 ? length : 1));
        GLsizei written = 0;
        impl_->functions.getProgramInfoLog(program, static_cast<GLsizei>(log.size()),
                                           &written, log.data());
        SDL_Log("OpenSim-next GLSL link failed: %s", log.data());
        impl_->functions.deleteProgram(program);
        return false;
    }
    impl_->program = program;
    impl_->ready = true;
    return true;
}

void OpenGLBasicShader::bind() const {
    if (impl_ && impl_->ready) impl_->functions.useProgram(impl_->program);
}

void OpenGLBasicShader::unbind() const {
    if (impl_ && impl_->functions.useProgram) impl_->functions.useProgram(0);
}

void OpenGLBasicShader::shutdown() noexcept {
    if (!impl_) return;
    if (impl_->program != 0 && impl_->functions.deleteProgram) {
        impl_->functions.deleteProgram(impl_->program);
    }
    impl_->program = 0;
    impl_->ready = false;
    impl_->functions = {};
}

bool OpenGLBasicShader::isReady() const noexcept {
    return impl_ && impl_->ready;
}
} // namespace opensim_next::renderer
