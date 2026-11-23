# Avatar Quick Actions

## Goal

Make avatar gestures faster to discover and invoke, with a modern social-world feel, while retaining Second Life and OpenSim compatibility.

## Baseline decision

OpenSim-next uses Firestorm as its functional Viewer baseline. The current Firestorm branch already has a mature Gestures floater, inventory observer, search filter, activation/loading flow, play/stop behavior, and gesture manager. Therefore, version 1 will **extend the existing Gestures floater** rather than create a duplicate gesture browser.

See [Firestorm baseline audit](../development/avatar-quick-actions-firestorm-audit.md).

## Version 1 scope

Evolve the existing Gestures floater into an on-demand Avatar Quick Actions workspace.

1. Preserve the existing inventory-backed gesture list, search, activation, editor, play/stop, and inventory refresh behavior.
2. Add a compact Favorites view using inventory item UUIDs stored in local per-account settings.
3. Add a bounded Recent list, updated only after a gesture is successfully invoked.
4. Let favorite/recent entries invoke the existing gesture activation/loading/playback path.
5. Validate saved UUIDs against inventory and gracefully discard deleted or unavailable entries.
6. Keep the panel on-demand so the 3D world viewport remains unobstructed by default.

## Architecture

- **UI:** extend `LLFloaterGesture` and its XUI layout; do not add a second gesture floater in version 1.
- **Execution:** delegate to `LLGestureMgr` and the existing `LLFloaterGesture` playback helper. Never start raw avatar motions directly from UI code.
- **Persistence:** local per-account settings for favorites and a bounded recent list. Do not create inventory objects or require server-side storage.
- **Compatibility:** no protocol changes, no new animation format, and no changes to LSL/OSSL animation calls or permissions.

## Explicit non-goals

- No replacement of AO, legacy gestures, inventory, or animation priorities.
- No changes to animation network messages or server behavior.
- No facial blend-shape editor, eye tracking, lip-sync, IK, or new animation runtime in this slice.
- No radial menu until the desktop quick-action flow is stable.
- No mandatory VR device support.

## Implementation sequence

1. Complete the Firestorm source audit and identify settings/XUI conventions.
2. Add Favorites to the existing Gestures floater with minimal changes.
3. Verify play, stop, inactive activation, delayed asset loading, inventory refresh, and deleted-item handling.
4. Add Recent history after the Favorites path is stable.
5. Add categories, keyboard navigation, and optional radial presentation in later increments.

## Acceptance criteria

- Existing gesture inventory, activation, editing, search, play/stop, and keyboard shortcut behavior remains intact.
- A user can invoke a favorite gesture without searching the full inventory list.
- Favorites and recent history persist locally and never create inventory items.
- Stale IDs do not crash the Viewer.
- The panel remains on-demand and leaves the world viewport unobstructed by default.
- No server or protocol changes are required.
- Windows x64 build and runtime smoke tests pass before the feature is considered complete.
