#include "opensim_next/platform/preview_window.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

#include <algorithm>
#include <cmath>
#include <iostream>

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
void drawMeshGeometry(const renderer::MeshData& mesh) {
    glBegin(GL_TRIANGLES);
    for (const std::uint32_t index : mesh.indices) {
        const auto& vertex = mesh.vertices[index];
        glNormal3fv(vertex.normal);
        glTexCoord2fv(vertex.uv);
        glVertex3fv(vertex.position);
    }
    glEnd();
}
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
void renderWorldItems(std::span<const renderer::DrawItem> items) {
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
            drawMeshGeometry(*item.geometry);
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
        renderWorldItems(items);
        SDL_GL_SwapWindow(window);
    }
    SDL_GL_DestroyContext(context); SDL_DestroyWindow(window); SDL_Quit(); return 0;
}
} // namespace opensim_next::platform
