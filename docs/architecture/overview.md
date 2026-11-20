# PC Viewer Architecture

## Status
This is the initial architecture boundary, not a claim that the modules are implemented.

## Logical modules
- app-shell: startup, settings, window lifecycle and diagnostics.
- protocol: login, LLUDP, CAPS/HTTP, asset transfer and simulator connection.
- world: renderer-independent region, terrain, object and avatar state.
- renderer-api: backend-independent device, scene, camera, resource and frame contracts.
- renderer-legacy: compatibility path to be evaluated.
- renderer-modern: future backend; Vulkan is a candidate, not a mandatory requirement.
- asset-pipeline: legacy assets, cache, glTF/GLB import and PBR fallback.
- social: chat, friends, presence, voice integration, opt-in transcription and translation.

## Dependency direction
Protocol updates world state and consumes user commands. World state must not depend on renderer or UI types. Renderers consume world snapshots. UI presents application and social state but does not parse packets.

```text
UI / App Shell
      |
Protocol <----> World State
                   |
               Renderer API
                /       \
        Legacy Backend  Modern Backend
```

## Compatibility invariants
- Do not casually change OpenSim packet IDs, fields, flags, reliability behavior or CAPS semantics.
- Keep legacy inventory, clothing, attachments, scripts and region content usable.
- Renderer choice must not change authoritative simulator behavior.
- Optional graphics features need graceful fallbacks.
- AI actions that publish, transfer, delete or modify assets require explicit user confirmation.

## First vertical slice
Grid selection -> login -> region connection -> terrain/basic primitives -> avatar presence/movement -> local chat -> clean disconnect.

A mock-only scene is useful for renderer tests, but does not count as protocol compatibility.
