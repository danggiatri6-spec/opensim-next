#include "opensim_next/app/application.hpp"
#include "opensim_next/renderer/mock_renderer.hpp"
#include "opensim_next/renderer/primitive_meshes.hpp"
#include "opensim_next/renderer/camera_controller.hpp"
#include "opensim_next/renderer/world_render_bridge.hpp"
#include "opensim_next/world/world_state.hpp"

#include <cassert>
#include <cmath>
#include <limits>
#include <string_view>

int main() {
    using opensim_next::app::Application;
    using namespace opensim_next::world;

    assert(Application::name() == std::string_view{"OpenSim-next PC Viewer"});

    WorldState world;
    assert(world.regionCount() == 0);

    const RegionDescriptor invalidRegion{"", "Invalid"};
    const RegionDescriptor validRegion{"region-1", "Welcome Region"};
    const RegionDescriptor duplicateRegion{"region-1", "Duplicate"};
    assert(!world.addRegion(invalidRegion));
    assert(world.addRegion(validRegion));
    assert(!world.addRegion(duplicateRegion));
    assert(world.regionCount() == 1);

    const WorldObject missingRegionObject{1, "Cube", {}};
    const WorldObject invalidObject{0, "Invalid ID", {}};
    const WorldObject object{42, "Test Cube", {}};
    const WorldObject duplicateObject{42, "Duplicate Cube", {}};
    assert(!world.addObject("missing-region", missingRegionObject));
    assert(!world.addObject("region-1", invalidObject));
    assert(world.addObject("region-1", object));
    assert(!world.addObject("region-1", duplicateObject));

    Transform moved;
    moved.position = {10.0, 20.0, 3.0};
    assert(world.updateObjectTransform("region-1", 42, moved));

    const RegionState* region = world.findRegion("region-1");
    assert(region != nullptr);
    assert(region->objects.at(42).transform.position.x == 10.0);
    assert(region->objects.at(42).transform.position.y == 20.0);

    const AvatarState avatar{7, "Resident", {}};
    assert(world.addAvatar("region-1", avatar));
    assert(world.updateAvatarTransform("region-1", 7, moved));
    assert(world.findRegion("region-1")->avatars.at(7).transform.position.z == 3.0);
    assert(world.removeAvatar("region-1", 7));
    assert(!world.removeAvatar("region-1", 7));

    assert(world.removeObject("region-1", 42));
    assert(!world.removeObject("region-1", 42));
    assert(world.removeRegion("region-1"));
    assert(world.findRegion("region-1") == nullptr);
    assert(world.regionCount() == 0);
    using namespace opensim_next::renderer;

    const auto cubeGeometry = primitives::makeUnitCube();
    assert(cubeGeometry.isValid());
    assert(cubeGeometry.vertices.size() == 24);
    assert(cubeGeometry.indices.size() == 36);
    const auto planeGeometry = primitives::makeUnitPlane();
    assert(planeGeometry.isValid());
    assert(planeGeometry.vertices.size() == 4);
    assert(planeGeometry.indices.size() == 6);

    auto invalidGeometry = cubeGeometry;
    invalidGeometry.indices[0] = 9999;
    assert(!invalidGeometry.isValid());
    invalidGeometry = cubeGeometry;
    invalidGeometry.indices.pop_back();
    assert(!invalidGeometry.isValid());
    invalidGeometry = cubeGeometry;
    invalidGeometry.vertices[0].position[0] = std::numeric_limits<float>::infinity();
    assert(!invalidGeometry.isValid());

    MockRenderer renderer;
    assert(!renderer.isInitialized());
    assert(!renderer.renderFrame({}));
    assert(!renderer.initialize({0, 720, true}));
    assert(renderer.initialize({1280, 720, true}));
    assert(renderer.isInitialized());
    assert(!renderer.initialize({1280, 720, true}));
    assert(!renderer.resize(1280, 0));
    assert(renderer.resize(1920, 1080));

    const auto capabilities = renderer.capabilities();
    assert(capabilities.maxTextureSize == 8192);
    assert(capabilities.physicallyBasedMaterials);
    assert(!capabilities.skeletalAnimation);

    const auto mesh = renderer.createMesh("test-cube");
    const auto texture = renderer.createTexture("test-albedo");
    assert(static_cast<bool>(mesh));
    assert(static_cast<bool>(texture));

    MaterialDescriptor materialDescriptor;
    materialDescriptor.baseColorTexture = texture;
    materialDescriptor.metallic = 0.2F;
    materialDescriptor.roughness = 0.7F;
    const auto material = renderer.createMaterial(materialDescriptor);
    assert(static_cast<bool>(material));

    CameraController camera;
    camera.setTarget({10.0, 20.0, 5.0});
    camera.setDistance(12.0);
    assert(camera.distance() == 12.0);
    const auto initialCamera = camera.cameraState();
    assert(initialCamera.up.z == 1.0);
    assert(initialCamera.farClip > initialCamera.nearClip);
    assert(initialCamera.position.x != camera.target().x
        || initialCamera.position.y != camera.target().y
        || initialCamera.position.z != camera.target().z);

    camera.applyInput({0.25, 0.1, 0.0, 0.0, 0.0});
    assert(camera.yawRadians() != -1.5707963267948966);
    assert(camera.pitchRadians() > 0.35);
    camera.applyInput({0.0, 0.0, 1.0, -0.5, 0.0});
    assert(camera.target().x != 10.0 || camera.target().y != 20.0
        || camera.target().z != 5.0);
    camera.applyInput({0.0, 0.0, 0.0, 0.0, 1.0});
    assert(camera.distance() < 12.0);
    camera.setDistance(0.0);
    assert(camera.distance() == 1.0);
    camera.setDistance(10000.0);
    assert(camera.distance() == 512.0);
    camera.setTarget({std::numeric_limits<double>::quiet_NaN(), 0.0, 0.0});
    assert(std::isfinite(camera.target().x));

    WorldState renderWorld;
    assert(renderWorld.addRegion({"render-region", "Render Test"}));
    Transform firstTransform;
    firstTransform.position = {1.0, 2.0, 3.0};
    Transform secondTransform;
    secondTransform.position = {4.0, 5.0, 6.0};
    assert(renderWorld.addObject("render-region", {200, "Second", secondTransform}));
    assert(renderWorld.addObject("render-region", {100, "First", firstTransform}));

    const RenderBinding renderBindings[]{
        {200, mesh, material},
        {100, mesh, material},
        {300, mesh, material},
        {400, MeshHandle{}, material}
    };
    const auto* renderRegion = renderWorld.findRegion("render-region");
    assert(renderRegion != nullptr);
    const auto bridgedDrawItems = WorldRenderBridge::buildDrawItems(*renderRegion, renderBindings);
    assert(bridgedDrawItems.size() == 2);
    assert(bridgedDrawItems[0].entityId == 100);
    assert(bridgedDrawItems[1].entityId == 200);
    assert(bridgedDrawItems[0].transform.position.z == 3.0);
    assert(bridgedDrawItems[1].transform.position.x == 4.0);

    assert(renderer.renderFrame({CameraState{}, bridgedDrawItems}));
    assert(renderer.renderedFrameCount() == 1);
    assert(renderer.lastDrawItemCount() == 2);

    const auto invalidMaterial = renderer.createMaterial(
        MaterialDescriptor{Color4{}, 0.0F, 1.5F, std::nullopt});
    assert(!static_cast<bool>(invalidMaterial));

    const DrawItem invalidDrawItem{100, MeshHandle{9999}, material, {}};
    const DrawItem invalidDrawItems[]{invalidDrawItem};
    assert(!renderer.renderFrame({CameraState{}, invalidDrawItems}));

    renderer.destroyMaterial(material);
    renderer.destroyTexture(texture);
    renderer.destroyMesh(mesh);
    assert(!renderer.renderFrame({CameraState{}, bridgedDrawItems}));
    renderer.shutdown();
    assert(!renderer.isInitialized());
    assert(renderer.capabilities().maxTextureSize == 0);
    return 0;
}
