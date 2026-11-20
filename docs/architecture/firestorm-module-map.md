# Firestorm module map for OpenSim-next

Audit baseline: Firestorm `Firestorm_7.2.5`, commit `4ac201fdc56127de27de34567286885f4e75d194`.

## Repository scale and build system

The pinned upstream Git tree contains 28,601 entries. The viewer is not a small library that can be added to the current CMake project. It uses the Linden Lab/Firestorm `indra` source layout, CMake subprojects, Autobuild dependency manifests and platform packaging scripts.

The `indra/newview` directory contains the bulk of the Viewer implementation and UI resources (26,886 tree entries at this baseline, including skins, translations, XUI layouts, shaders and other resources). Core libraries are separated into modules such as:

| Module | Tree entries | Responsibility / reuse target |
|---|---:|---|
| `indra/llmessage` | 181 | LLUDP messaging, message templates and protocol foundations |
| `indra/llcorehttp` | 69 | HTTP transport |
| `indra/llappearance` | 36 | Appearance and wearable-related foundations |
| `indra/llcharacter` | 44 | Character, rigging and animation foundations |
| `indra/llinventory` | 45 | Inventory model and operations |
| `indra/llprimitive` | 43 | Primitive/object data |
| `indra/llrender` | 54 | Rendering abstraction and implementation |
| `indra/llimage` | 27 | Image decode and image data |
| `indra/llfilesystem` | 24 | File/cache storage foundations |
| `indra/llui` | 233 | UI framework and controls |
| `indra/llwindow` | 44 | Platform window/input layer |
| `indra/llcommon` | 317 | Shared utilities and base infrastructure |
| `indra/newview` | 26,886 | Main Viewer application, OpenSim/SL integration, UI, skins and resources |

Counts are Git tree entries, not source-line counts or independent build targets.

## What this means for the integration

Do not try to port isolated Firestorm source files into the existing SDL3/CMake prototype. The Viewer modules depend on shared LL libraries, generated headers, platform abstractions, bundled third-party packages and build-time configuration. Copying only `indra/newview` would not produce a buildable client.

The correct initial strategy is to build the full upstream tree unmodified, using its own build system. Then make product changes as a small patch series.

## Reuse boundaries

### Keep upstream as the initial owner

- Network protocol and login/session lifecycle.
- Region connection, object updates and simulator capabilities.
- Avatar appearance, animation, attachments and movement.
- Inventory, asset fetching, cache and texture decoding.
- Chat, friends, groups, presence and notifications.
- Object editing/build tools and established UI behavior.
- Existing renderer, graphics settings and platform abstraction for the first working release.

### OpenSim-next-owned additions

- Product name, branding, onboarding and Grid directory.
- Default Grid configuration and account entry experience.
- Product-level settings and accessible UI improvements.
- AI translation and user-controlled creation assistance.
- Low/medium/high performance profiles with safe defaults.
- New service boundaries and renderer experiments, introduced behind adapters.

### Integration cautions

- Do not maintain two competing authoritative copies of region/object/avatar state. Define which Firestorm subsystem owns live simulator state and which OpenSim-next services consume read-only events or commands.
- Avoid a wholesale renderer replacement at the start. First measure Firestorm's actual performance on target integrated GPUs and identify bottlenecks.
- Preserve OpenSim protocol compatibility and test against a real OpenSim Grid after every network/session change.
- Keep optional licensed dependencies (for example KDU/Kakadu or FMOD) out of the initial build unless there is an explicit need and valid licensing.
- Audit the license of each dependency, artwork, icon, font and other asset separately.

## Recommended repository arrangement

Preferred long-term arrangement:

1. A GitHub fork of `FirestormViewer/phoenix-firestorm` (or a separate repository created from a mirror while preserving all upstream Git history) as the actual OpenSim-next Viewer source.
2. An `opensim-next` remote pointing to the original Firestorm upstream.
3. An `opensim-next-dev` branch for product work; keep the chosen upstream release tag/commit recorded.
4. A separate `opensim-next-architecture` or existing prototype repository for the small SDL3 renderer experiments until individual components are deliberately ported.
5. A patch policy: product branding/configuration first, then small isolated features; periodically merge/rebase upstream changes and document conflicts.

A GitHub fork should be created through GitHub's Fork action while authenticated as the repository owner. The available repository automation in this session can create branches and files in the existing `opensim-next` repository, but does not expose a fork-creation operation. No fork or source import is claimed here.

## Immediate build gate

Before making any source changes:

1. Use Windows 10/11 x64 with Visual Studio 2022 and the versions listed in the pinned upstream Windows build guide.
2. Clone the pinned Firestorm source and the required `fs-build-variables` repository.
3. Install Autobuild and Python requirements exactly as documented upstream.
4. Configure the `ReleaseOS` target (not `ReleaseFS_open`, which excludes OpenSim).
5. Build the untouched baseline.
6. Launch it and test login, region entry, avatar appearance, inventory, chat and basic object editing on a test OpenSim Grid.
7. Only then begin branding and product patches.

## Baseline references

- Source tree: https://github.com/FirestormViewer/phoenix-firestorm/tree/4ac201fdc56127de27de34567286885f4e75d194
- Windows build guide: https://github.com/FirestormViewer/phoenix-firestorm/blob/Firestorm_7.2.5/doc/building_windows.md
- Build variables: https://github.com/FirestormViewer/fs-build-variables
- Existing integration plan: [firestorm-integration.md](../development/firestorm-integration.md)
- Baseline build audit: [firestorm-baseline-audit.md](../development/firestorm-baseline-audit.md)
