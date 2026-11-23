# Avatar Quick Actions — Firestorm Baseline Audit

## Audit result

The pinned Firestorm branch already provides most of the backend and basic UI behavior originally proposed for a separate Avatar Quick Actions floater. A second independent gesture browser would duplicate inventory observation, gesture activation, filtering, and playback logic, increasing regression risk.

Relevant existing implementation:

- `indra/newview/llfloatergesture.h/.cpp`: inventory-backed gesture floater, observer lifecycle, filtering, list refresh, activation, editing, play/stop.
- `indra/newview/skins/default/xui/en/floater_gesture.xml`: existing desktop gesture list and controls.
- `indra/newview/llgesturemgr.h`: public gesture manager API, including active/playing state, play/stop, activation/deactivation, and observers.
- `indra/newview/fsfloaterposer.h/.cpp`: Firestorm's advanced avatar posing implementation; not a replacement for inventory gestures.

## Verified behavior in the current branch

- The gesture floater observes `LLGestureMgr` and refreshes when its state changes.
- It fetches the user's gesture inventory folder and descendants.
- It lists active gestures and inventory gestures that are not currently active.
- It has a filter editor and preserves selection/scroll state while rebuilding.
- Double-click and the Play button use the existing playback path.
- If a gesture is inactive, the existing UI activates it with the inventory asset and schedules playback after loading.
- If the gesture is already playing, the same playback helper stops it.
- The UI reflects whether the selected gesture is currently playing.
- Firestorm's account-level gesture enable/disable setting is respected.

## Revised implementation decision

Do not create a second gesture floater in the first milestone. Evolve the existing Gestures floater incrementally into the Avatar Quick Actions workspace.

### First implementation slice

1. Preserve all existing gesture inventory, activation, editing, filtering, play/stop, and observer behavior.
2. Add a compact Favorites presentation backed by local per-account settings containing inventory item UUIDs.
3. Add a bounded Recent list, also local, updated only after a gesture is successfully invoked.
4. Provide quick-play actions that delegate to the existing `LLFloaterGesture` and `LLGestureMgr` flow.
5. Validate stored UUIDs against inventory; gracefully discard stale/deleted favorites and recent entries.
6. Keep the window on-demand; do not reserve permanent screen space or cover the world view by default.

### Safety constraints

- Do not call raw avatar motion APIs from the UI.
- Do not bypass the existing inactive-gesture activation/loading callback.
- Do not alter LSL/OSSL animation calls, permissions, network messages, AO behavior, or gesture asset formats.
- Do not add server-side persistence or create inventory items for favorites.
- Do not change the existing gesture editor or activation semantics.

## Next source-level investigation

Before changing C++, inspect the existing per-account settings declarations/defaults and the XUI/floater registration conventions. Then implement Favorites with the smallest possible patch and validate compilation before adding Recent history.

## Acceptance criteria

The existing gesture floater remains fully functional. Favorites can invoke existing inventory gestures through the established activation/loading/playback path, and stale UUIDs never cause a crash. No server or protocol change is needed.
