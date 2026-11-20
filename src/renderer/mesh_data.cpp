#include "opensim_next/renderer/mesh_data.hpp"

#include <cmath>

namespace opensim_next::renderer {

bool MeshData::isValid() const noexcept {
    if (vertices.empty() || indices.empty() || indices.size() % 3 != 0) {
        return false;
    }

    for (const auto& vertex : vertices) {
        for (const float value : vertex.position) {
            if (!std::isfinite(value)) return false;
        }
        for (const float value : vertex.normal) {
            if (!std::isfinite(value)) return false;
        }
        for (const float value : vertex.uv) {
            if (!std::isfinite(value)) return false;
        }
    }

    for (const auto index : indices) {
        if (index >= vertices.size()) return false;
    }
    return true;
}

} // namespace opensim_next::renderer
