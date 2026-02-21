---
phase: 01-build-system-foundation
plan: 01
subsystem: infra
tags: [cmake, juce, clap, vst3, ninja, audio-plugin, git-submodules]

requires: []

provides:
  - JUCE 8.0.12 git submodule at libs/JUCE
  - clap-juce-extensions (main, JUCE-8-compatible) git submodule at libs/clap-juce-extensions
  - CMakeLists.txt building CLAP, VST3, and Standalone targets
  - Minimal stub PluginProcessor and PluginEditor sources
  - Verified cmake + ninja build producing all three binary artifacts

affects:
  - 02-plugin-core-webview
  - 03-dev-workflow-documentation

tech-stack:
  added:
    - JUCE 8.0.12 (audio plugin framework via git submodule)
    - clap-juce-extensions main/02f91b7 (CLAP format wrapper, JUCE-8-compatible)
    - CMake 3.22+ with Ninja generator
    - clap 1.2.7 (nested submodule inside clap-juce-extensions)
    - clap-helpers (nested submodule inside clap-juce-extensions)
  patterns:
    - git submodule + pinned commit for reproducible dependency management
    - juce_add_plugin with clap_juce_extensions_plugin for CLAP format
    - Stub source pattern: minimal compilable stubs to be replaced in Phase 2

key-files:
  created:
    - .gitmodules
    - CMakeLists.txt
    - src/PluginProcessor.h
    - src/PluginProcessor.cpp
    - src/PluginEditor.h
    - src/PluginEditor.cpp
  modified: []

key-decisions:
  - "Used JUCE 8.0.12 (latest stable tag) rather than JUCE 7.x for long-term support"
  - "Used clap-juce-extensions main branch (not 0.26.0 tag) because 0.26.0 is incompatible with JUCE 8 - it references removed file juce_LegacyAudioParameter.cpp"
  - "CLAP ID set to com.yourcompany.plugintemplate as placeholder to be renamed on fork"
  - "PLUGIN_MANUFACTURER_CODE=Mfrc, PLUGIN_CODE=Plug as template placeholders"
  - "IS_SYNTH=FALSE, IS_MIDI_EFFECT=FALSE as general audio effect defaults per requirements"
  - "COPY_PLUGIN_AFTER_BUILD=FALSE to keep build artifacts in build/ directory"

patterns-established:
  - "cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release is the canonical configure command"
  - "ninja -C build is the canonical build command"
  - "Artifacts land at build/PluginTemplate_artefacts/Release/{CLAP,VST3,Standalone}/"

requirements-completed: [BUILD-01, BUILD-02, BUILD-03, BUILD-04]

duration: 6min
completed: 2026-02-21
---

# Phase 1 Plan 1: Build System Foundation Summary

**JUCE 8.0.12 + clap-juce-extensions CMake build producing CLAP, VST3, and Standalone binaries via git submodules and a single cmake + ninja invocation**

## Performance

- **Duration:** 6 min
- **Started:** 2026-02-21T07:43:15Z
- **Completed:** 2026-02-21T07:49:27Z
- **Tasks:** 3
- **Files modified:** 8

## Accomplishments
- Added JUCE 8.0.12 and clap-juce-extensions as git submodules (pinned commits, clone with --recursive)
- Wrote CMakeLists.txt with juce_add_plugin + clap_juce_extensions_plugin for three format targets
- Created minimal stub PluginProcessor and PluginEditor sources that compile cleanly
- Verified full cmake + ninja build succeeds producing CLAP, VST3, and Standalone artifacts

## Task Commits

Each task was committed atomically:

1. **Tasks 1-3: JUCE/clap-juce-extensions submodules, CMakeLists.txt, stub sources, and verified build** - `6f018c9` (feat)

**Plan metadata:** (created after this entry)

## Files Created/Modified
- `.gitmodules` - Submodule declarations for libs/JUCE and libs/clap-juce-extensions
- `CMakeLists.txt` - Full build definition: JUCE + clap-juce-extensions + CLAP/VST3/Standalone targets
- `src/PluginProcessor.h` - Minimal juce::AudioProcessor subclass declaration
- `src/PluginProcessor.cpp` - Pass-through implementation with stereo in/out
- `src/PluginEditor.h` - Minimal juce::AudioProcessorEditor subclass declaration
- `src/PluginEditor.cpp` - Placeholder editor (grey background, centered text)
- `libs/JUCE` - Submodule pinned to commit 29396c22 (JUCE 8.0.12)
- `libs/clap-juce-extensions` - Submodule pinned to commit 02f91b7 (main, JUCE-8-compatible)

