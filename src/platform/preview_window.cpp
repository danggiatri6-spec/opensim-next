#include "opensim_next/platform/preview_window.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

#include <algorithm>
#include <cmath>
#include <iostream>

namespace opensim_next::platform {
namespace {

void drawCube() {
    glBegin(GL_QUADS);

    glColor3f(0.85F, 0.25F, 0.20F);
    glVertex3f(-1.0F, -1.0F,  1.0F); glVertex3f( 1.0F, -1.0F,  1.0F);
    glVertex3f( 1.0F,  1.0F,  1.0F); glVertex3f(-1.0F,  1.0F,  1.0F);

    glColor3f(0.20F, 0.55F, 0.90F);
    glVertex3f(-1.0F, -1.0F, -1.0F); glVertex3f(-1.0F,  1.0F, -1.0F);
    glVertex3f( 1.0F,  1.0F, -1.0F); glVertex3f( 1.0F, -1.0F, -1.0F);

    glColor3f(0.25F, 0.75F, 0.40F);
    glVertex3f(-1.0F,  1.0F, -1.0F); glVertex3f(-1.0F,  1.0F,  1.0F);
    glVertex3f( 1.0F,  1.0F,  1.0F); glVertex3f( 1.0F,  1.0F, -1.0F);

    glColor3f(0.90F, 0.70F, 0.20F);
    glVertex3f(-1.0F, -1.0F, -1.0F); glVertex3f( 1.0F, -1.0F, -1.0F);
    glVertex3f( 1.0F, -1.0F,  1.0F); glVertex3f(-1.0F, -1.0F,  1.0F);

    glColor3f(0.65F, 0.35F, 0.80F);
    glVertex3f( 1.0F, -1.0F, -1.0F); glVertex3f( 1.0F,  1.0F, -1.0F);
    glVertex3f( 1.0F,  1.0F,  1.0F); glVertex3f( 1.0F, -1.0F,  1.0F);

    glColor3f(0.20F, 0.75F, 0.80F);
    glVertex3f(-1.0F, -1.0F, -1.0F); glVertex3f(-1.0F, -1.0F,  1.0F);
    glVertex3f(-1.0F,  1.0F,  1.0F); glVertex3f(-1.0F,  1.0F, -1.0F);

    glEnd();
}

void setProjection(int width, int height) {
    const int safeWidth = std::max(width, 1);
    const int safeHeight = std::max(height, 1);
    glViewport(0, 0, safeWidth, safeHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    constexpr double nearPlane = 0.1;
    constexpr double farPlane = 100.0;
    constexpr double halfHeight = nearPlane * 0.4142135623730950;
    const double halfWidth = halfHeight * static_cast<double>(std::max(width, 1))
        / static_cast<double>(safeHeight);
    glFrustum(-halfWidth, halfWidth, -halfHeight, halfHeight, nearPlane, farPlane);

    glMatrixMode(GL_MODELVIEW);
}

} // namespace

int PreviewWindow::run() const {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << '\n';
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_Window* window = SDL_CreateWindow(
        "OpenSim-next PC Viewer - Graphics Preview",
        1280, 720,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );
    if (window == nullptr) {
        std::cerr << "Window creation failed: " << SDL_GetError() << '\n';
        SDL_Quit();
        return 1;
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (context == nullptr) {
        std::cerr << "OpenGL context creation failed: " << SDL_GetError() << '\n';
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_GL_SetSwapInterval(1);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    int width = 1280;
    int height = 720;
    SDL_GetWindowSizeInPixels(window, &width, &height);
    setProjection(width, height);

    std::cout << "OpenSim-next graphics preview started. Close the window to exit.\n";

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            } else if (event.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED) {
                width = event.window.data1;
                height = event.window.data2;
                setProjection(width, height);
            } else if (event.type == SDL_EVENT_KEY_DOWN
                       && event.key.key == SDLK_ESCAPE) {
                running = false;
            }
        }

        const float seconds = static_cast<float>(SDL_GetTicks()) / 1000.0F;
        glClearColor(0.055F, 0.075F, 0.11F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glLoadIdentity();
        glTranslatef(0.0F, 0.0F, -6.0F);
        glRotatef(seconds * 24.0F, 0.4F, 1.0F, 0.2F);
        drawCube();

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DestroyContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

} // namespace opensim_next::platform
