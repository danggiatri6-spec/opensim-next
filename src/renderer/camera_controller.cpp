#include "opensim_next/renderer/camera_controller.hpp"

#include <algorithm>
#include <cmath>

namespace opensim_next::renderer {
namespace {

constexpr double kPi = 3.14159265358979323846;
constexpr double kEpsilon = 1.0e-9;

world::Vec3 subtract(const world::Vec3& left, const world::Vec3& right) noexcept {
    return {left.x - right.x, left.y - right.y, left.z - right.z};
}

world::Vec3 add(const world::Vec3& left, const world::Vec3& right) noexcept {
    return {left.x + right.x, left.y + right.y, left.z + right.z};
}

world::Vec3 multiply(const world::Vec3& value, double scalar) noexcept {
    return {value.x * scalar, value.y * scalar, value.z * scalar};
}

world::Vec3 cross(const world::Vec3& left, const world::Vec3& right) noexcept {
    return {
        left.y * right.z - left.z * right.y,
        left.z * right.x - left.x * right.z,
        left.x * right.y - left.y * right.x
    };
}

double length(const world::Vec3& value) noexcept {
    return std::sqrt(value.x * value.x + value.y * value.y + value.z * value.z);
}

world::Vec3 normalized(const world::Vec3& value, const world::Vec3& fallback) noexcept {
    const double magnitude = length(value);
    return magnitude > kEpsilon ? multiply(value, 1.0 / magnitude) : fallback;
}

} // namespace

CameraController::CameraController(CameraControllerConfig config)
    : config_(config) {
    if (!std::isfinite(config_.minimumDistance) || config_.minimumDistance <= 0.0) {
        config_.minimumDistance = 0.1;
    }
    if (!std::isfinite(config_.maximumDistance)
        || config_.maximumDistance < config_.minimumDistance) {
        config_.maximumDistance = std::max(512.0, config_.minimumDistance);
    }
    if (!std::isfinite(config_.minimumPitchRadians)) {
        config_.minimumPitchRadians = -1.5533430342749532;
    }
    if (!std::isfinite(config_.maximumPitchRadians)
        || config_.maximumPitchRadians <= config_.minimumPitchRadians) {
        config_.maximumPitchRadians = 1.5533430342749532;
    }
    if (!std::isfinite(config_.orbitSensitivity) || config_.orbitSensitivity < 0.0) {
        config_.orbitSensitivity = 1.0;
    }
    if (!std::isfinite(config_.panSensitivity) || config_.panSensitivity < 0.0) {
        config_.panSensitivity = 1.0;
    }
    if (!std::isfinite(config_.zoomSensitivity) || config_.zoomSensitivity < 0.0) {
        config_.zoomSensitivity = 1.0;
    }
    distance_ = std::clamp(distance_, config_.minimumDistance, config_.maximumDistance);
    pitchRadians_ = std::clamp(pitchRadians_, config_.minimumPitchRadians,
                               config_.maximumPitchRadians);
}

void CameraController::setTarget(const world::Vec3& target) noexcept {
    if (std::isfinite(target.x) && std::isfinite(target.y) && std::isfinite(target.z)) {
        target_ = target;
    }
}

void CameraController::setDistance(double distance) noexcept {
    if (std::isfinite(distance)) {
        distance_ = std::clamp(distance, config_.minimumDistance, config_.maximumDistance);
    }
}

void CameraController::applyInput(const CameraInput& input) noexcept {
    if (std::isfinite(input.orbitDeltaYaw)) {
        yawRadians_ = std::remainder(
            yawRadians_ + input.orbitDeltaYaw * config_.orbitSensitivity, 2.0 * kPi);
    }
    if (std::isfinite(input.orbitDeltaPitch)) {
        pitchRadians_ = std::clamp(
            pitchRadians_ + input.orbitDeltaPitch * config_.orbitSensitivity,
            config_.minimumPitchRadians, config_.maximumPitchRadians);
    }

    const CameraState current = cameraState();
    const world::Vec3 worldUp{0.0, 0.0, 1.0};
    const world::Vec3 right = normalized(cross(current.forward, worldUp), {1.0, 0.0, 0.0});
    const world::Vec3 cameraUp = normalized(cross(right, current.forward), worldUp);
    const world::Vec3 pan = add(
        multiply(right, -input.panDeltaRight * config_.panSensitivity),
        multiply(cameraUp, -input.panDeltaUp * config_.panSensitivity));
    if (std::isfinite(pan.x) && std::isfinite(pan.y) && std::isfinite(pan.z)) {
        target_ = add(target_, pan);
    }

    if (std::isfinite(input.zoomDelta)) {
        const double factor = std::exp(-input.zoomDelta * config_.zoomSensitivity);
        if (std::isfinite(factor)) {
            setDistance(distance_ * factor);
        }
    }
}

const world::Vec3& CameraController::target() const noexcept { return target_; }
double CameraController::distance() const noexcept { return distance_; }
double CameraController::yawRadians() const noexcept { return yawRadians_; }
double CameraController::pitchRadians() const noexcept { return pitchRadians_; }

CameraState CameraController::cameraState() const noexcept {
    const double cosPitch = std::cos(pitchRadians_);
    const world::Vec3 offset{
        distance_ * cosPitch * std::cos(yawRadians_),
        distance_ * cosPitch * std::sin(yawRadians_),
        distance_ * std::sin(pitchRadians_)
    };
    const world::Vec3 position = add(target_, offset);
    const world::Vec3 forward = normalized(subtract(target_, position), {0.0, 1.0, 0.0});
    return {position, forward, {0.0, 0.0, 1.0}, 60.0F, 0.1F, 1024.0F};
}

} // namespace opensim_next::renderer
