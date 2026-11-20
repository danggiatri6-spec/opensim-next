#include "opensim_next/app/application.hpp"
#include "opensim_next/renderer/camera_controller.hpp"
#include "opensim_next/renderer/mock_renderer.hpp"
#include "opensim_next/renderer/primitive_meshes.hpp"
#include "opensim_next/renderer/world_render_bridge.hpp"
#include "opensim_next/world/world_state.hpp"
#ifdef OPENSIM_NEXT_ENABLE_SDL3
#include "opensim_next/platform/preview_window.hpp"
#endif
#include <iostream>
namespace opensim_next::app {
int Application::run() const {
    using namespace renderer; using namespace world;
    WorldState world;
    if (!world.addRegion({"local-preview","Local Preview"})) return 1;
    Transform a; a.position={0,0,0};
    Transform b; b.position={1.8,0,0}; b.scale={.7,.7,.7};
    if (!world.addObject("local-preview",{1,"Preview Cube A",a})) return 1;
    if (!world.addObject("local-preview",{2,"Preview Cube B",b})) return 1;
    const MeshData cubeGeometry = primitives::makeUnitCube();
    if (!cubeGeometry.isValid()) return 1;
    MockRenderer mock;
    if (!mock.initialize({1280,720,true})) return 1;
    const MeshHandle mesh=mock.createMesh("unit-cube");
    const MaterialHandle material=mock.createMaterial(MaterialDescriptor{});
    if (!mesh || !material) { mock.shutdown(); return 1; }
    const RenderBinding bindings[]={{1,mesh,material,&cubeGeometry},{2,mesh,material,&cubeGeometry}};
    const RegionState* region=world.findRegion("local-preview");
    if (!region) { mock.shutdown(); return 1; }
    const auto drawItems=WorldRenderBridge::buildDrawItems(*region,bindings);
    CameraController camera; camera.setTarget({.8,0,0}); camera.setDistance(7.0);
    const bool submitted=mock.renderFrame({camera.cameraState(),drawItems});
    std::cout<<name()<<'\n'<<"World-to-renderer pipeline: "<<(submitted?"ready":"failed")<<'\n';
    std::cout<<"Backend: mock validation + SDL3/OpenGL compatibility preview\n";
    std::cout<<"World objects submitted: "<<mock.lastDrawItemCount()<<'\n';
    mock.destroyMaterial(material); mock.destroyMesh(mesh); mock.shutdown();
    if (!submitted) return 1;
#ifdef OPENSIM_NEXT_ENABLE_SDL3
    return platform::PreviewWindow{}.run(drawItems,camera);
#else
    return 0;
#endif
}
} // namespace opensim_next::app