## Decisions Made
- Used JUCE 8.0.12 (latest stable tag at time of execution) for long-term API stability
- Used clap-juce-extensions main branch (commit 02f91b7) rather than tag 0.26.0 because tag 0.26.0 references `juce_LegacyAudioParameter.cpp` which was removed in JUCE 8; main branch has explicit JUCE 8 compatibility fixes
- CLAP ID `com.yourcompany.plugintemplate`, manufacturer code `Mfrc`, plugin code `Plug` are template placeholders to be renamed per project
- IS_SYNTH=FALSE, IS_MIDI_EFFECT=FALSE, NEEDS_MIDI_INPUT=FALSE, NEEDS_MIDI_OUTPUT=FALSE set as general audio effect defaults

## Deviations from Plan

### Auto-fixed Issues

**1. [Rule 3 - Blocking] Initialized nested submodules inside clap-juce-extensions**
- **Found during:** Task 3 (cmake configure)
- **Issue:** clap-juce-extensions has nested submodules (clap and clap-helpers) that were not populated; cmake failed with "does not contain a CMakeLists.txt file"
- **Fix:** Ran `git submodule update --init --recursive` to populate clap-libs/clap and clap-libs/clap-helpers
- **Files modified:** (submodule state only, no tracked files added)
- **Verification:** cmake configured successfully after fix
- **Committed in:** 6f018c9 (part of task commit)

**2. [Rule 3 - Blocking] Switched clap-juce-extensions from tag 0.26.0 to main branch**
- **Found during:** Task 3 (ninja build)
- **Issue:** clap-juce-extensions 0.26.0 includes `<juce_audio_processors/format_types/juce_LegacyAudioParameter.cpp>` which was removed from JUCE 8; build failed with fatal error
- **Fix:** Checked out `origin/main` (commit 02f91b7) of clap-juce-extensions which has "Compatibility with JUCE 8.0.11" commit and subsequent fixes
- **Files modified:** libs/clap-juce-extensions (submodule pointer updated in .gitmodules)
- **Verification:** ninja build succeeded producing all three artifacts
- **Committed in:** 6f018c9 (part of task commit)

---

**Total deviations:** 2 auto-fixed (2 blocking)
**Impact on plan:** Both auto-fixes required for build to succeed. The clap-juce-extensions version change is a significant discovery: the latest tagged release (0.26.0) is not compatible with JUCE 8. Main branch must be used. This should be noted for anyone forking this template.

## Issues Encountered
- clap-juce-extensions 0.26.0 incompatible with JUCE 8.0.12 — resolved by using main branch

## System Dependencies (Linux/CachyOS)
All required packages were already installed on the development system:
- `alsa` (libasound)
- `freetype2` + `fontconfig`
- `gl` (OpenGL)
- `webkit2gtk-4.1`
- `gtk+-x11-3.0`

If a fresh CachyOS/Arch install is missing these, install with:
```
sudo pacman -S alsa-lib freetype2 fontconfig mesa webkit2gtk-4.1 gtk3
```

## Build Artifact Paths
- CLAP: `build/PluginTemplate_artefacts/Release/CLAP/PluginTemplate.clap`
- VST3: `build/PluginTemplate_artefacts/Release/VST3/PluginTemplate.vst3/`
- Standalone: `build/PluginTemplate_artefacts/Release/Standalone/PluginTemplate`

## User Setup Required
None - no external service configuration required. Build is fully self-contained after `git clone --recursive`.

## Next Phase Readiness
- Build system is complete and verified; Phase 2 can begin immediately
- PluginProcessor and PluginEditor stubs are in place; Phase 2 will flesh them out with AudioProcessorValueTreeState, state management, and WebView integration
- Note for Phase 3: install-linux.sh should document the system dependency list above

---
*Phase: 01-build-system-foundation*
*Completed: 2026-02-21*

## Self-Check: PASSED

All created files confirmed present. Commit 6f018c9 confirmed in git log. All three binary artifacts present in build directory.
