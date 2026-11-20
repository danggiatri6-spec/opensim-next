#include "opensim_next/platform/preview_window.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <cstddef>
#include <cstdint>
#include <unordered_map>

namespace opensim_next::platform {
namespace {
struct Vec3 { double x, y, z; };
Vec3 cross(const Vec3& a, const Vec3& b) {
    return {a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y-a.y*b.x};
}
Vec3 normalize(const Vec3& v) {
    const double n=std::sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
    return n<1.0e-9 ? Vec3{0,1,0} : Vec3{v.x/n,v.y/n,v.z/n};
}
void applyCamera(const renderer::CameraState& c) {
    const Vec3 eye{c.position.x,c.position.y,c.position.z};
    const Vec3 forward=normalize({c.forward.x,c.forward.y,c.forward.z});
    const Vec3 side=normalize(cross(forward,{c.up.x,c.up.y,c.up.z}));
    const Vec3 up=cross(side,forward);
    const GLdouble view[16]={
        side.x,up.x,-forward.x,0,
        side.y,up.y,-forward.y,0,
        side.z,up.z,-forward.z,0,
        -(side.x*eye.x+side.y*eye.y+side.z*eye.z),
        -(up.x*eye.x+up.y*eye.y+up.z*eye.z),
        forward.x*eye.x+forward.y*eye.y+forward.z*eye.z,1
    };
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(view);
}
constexpr GLenum kArrayBuffer = 0x8892;
constexpr GLenum kElementArrayBuffer = 0x8893;
constexpr GLenum kStaticDraw = 0x88E4;

using GenBuffersProc = void (APIENTRYP)(GLsizei, GLuint*);
using DeleteBuffersProc = void (APIENTRYP)(GLsizei, const GLuint*);
using BindBufferProc = void (APIENTRYP)(GLenum, GLuint);
using BufferDataProc = void (APIENTRYP)(GLenum, std::ptrdiff_t, const void*, GLenum);

struct OpenGLBufferFunctions {
    GenBuffersProc* genBuffers{};
    DeleteBuffersProc* deleteBuffers{};
    BindBufferProc* bindBuffer{};
    BufferDataProc* bufferData{};

    [[nodiscard]] bool load() {
        genBuffers = reinterpret_cast<GenBuffersProc*>(SDL_GL_GetProcAddress("glGenBuffers"));
        deleteBuffers = reinterpret_cast<DeleteBuffersProc*>(SDL_GL_GetProcAddress("glDeleteBuffers"));
        bindBuffer = reinterpret_cast<BindBufferProc*>(SDL_GL_GetProcAddress("glBindBuffer"));
        bufferData = reinterpret_cast<BufferDataProc*>(SDL_GL_GetProcAddress("glBufferData"));
        return genBuffers && deleteBuffers && bindBuffer && bufferData;
    }
};

struct GpuMesh {
    GLuint vertexBuffer{};
    GLuint indexBuffer{};
    GLsizei indexCount{};
};

class GpuMeshCache final {
public:
    explicit GpuMeshCache(OpenGLBufferFunctions& functions) : functions_(functions) {}

    [[nodiscard]] bool draw(const renderer::MeshData& mesh) {
        if (!mesh.isValid()) return false;
        auto found = meshes_.find(&mesh);
        if (found == meshes_.end()) {
            GpuMesh gpu;
            functions_.genBuffers(1, &gpu.vertexBuffer);
            functions_.genBuffers(1, &gpu.indexBuffer);
            if (gpu.vertexBuffer == 0 || gpu.indexBuffer == 0) {
                release(gpu);
                return false;
            }
            functions_.bindBuffer(kArrayBuffer, gpu.vertexBuffer);
            functions_.bufferData(kArrayBuffer,
                static_cast<std::ptrdiff_t>(mesh.vertices.size() * sizeof(renderer::MeshVertex)),
                mesh.vertices.data(), kStaticDraw);
            functions_.bindBuffer(kElementArrayBuffer, gpu.indexBuffer);
            functions_.bufferData(kElementArrayBuffer,
                static_cast<std::ptrdiff_t>(mesh.indices.size() * sizeof(std::uint32_t)),
                mesh.indices.data(), kStaticDraw);
            gpu.indexCount = static_cast<GLsizei>(mesh.indices.size());
            found = meshes_.emplace(&mesh, gpu).first;
        }

        const GpuMesh& gpu = found->second;
        functions_.bindBuffer(kArrayBuffer, gpu.vertexBuffer);
        functions_.bindBuffer(kElementArrayBuffer, gpu.indexBuffer);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        constexpr GLsizei stride = static_cast<GLsizei>(sizeof(renderer::MeshVertex));
        glVertexPointer(3, GL_FLOAT, stride,
            reinterpret_cast<const void*>(offsetof(renderer::MeshVertex, position)));
        glNormalPointer(GL_FLOAT, stride,
            reinterpret_cast<const void*>(offsetof(renderer::MeshVertex, normal)));
        glTexCoordPointer(2, GL_FLOAT, stride,
            reinterpret_cast<const void*>(offsetof(renderer::MeshVertex, uv)));
        glDrawElements(GL_TRIANGLES, gpu.indexCount, GL_UNSIGNED_INT, nullptr);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
        functions_.bindBuffer(kArrayBuffer, 0);
        functions_.bindBuffer(kElementArrayBuffer, 0);
        return true;
    }

