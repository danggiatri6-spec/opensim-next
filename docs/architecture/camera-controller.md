# Camera controller

## Purpose

The camera controller is a renderer-independent orbit camera model. It converts normalized input deltas into a camera snapshot without depending on a window library, mouse API, or graphics backend.

## Controls

- Orbit: change yaw and pitch in radians.
- Pan: move the orbit target along camera-right and camera-up axes.
- Zoom: apply exponential distance scaling, clamped to configured minimum and maximum distances.
- Target and distance can also be set directly by application code.

## Coordinate convention

The initial implementation assumes a Z-up world. Camera up is `{0, 0, 1}`. The renderer backend remains responsible for projection and graphics-API clip-space conversion.

## Safety behavior

Non-finite target, distance, and input values are ignored. Pitch is clamped to configured limits to avoid a singularity at the poles. Distance is clamped to a positive configured interval.

## Scope limits

This is a pure camera state/controller, not a complete input system. It does not read mouse or keyboard events, perform collision detection, or manage avatar-follow behavior. The default sensitivities are neutral multipliers; the platform input layer should normalize device-specific deltas before passing them in.
