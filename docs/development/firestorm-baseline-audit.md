# Firestorm baseline audit

Audit date: 2026-11-20

## Candidate upstream baseline

- Repository: https://github.com/FirestormViewer/phoenix-firestorm
- Candidate official release branch: `Firestorm_7.2.5`
- Pinned upstream commit: `4ac201fdc56127de27de34567286885f4e75d194`
- Branch URL: https://github.com/FirestormViewer/phoenix-firestorm/tree/Firestorm_7.2.5
- This is a candidate pin for evaluation, not yet a validated OpenSim-next build baseline.

The upstream README says Firestorm is derived from the Second Life viewer, supports Second Life and various OpenSim worlds, and advises projects creating their own viewer to use official release branches rather than preview/nightly/master.

## Important build facts from the pinned release

Windows build guide: https://github.com/FirestormViewer/phoenix-firestorm/blob/Firestorm_7.2.5/doc/building_windows.md

- Windows 10/11 64-bit.
- 64-bit builds only; upstream says 32-bit builds are no longer possible.
- Visual Studio 2022 with Desktop development with C++.
- CMake 4.1.2 or newer.
- Git for Windows.
- Python 3 and pip.
- Cygwin 64 with the patch package for testing.
- Autobuild installed from the repository requirements and Second Life autobuild; upstream guide specifies Autobuild 3.8 or newer for Windows.
- The Firestorm build variables repository is required: https://github.com/FirestormViewer/fs-build-variables
- The guide recommends an unmodified source tree for each patch/change set.

The Linux guide lists Ubuntu 22.04 x86_64, 16 GB or more RAM, 64 GB disk, and a 4-core CPU as its recommended build environment. This is another reason not to treat Linux as the first integration build target.

## OpenSim build configuration

The Windows guide lists these relevant configurations:

- `ReleaseFS_open`: no KDU, no FMOD, and **no OpenSim**.
- `ReleaseOS`: no KDU, no FMOD, **with OpenSim**.
- `RelWithDebInfoOS`: OpenSim-enabled configuration with debug information.

Therefore the first integration build must use `ReleaseOS`, not `ReleaseFS_open`. The Windows guide's command pattern is:

\`autobuild configure -A 64 -c ReleaseOS -- --chan OpenSimNext -DLL_TESTS:BOOL=FALSE\`

followed by:

\`autobuild build -A 64 -c ReleaseOS --no-configure\`

The exact switches should be confirmed against the pinned branch and build environment before execution. This avoids optional proprietary dependencies such as Kakadu/KDU and FMOD while enabling OpenSim support.

## Initial integration policy

- Preserve upstream Git history; do not copy the source tree file-by-file into the current CMake prototype.
- Keep this branch as the integration planning branch until the build environment and repository strategy are confirmed.
- Pin the upstream baseline to the commit above during the first build investigation; update the pin only through a reviewed baseline change.
- First compile the upstream viewer with minimal local changes.
- Then connect it to a test OpenSim Grid and verify login, region arrival, avatar appearance, inventory, chat and basic building.
- Only after the baseline works should OpenSim-next branding, Grid defaults and product changes be introduced.

## Known constraints and risks

1. Firestorm has a large source tree and a separate Autobuild-based dependency/build system; it is not a drop-in CMake subdirectory.
2. The current OpenSim-next CMake preview is a small architecture prototype and should not be merged wholesale into Firestorm.
3. Build dependencies are substantial and many binary packages are downloaded separately by Autobuild.
4. Some optional integrations require separately obtained/licensed dependencies. Use open configurations where suitable and audit each dependency.
5. Firestorm's repository contains LGPL-2.1 licensing information. The complete source and dependency notices must be audited before redistribution; do not assume every bundled dependency has the same license.
6. A successful compile alone is insufficient: an OpenSim Grid compatibility smoke test is required.

## Next actions

- [ ] Prepare a clean Windows 10/11 build environment with the exact toolchain above.
- [ ] Clone the pinned Firestorm source and fs-build-variables repositories.
- [ ] Configure the OpenSim-enabled `ReleaseOS` target.
- [ ] Build the unmodified baseline.
- [ ] Run the viewer and connect to a test OpenSim Grid.
- [ ] Record build commands, failures, dependency versions and smoke-test results.
- [ ] Decide whether the long-term public repository should be based on the Firestorm fork history or remain a separate integration repository linked to this prototype.