    void clear() {
        for (auto& entry : meshes_) release(entry.second);
        meshes_.clear();
    }

private:
    void release(const GpuMesh& mesh) {
        if (mesh.vertexBuffer != 0) functions_.deleteBuffers(1, &mesh.vertexBuffer);
        if (mesh.indexBuffer != 0) functions_.deleteBuffers(1, &mesh.indexBuffer);
    }

    OpenGLBufferFunctions& functions_;
    std::unordered_map<const renderer::MeshData*, GpuMesh> meshes_;
};
void drawUnitCube() {
    glBegin(GL_QUADS);
    glNormal3f(0,0,1);
    glVertex3f(-.5F,-.5F,.5F); glVertex3f(.5F,-.5F,.5F); glVertex3f(.5F,.5F,.5F); glVertex3f(-.5F,.5F,.5F);
    glNormal3f(0,0,-1);
    glVertex3f(-.5F,-.5F,-.5F); glVertex3f(-.5F,.5F,-.5F); glVertex3f(.5F,.5F,-.5F); glVertex3f(.5F,-.5F,-.5F);
    glNormal3f(0,1,0);
    glVertex3f(-.5F,.5F,-.5F); glVertex3f(-.5F,.5F,.5F); glVertex3f(.5F,.5F,.5F); glVertex3f(.5F,.5F,-.5F);
    glNormal3f(0,-1,0);
    glVertex3f(-.5F,-.5F,-.5F); glVertex3f(.5F,-.5F,-.5F); glVertex3f(.5F,-.5F,.5F); glVertex3f(-.5F,-.5F,.5F);
    glNormal3f(1,0,0);
    glVertex3f(.5F,-.5F,-.5F); glVertex3f(.5F,.5F,-.5F); glVertex3f(.5F,.5F,.5F); glVertex3f(.5F,-.5F,.5F);
    glNormal3f(-1,0,0);
    glVertex3f(-.5F,-.5F,-.5F); glVertex3f(-.5F,-.5F,.5F); glVertex3f(-.5F,.5F,.5F); glVertex3f(-.5F,.5F,-.5F);
    glEnd();
}
void setProjection(int width,int height) {
    const int w=std::max(width,1),h=std::max(height,1);
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    constexpr double nearPlane=.1,farPlane=2048.0,halfHeight=.05773502691896257;
    const double halfWidth=halfHeight*static_cast<double>(w)/h;
    glFrustum(-halfWidth,halfWidth,-halfHeight,halfHeight,nearPlane,farPlane);
    glMatrixMode(GL_MODELVIEW);
}
void renderWorldItems(std::span<const renderer::DrawItem> items, GpuMeshCache& meshCache) {
    for(const auto& item:items) {
        if(!item.mesh || !item.material) continue;
        glPushMatrix();
        glTranslated(item.transform.position.x,item.transform.position.y,item.transform.position.z);
        const auto& q=item.transform.rotation;
        const double qw=std::clamp(q.w,-1.0,1.0);
        const double angle=2.0*std::acos(qw)*57.29577951308232;
        const double s=std::sqrt(std::max(0.0,1.0-qw*qw));
        if(s>1.0e-8) glRotated(angle,q.x/s,q.y/s,q.z/s);
        glScaled(item.transform.scale.x,item.transform.scale.y,item.transform.scale.z);
        const unsigned int hash=static_cast<unsigned int>(item.entityId*2654435761U);
        glColor3f(.35F+((hash>>16U)&255U)/510.0F,
                  .35F+((hash>>8U)&255U)/510.0F,
                  .35F+(hash&255U)/510.0F);
        if (item.geometry && item.geometry->isValid()) {
            (void)meshCache.draw(*item.geometry);
        } else {
            drawUnitCube();
        }
        glPopMatrix();
    }
}
} // namespace

int PreviewWindow::run(std::span<const renderer::DrawItem> items,
                       renderer::CameraController& camera) const {
    if(!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr<<"SDL initialization failed: "<<SDL_GetError()<<'\n'; return 1;
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,24);
    SDL_Window* window=SDL_CreateWindow("OpenSim-next PC Viewer - World Preview",1280,720,
                                        SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
    if(!window) { std::cerr<<"Window creation failed: "<<SDL_GetError()<<'\n'; SDL_Quit(); return 1; }
    SDL_GLContext context=SDL_GL_CreateContext(window);
    if(!context) { std::cerr<<"OpenGL context creation failed: "<<SDL_GetError()<<'\n'; SDL_DestroyWindow(window); SDL_Quit(); return 1; }
    SDL_GL_SetSwapInterval(1);
    OpenGLBufferFunctions bufferFunctions;
    if (!bufferFunctions.load()) {
        std::cerr << "OpenGL vertex-buffer functions are unavailable on this driver.\\n";
        SDL_GL_DestroyContext(context); SDL_DestroyWindow(window); SDL_Quit(); return 1;
    }
    GpuMeshCache meshCache(bufferFunctions);
    glEnable(GL_DEPTH_TEST); glDisable(GL_CULL_FACE);
    glEnable(GL_LIGHTING); glEnable(GL_LIGHT0); glEnable(GL_COLOR_MATERIAL);
    const GLfloat light[]={4.0F,-5.0F,8.0F,0.0F};
    glLightfv(GL_LIGHT0,GL_POSITION,light);
    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
    int width=1280,height=720; SDL_GetWindowSizeInPixels(window,&width,&height); setProjection(width,height);
    std::cout<<"World preview: "<<items.size()<<" bound objects. Left drag: orbit; right drag: pan; wheel: zoom.\n";
    bool running=true,left=false,right=false;
    while(running) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if(event.type==SDL_EVENT_QUIT) running=false;
            else if(event.type==SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED) { width=event.window.data1; height=event.window.data2; setProjection(width,height); }
            else if(event.type==SDL_EVENT_KEY_DOWN && event.key.key==SDLK_ESCAPE) running=false;
            else if(event.type==SDL_EVENT_MOUSE_BUTTON_DOWN) { if(event.button.button==SDL_BUTTON_LEFT) left=true; if(event.button.button==SDL_BUTTON_RIGHT) right=true; }
            else if(event.type==SDL_EVENT_MOUSE_BUTTON_UP) { if(event.button.button==SDL_BUTTON_LEFT) left=false; if(event.button.button==SDL_BUTTON_RIGHT) right=false; }
            else if(event.type==SDL_EVENT_MOUSE_MOTION) {
                renderer::CameraInput input;
                if(left) { input.orbitDeltaYaw=-event.motion.xrel*.006; input.orbitDeltaPitch=-event.motion.yrel*.006; }
                if(right) { input.panDeltaRight=-event.motion.xrel*.01; input.panDeltaUp=event.motion.yrel*.01; }
                camera.applyInput(input);
            } else if(event.type==SDL_EVENT_MOUSE_WHEEL) {
                renderer::CameraInput input; input.zoomDelta=-event.wheel.y*.12; camera.applyInput(input);
            }
        }
        glClearColor(.055F,.075F,.11F,1.0F);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        applyCamera(camera.cameraState());
        renderWorldItems(items, meshCache);
        SDL_GL_SwapWindow(window);
    }
    meshCache.clear();
    SDL_GL_DestroyContext(context); SDL_DestroyWindow(window); SDL_Quit(); return 0;
}
} // namespace opensim_next::platform
