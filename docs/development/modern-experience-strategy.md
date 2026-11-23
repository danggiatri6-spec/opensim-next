# Modern Virtual-World Experience Strategy

## Purpose

OpenSim-next will adopt the strongest user-facing capabilities demonstrated by modern social VR platforms such as VRChat without becoming a VRChat clone. The product combines the openness and creator freedom of Second Life/OpenSim with a modern, expressive, responsive virtual-world experience.

**Core direction:** Second Life ecosystem compatibility as the foundation; modern social-world presentation and interaction as the experience target; ordinary-PC performance as a hard product constraint.

**Platform delivery order:** Windows x64 is the sole initial build and test-package target. Linux and macOS builds are deferred until the Windows Viewer is reliably buildable, launches consistently, and produces a stable portable ZIP. Do not spend CI time or project effort on other desktop platforms during this first milestone.

This strategy is an incremental evolution of the existing Firestorm-based Viewer plan. It does not authorize a full Viewer rewrite or a switch to a Godot-based production client.

## Non-negotiable principles

1. **Preserve the SL/OpenSim ecosystem.** Retain mature Firestorm functionality and the existing protocol, avatar, inventory, asset, building, social, and scripting workflows wherever possible. Compatibility changes must be tested against real SL/OpenSim content.
2. **Improve the experience in layers.** Rendering, avatar presentation, local avatar physics, interaction, scripting, and UI must have explicit module boundaries and stable interfaces.
3. **Keep the ordinary PC accessible.** Windows 10/11 64-bit is the first product target. Basic world access, social interaction, and building should remain usable on integrated graphics and 8 GB systems. Provide scalable quality settings; do not require a discrete GPU or cloud GPU rendering.
4. **Do not move client presentation work unnecessarily to the server.** Expensive visual-only effects should normally be computed locally. The server remains authoritative for shared world state, permissions, and gameplay-relevant outcomes.
5. **Implement ideas independently.** VRChat SDKs, Udon runtime, proprietary platform services, and platform-specific upload pipelines are not drop-in components. Study documented behavior and implement compatible, independently designed OpenSim-next capabilities.

## Capability goals

### 1. Modern rendering

Evolve the production Firestorm-derived Viewer incrementally. Prioritize a modern material and lighting pipeline while retaining a compatibility path for legacy content and older GPUs.

Candidate capabilities, introduced according to measured value and hardware cost:
- PBR material support and a clear material/texture conversion strategy.
- Improved environment lighting, tone mapping, and HDR where supported.
- Configurable shadows, ambient occlusion, reflections, and post-processing.
- Better water, glass, metal, fabric, and surface response.
- Frustum culling, batching, LOD, texture streaming, and memory budgets.

Do not enable every effect by default. Maintain low/medium/high (and optional advanced) profiles, with per-feature controls for resolution, view distance, shadows, anti-aliasing, and effects. The legacy/fallback path must remain viable.

### 2. Modern avatar presentation

Preserve existing SL avatar skeletons, appearance, wearables, attachments, animations, and content behavior. Add a presentation layer that can progressively support:
- Facial expression and blend-shape mappings where avatar assets provide them.
- Eye gaze and optional eye tracking where hardware and permissions allow.
- Lip-sync and mouth movement.
- Finger articulation and richer gesture presentation.
- IK and improved body-pose blending.
- Better animation blending and attachment presentation.

New capabilities must degrade gracefully for legacy avatars and must not make new avatar formats mandatory for existing content.

### 3. Local avatar and attachment physics

Study the interaction model of systems such as VRChat PhysBones, then implement an independent, lightweight local simulation for suitable avatar attachments:
- Hair, tails, ears, and other flexible attachments.
- Optional cloth-like secondary motion where practical.
- Configurable collision constraints and interaction permissions.
- Per-avatar/per-feature performance budgets and quality settings.

Prefer client-side simulation for visual secondary motion. Synchronize only necessary parameters or interaction outcomes; do not send per-bone simulation updates to the region server. Shared gameplay-relevant physics and object authority remain governed by the server's existing physics and synchronization model.

### 4. Rich world and avatar interaction

Create reusable interaction primitives for touch/contact events, attachment interaction, object grabbing where supported, triggers, and environment responses. Include explicit ownership, permission, distance, rate-limit, and network-authority rules. A resident must not be able to manipulate another resident's avatar or private attachments without consent.

