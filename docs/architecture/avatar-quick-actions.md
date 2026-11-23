# Avatar Quick Actions

## Goal

Add a fast, discoverable avatar action surface to the Firestorm-derived OpenSim-next Viewer. The interaction should feel closer to a modern social-world expression menu while continuing to use the existing Second Life avatar, inventory, gesture, and animation systems.

This is an incremental Viewer UX feature, not a replacement animation runtime.

## Existing capabilities to reuse

The current Firestorm baseline already contains:

- `LLGestureMgr` and `LLViewerGesture` for inventory-backed gesture loading and playback.
- `LLFloaterGesture` for gesture inventory, filtering, activation, playback, editing, and keyboard shortcuts.
- `LLEmote` for legacy facial-emote motion support.
- Firestorm Poser (`FSFloaterPoser`, `FSPoserAnimator`) and hand-pose presets for advanced local posing.

The first implementation must call existing public interfaces rather than duplicate gesture playback or manipulate avatar joints directly.

## Version 1 scope

Create a compact, on-demand **Avatar Quick Actions** floater for desktop users.

### User-facing sections

1. **Favorites** — user-selected gestures shown as large, easy-to-hit action tiles.
2. **Gestures** — inventory-backed gesture list with a search/filter field.
3. **Recent** — recently invoked gestures, kept locally.
4. **Stop** — stop the user's currently playing gesture/action using the existing supported Viewer behavior.

The floater should open from an obvious toolbar/menu entry and close without permanently reducing the 3D world viewport. It must be usable with mouse and keyboard and remain practical at ordinary desktop resolutions.

### Behavior

- Selecting a gesture invokes the existing gesture manager using its inventory item ID.
- Do not create, upload, purchase, or bundle animations.
- Do not silently activate inactive inventory gestures; make activation state visible and follow the existing activation rules.
- Favorites and recent history are local Viewer preferences, not new server-side inventory objects.
- Missing, unloaded, or inactive gestures must show a clear state rather than crashing or silently doing nothing.
- Preserve the existing Gestures floater and all current gesture editing/activation workflows.

## Explicit non-goals for version 1

- No new animation format or runtime.
- No changes to LSL/OSSL animation permissions or network messages.
- No replacement of AO, inventory gestures, or existing animation priorities.
- No facial blend-shape editor, eye tracking, lip-sync, or IK implementation in this first slice.
- No server-side storage or replication of favorites/recent history.
- No radial menu in the first implementation; establish a reliable desktop panel first. A radial presentation can later reuse the same action model.

## Architecture

- **UI:** a new small floater and XUI layout, opened on demand.
- **Action model:** a thin UI-facing model containing inventory item ID, display name, activation state, and local favorite/recent metadata.
- **Execution:** delegate playback and state changes to existing gesture/Viewer APIs. The UI must not directly start raw avatar motions.
- **Persistence:** use existing per-account/local settings conventions for favorite IDs and a bounded recent list. Validate IDs against inventory whenever the floater refreshes.
- **Compatibility:** no protocol changes. Existing Second Life/OpenSim inventory and gesture semantics remain authoritative.

## Implementation sequence

1. Audit the exact gesture-manager entry points and existing floater registration/toolbar patterns on the pinned Firestorm branch.
2. Add the smallest possible new floater and registration, initially displaying a filtered list of existing inventory gestures.
3. Wire one action tile to the existing gesture playback path; verify play, stop, inactive, missing-item, and inventory-refresh behavior.
4. Add favorites and recent history using local settings.
5. Add categories and keyboard navigation only after the basic flow is stable.
6. Build one Windows x64 ZIP and test the feature on both Second Life and an OpenSim test grid.

## Acceptance criteria

- Existing Second Life login, inventory, gestures, AO, and animation behavior remain intact.
- A user can open the floater, find a gesture, and play it without opening Inventory.
- Existing gesture activation, edit, and shortcut workflows still work.
- Favorites/recent history survive Viewer restart and do not create inventory items.
- The panel is on-demand and does not permanently occupy world-view space.
- No new network protocol or server feature is required.
- The change passes a Windows x64 build and runtime smoke test before being considered complete.

## Later extensions

After version 1 is stable, consider a radial presentation, explicit gesture categories, supported facial expression shortcuts, optional hand-pose presets, and integration with the existing Poser. Each extension must reuse established animation/pose paths and preserve legacy avatar fallbacks.
