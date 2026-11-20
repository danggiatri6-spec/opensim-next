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
    return 0;
}
