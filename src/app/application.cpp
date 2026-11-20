#include "opensim_next/app/application.hpp"

#include "opensim_next/renderer/camera_controller.hpp"
#include "opensim_next/renderer/mock_renderer.hpp"
#include "opensim_next/renderer/world_render_bridge.hpp"
#include "opensim_next/world/world_state.hpp"
#ifdef OPENSIM_NEXT_ENABLE_SDL3
#include "opensim_next/platform/preview_window.hpp"
#endif

#include <iostream>

namespace opensim_next::app {

int Application::run() const {
    using namespace renderer;
    using namespace world;

    WorldState world;
    if (!world.addRegion({"local-preview", "Local Preview"})) {
        return 1;
    }

    Transform cubeTransform;
    cubeTransform.position = {0.0, 0.0, 0.5};
    if (!world.addObject("local-preview", {1, "Preview Cube", cubeTransform})) {
        return 1;
    }

    MockRenderer renderer;
    if (!renderer.initialize({1280, 720, true})) {
        return 1;
    }

    const MeshHandle cubeMesh = renderer.createMesh("preview-cube");
    const MaterialHandle cubeMaterial = renderer.createMaterial(MaterialDescriptor{});
    if (!cubeMesh || !cubeMaterial) {
        renderer.shutdown();
        return 1;
    }

    const RenderBinding bindings[]{{1, cubeMesh, cubeMaterial}};
    const RegionState* region = world.findRegion("local-preview");
    if (region == nullptr) {
        renderer.shutdown();
        return 1;
    }

    const auto drawItems = WorldRenderBridge::buildDrawItems(*region, bindings);
    CameraController camera;
    camera.setTarget({0.0, 0.0, 0.5});

    const bool submitted = renderer.renderFrame({camera.cameraState(), drawItems});
    std::cout << name() << '\n';
    std::cout << "World-to-renderer pipeline: "
              << (submitted ? "ready" : "failed") << '\n';
    std::cout << "Backend: mock (no pixels are drawn)\n";
    std::cout << "Draw items submitted: " << renderer.lastDrawItemCount() << '\n';

    renderer.destroyMaterial(cubeMaterial);
    renderer.destroyMesh(cubeMesh);
    renderer.shutdown();
    if (!submitted) {
        return 1;
    }
#ifdef OPENSIM_NEXT_ENABLE_SDL3
    std::cout << "Opening SDL3/OpenGL compatibility preview. Press Escape or close the window to exit.\\n";
    return platform::PreviewWindow{}.run();
#else
    return 0;
#endif
}

} // namespace opensim_next::app
