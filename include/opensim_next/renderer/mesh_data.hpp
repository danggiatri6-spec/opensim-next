#pragma once

#include <cstdint>
#include <vector>

namespace opensim_next::renderer {

struct MeshVertex {
    float position[3]{};
    float normal[3]{};
    float uv[2]{};
};

struct MeshData {
    std::vector<MeshVertex> vertices;
    std::vector<std::uint32_t> indices;

    [[nodiscard]] bool isValid() const noexcept;
};

} // namespace opensim_next::renderer
