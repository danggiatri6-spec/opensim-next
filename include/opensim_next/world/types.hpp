#pragma once

#include <cstdint>
#include <string>

namespace opensim_next::world {

using EntityId = std::uint64_t;

struct Vec3 {
    double x{0.0};
    double y{0.0};
    double z{0.0};
};

struct Quaternion {
    double x{0.0};
    double y{0.0};
    double z{0.0};
    double w{1.0};
};

struct Transform {
    Vec3 position{};
    Quaternion rotation{};
    Vec3 scale{1.0, 1.0, 1.0};
};

struct RegionDescriptor {
    std::string id;
    std::string name;
};

struct WorldObject {
    EntityId id{0};
    std::string name;
    Transform transform{};
};

struct AvatarState {
    EntityId id{0};
    std::string displayName;
    Transform transform{};
};

} // namespace opensim_next::world
