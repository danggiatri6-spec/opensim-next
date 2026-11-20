# Firestorm source bootstrap for OpenSim-next

This document turns the pinned-source decision into a repeatable repository and Windows build procedure.

## 0. Important status

- Upstream: `FirestormViewer/phoenix-firestorm`
- Release branch: `Firestorm_7.2.5`
- Pinned commit: `4ac201fdc56127de27de34567286885f4e75d194`
- OpenSim build configuration: `ReleaseOS`
- No source fork/import or successful build is claimed by this document.

The current ChatGPT GitHub connection can create files and branches in `opensim-next`, but does not expose the GitHub Fork operation. The repository owner must create the fork through GitHub's web UI once.

## 1. Create the source repository

1. Sign in to GitHub as the owner of the OpenSim-next repositories.
2. Open https://github.com/FirestormViewer/phoenix-firestorm
3. Click **Fork**.
4. Select the personal account that owns `opensim-next`.
5. Name the fork `opensim-next-viewer`.
6. Keep the fork public if the project intends to distribute source and binaries under the applicable upstream and dependency licenses.
7. Create the fork. Do not use GitHub's template or download ZIP; a real fork preserves the upstream relationship and Git history.

Expected fork URL:

`https://github.com/danggiatri6-spec/opensim-next-viewer`

If GitHub reports that the account already has a fork, inspect it first rather than creating a second copy.

## 2. Pin the release source locally

Run these commands in Windows Command Prompt after Git for Windows is installed:

```bat
cd /d C:\
git clone https://github.com/danggiatri6-spec/opensim-next-viewer.git
cd opensim-next-viewer
git remote add upstream https://github.com/FirestormViewer/phoenix-firestorm.git
git fetch upstream --tags
git checkout -b opensim-next/firestorm-7.2.5 4ac201fdc56127de27de34567286885f4e75d194
git push -u origin opensim-next/firestorm-7.2.5
git remote -v
git rev-parse HEAD
```

The final `git rev-parse HEAD` must print:

`4ac201fdc56127de27de34567286885f4e75d194`

Keep the remote names:
- `origin`: the personal `opensim-next-viewer` fork
- `upstream`: the official Firestorm source repository

Do not commit OpenSim-next product changes directly to the pinned baseline branch. Create a separate development branch after the untouched baseline has been built and tested.

## 3. Windows build machine

Use a dedicated Windows 10/11 x64 machine or VM. Do not use Windows 7 for this baseline.

Follow the pinned upstream Windows guide:
https://github.com/FirestormViewer/phoenix-firestorm/blob/4ac201fdc56127de27de34567286885f4e75d194/doc/building_windows.md

Required tools documented by upstream:
- Windows 10/11 64-bit
- Visual Studio 2022 with **Desktop development with C++**
- CMake 4.1.2 or newer
- Git for Windows
- Cygwin 64-bit with the `Devel/patch` package (Cygwin terminal is used for the intermediate checks; actual builds run from Windows Command Prompt)
- Python 3 and pip
- Autobuild 3.8 or newer
- The upstream `fs-build-variables` repository

Set the Visual Studio version for Autobuild to 170 (Visual Studio 2022).

## 4. Install Python build requirements

From the checked-out Firestorm source directory, use Windows Command Prompt:

```bat
python -m pip install -r requirements.txt
python -m pip install git+https://github.com/secondlife/autobuild.git#egg=autobuild
autobuild --version
```

Upstream's Windows guide requires Autobuild 3.8 or newer. The source `requirements.txt` pins the project's Python build dependencies; keep the command sequence aligned with the pinned source's current guide.

## 5. Configure build variables

Clone the official build variables repository outside the Firestorm source tree:

```bat
cd /d C:\
git clone https://github.com/FirestormViewer/fs-build-variables.git fs-build-variables
```

For the current Command Prompt session, set:

```bat
set AUTOBUILD_VSVER=170
set AUTOBUILD_VARIABLES_FILE=C:\fs-build-variables\variables
```

The path must point to the actual `variables` file in that checkout.

## 6. Build the untouched OpenSim-enabled baseline

From the Firestorm source directory, in Windows Command Prompt (cmd.exe, not PowerShell):

```bat
autobuild configure -A 64 -c ReleaseOS -- --chan OpenSimNext -DLL_TESTS:BOOL=FALSE
autobuild build -A 64 -c ReleaseOS --no-configure
```

Why `ReleaseOS`:
- Includes OpenSim support.
- Does not require KDU/Kakadu.
- Does not require FMOD.
- Is the correct first target for an OpenSim-focused client.

Do not substitute `ReleaseFS_open`: upstream documents it as having no OpenSim support.

The first configure/build can download and compile a large dependency set and may take substantial time and disk space. Do not interrupt it merely because dependency download output appears quiet.

## 7. Baseline acceptance checklist

Do not begin product-level modifications until the untouched build has passed these checks on a test OpenSim Grid:

- [ ] Viewer executable launches on Windows 10/11 x64.
- [ ] Login succeeds against the test Grid.
- [ ] Region connection and terrain render work.
- [ ] Avatar appears, moves and animates.
- [ ] Inventory folders and assets load.
- [ ] Local chat and avatar-to-avatar IM work.
- [ ] Basic object selection, edit and movement work.
- [ ] Texture and asset cache survive a restart.
- [ ] No required paid/optional dependency was silently assumed.

Record the exact commit, build configuration, compiler version, build log and test Grid version with the result.

## 8. Product development after baseline

After the baseline passes, create a product branch:

```bat
git checkout -b opensim-next/dev
git push -u origin opensim-next/dev
```

Keep changes small and traceable:
1. Product channel/name and branding.
2. Default Grid entry and login experience.
3. OpenSim-next settings and performance presets.
4. AI-assisted translation and creation workflows.
5. Renderer changes only after baseline profiling and feature-parity tests.

Keep the untouched pinned baseline available. Upstream's Windows guide recommends using a clean source tree for each patch/change set; use separate clones or Git worktrees when preparing isolated patches.

## 9. Upstream references

- Firestorm source at pinned commit: https://github.com/FirestormViewer/phoenix-firestorm/tree/4ac201fdc56127de27de34567286885f4e75d194
- Windows build guide: https://github.com/FirestormViewer/phoenix-firestorm/blob/4ac201fdc56127de27de34567286885f4e75d194/doc/building_windows.md
- Build variables: https://github.com/FirestormViewer/fs-build-variables
- Python requirements: https://github.com/FirestormViewer/phoenix-firestorm/blob/4ac201fdc56127de27de34567286885f4e75d194/requirements.txt
