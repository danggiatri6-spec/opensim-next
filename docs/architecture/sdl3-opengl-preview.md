# SDL3 and OpenGL preview

## Purpose

This is a graphics bring-up executable path, not the final renderer backend. It proves that the desktop application can create a cross-platform window, obtain an OpenGL context, process basic events, resize its viewport, and present frames.

## Dependency strategy

- SDL3 is fetched by CMake FetchContent at the pinned `release-3.4.20` tag when `OPENSIM_NEXT_ENABLE_SDL3=ON`.
- SDL3 is used for window creation, events, and OpenGL context management.
- The preview requests an OpenGL 2.1 compatibility context to keep the first test path usable on older desktop graphics stacks.
- The current CI builds the preview on Windows and Ubuntu. CI does not launch a graphical window because hosted runners are headless.

## Current visible test

The preview draws a rotating colored cube with fixed-function OpenGL calls. It handles window close, Escape, pixel-size changes, depth buffering, and buffer swapping.

This cube is deliberately a minimal graphics smoke test. It is not yet created from `WorldRenderBridge` draw items, and it does not implement PBR, shaders, terrain, avatar rendering, asset loading, or OpenSim networking.

## Why fixed-function OpenGL is temporary

The first objective is to verify window/context creation and a low-complexity GPU path. Fixed-function OpenGL is a compatibility bridge only. The next renderer step is a shader-based backend behind `IRenderer`, with capability detection and a fallback strategy. We should not let this preview become a permanent rendering architecture.

## Build switches

- `OPENSIM_NEXT_ENABLE_SDL3=ON`: fetch SDL3 and build the interactive preview (default).
- `OPENSIM_NEXT_ENABLE_SDL3=OFF`: build the core and tests without SDL3 or a graphics window.

The preview requires an OpenGL-capable display driver at runtime. The headless CI validates compilation and core tests, not actual GPU output.