Interactions should work with existing scripted objects where possible and expose carefully versioned extensions for new content.

### 5. Scripting evolution: LSL compatibility plus safe extensions

- Keep LSL/OSSL compatibility a first-class requirement; do not replace LSL with Udon or C#.
- Audit and improve existing script-engine behavior against documented SL semantics and real content tests.
- Evaluate a C#-based extension environment for new experiences only after security architecture, execution limits, API boundaries, and server/client authority are specified.
- Any new C# execution must be sandboxed: no unrestricted filesystem, process, reflection, native-code, or network access for untrusted world scripts.
- Consider a node-based visual scripting editor as a later creator tool, not as a prerequisite for compatibility or current Viewer milestones.

Udon and UdonSharp are reference points for developer experience and constrained execution, not dependencies to embed or clone.

### 6. Modern user experience and creator workflow

Modernize the interface without obscuring the 3D world. Keep inventory/workspace panels on demand rather than permanently consuming the world viewport. Improve avatar management, graphics settings, social interaction, build workflows, onboarding, and asset handling incrementally. Preserve direct in-world building as a distinguishing OpenSim/SL capability.

## Architecture boundaries

- **Firestorm-derived production Viewer:** main client foundation and owner of presentation and local visual simulation.
- **Protocol/network layer:** retains SL/OpenSim protocol responsibilities and transports authoritative shared state.
- **World model:** owns client-side replicated world state; renderer and presentation systems consume snapshots rather than becoming authorities.
- **Renderer/material pipeline:** evolves behind stable interfaces, with legacy fallback and modern rendering paths.
- **Avatar presentation/animation:** maps existing avatar data and animations into richer local presentation.
- **Local secondary-physics system:** handles visual hair/attachment motion within strict CPU budgets.
- **Interaction layer:** defines permission-aware interaction events and synchronized outcomes.
- **Server/OpenSim-next:** remains responsible for region/world authority, permissions, scripts, and shared state according to the server architecture. New server capabilities are introduced only when local presentation cannot safely or correctly provide the behavior.

The experimental SDL3/OpenGL prototype remains a renderer/architecture research track. It is not a parallel full Viewer implementation and is not the production client foundation.

## Delivery order

The sequence below is a dependency-aware direction, not a promise that every item ships in one release. Work may overlap when interfaces and tests are stable.

1. **Foundation and compatibility:** complete the Firestorm baseline integration and Windows x64 build; validate launch and produce a stable portable ZIP before starting other desktop platforms; establish SL/OpenSim regression grids, representative legacy avatars, wearables, attachments, scripted furniture, HUDs, AO, and building tests.
2. **Rendering fundamentals:** texture/material pipeline, shader material interface, resource lifecycle, batching, culling, LOD, and low-end profiling; then add PBR and selected lighting effects behind quality controls.
3. **Avatar presentation:** audit skeleton/animation/appearance compatibility; add expression, gaze, lip-sync, gestures, and IK incrementally.
4. **Local secondary physics:** prototype and profile hair/attachment motion, then add optional collision and consent-aware interactions.
5. **Interaction framework:** reusable contact/trigger/grab-style primitives with explicit permissions and synchronization rules.
6. **Scripting extensions and creator tools:** improve LSL compatibility continuously; separately evaluate sandboxed C# and visual scripting after security and compatibility gates are met.
7. **UX refinement:** modernize workflows and panels while keeping the world viewport primary.

## Acceptance criteria

A feature is not considered complete merely because it works in a showcase scene. It must:
- Pass relevant SL/OpenSim compatibility regression tests.
- Have documented behavior, limits, permissions, and fallback behavior.
- Be profiled on integrated graphics and an 8 GB Windows system, as well as a representative mid-range PC.
- Respect configurable performance budgets and avoid requiring cloud rendering.
- Avoid breaking legacy assets, scripts, avatars, inventory, or existing Firestorm workflows.
- Clearly identify whether state is local presentation, replicated client state, or server-authoritative state.

## Explicit exclusions

- No production Godot Viewer rewrite.
- No wholesale replacement of Firestorm or the SL/OpenSim protocol stack.
- No replacement of LSL with Udon/UdonSharp.
- No requirement that all users have VR headsets, eye tracking, or high-end GPUs.
- No cloud GPU rendering dependency in the current product strategy.
- No server-side replication of every visual-only bone, cloth, or hair simulation step.
