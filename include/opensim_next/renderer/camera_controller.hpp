#pragma once

#include "opensim_next/renderer/renderer_api.hpp"

namespace opensim_next::renderer {

struct CameraInput {
    double orbitDeltaYaw{0.0};
    double orbitDeltaPitch{0.0};
    double panDeltaRight{0.0};
    double panDeltaUp{0.0};
    double zoomDelta{0.0};
};

struct CameraControllerConfig {
    double minimumDistance{1.0};
    double maximumDistance{512.0};
    double orbitSensitivity{1.0};
    double panSensitivity{1.0};
    double zoomSensitivity{1.0};
    double minimumPitchRadians{-1.5533430342749532};
    double maximumPitchRadians{1.5533430342749532};
};

class CameraController final {
public:
    explicit CameraController(CameraControllerConfig config = {});

    void setTarget(const world::Vec3& target) noexcept;
    void setDistance(double distance) noexcept;
    void applyInput(const CameraInput& input) noexcept;

    [[nodiscard]] const world::Vec3& target() const noexcept;
    [[nodiscard]] double distance() const noexcept;
    [[nodiscard]] double yawRadians() const noexcept;
    [[nodiscard]] double pitchRadians() const noexcept;
    [[nodiscard]] CameraState cameraState() const noexcept;

private:
    CameraControllerConfig config_;
    world::Vec3 target_{};
    double distance_{8.0};
    double yawRadians_{-1.5707963267948966};
    double pitchRadians_{0.35};
};

} // namespace opensim_next::renderer
