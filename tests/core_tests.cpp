#include "opensim_next/app/application.hpp"
#include "opensim_next/world/world_state.hpp"

#include <cassert>
#include <string_view>

int main() {
    using opensim_next::app::Application;
    using namespace opensim_next::world;

    assert(Application::name() == std::string_view{"OpenSim-next PC Viewer"});

    WorldState world;
    assert(world.regionCount() == 0);
    assert(!world.addRegion({"", "Invalid"}));
    assert(world.addRegion({"region-1", "Welcome Region"}));
    assert(!world.addRegion({"region-1", "Duplicate"}));
    assert(world.regionCount() == 1);

    assert(!world.addObject("missing-region", {1, "Cube", {}}));
    assert(!world.addObject("region-1", {0, "Invalid ID", {}}));
    assert(world.addObject("region-1", {42, "Test Cube", {}}));
    assert(!world.addObject("region-1", {42, "Duplicate Cube", {}}));

    Transform moved;
    moved.position = {10.0, 20.0, 3.0};
    assert(world.updateObjectTransform("region-1", 42, moved));

    const RegionState* region = world.findRegion("region-1");
    assert(region != nullptr);
    assert(region->objects.at(42).transform.position.x == 10.0);
    assert(region->objects.at(42).transform.position.y == 20.0);

    assert(world.addAvatar("region-1", {7, "Resident", {}}));
    assert(world.updateAvatarTransform("region-1", 7, moved));
    assert(world.findRegion("region-1")->avatars.at(7).transform.position.z == 3.0);
    assert(world.removeAvatar("region-1", 7));
    assert(!world.removeAvatar("region-1", 7));

    assert(world.removeObject("region-1", 42));
    assert(!world.removeObject("region-1", 42));
    assert(world.removeRegion("region-1"));
    assert(world.findRegion("region-1") == nullptr);
    assert(world.regionCount() == 0);
    return 0;
}
