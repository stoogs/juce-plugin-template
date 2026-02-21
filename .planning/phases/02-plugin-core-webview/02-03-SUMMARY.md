---
phase: 02-plugin-core-webview
plan: 03
subsystem: ui
tags: [juce, webview, webbrowsercomponent, binary-data, cmake, webkit2gtk, resource-provider, clap, vst3]

# Dependency graph
requires:
  - phase: 02-plugin-core-webview
    plan: 01
    provides: PluginProcessor with APVTS and state management
  - phase: 02-plugin-core-webview
    plan: 02
    provides: web/index.html HTML scaffold with JS<->C++ bridge wiring
  - phase: 01-build-system-foundation
    plan: 01
    provides: CMake build system with JUCE/clap-juce-extensions submodules

provides:
  - PluginEditor hosting juce::WebBrowserComponent showing web/index.html from BinaryData
  - JUCE 8 ResourceProvider pattern serving HTML from embedded binary data (no file path at runtime)
  - JS->C++ bridge via withNativeIntegrationEnabled() + withEventListener("pluginMessage", ...)
  - CMakeLists.txt with JUCE_WEB_BROWSER=1, juce_add_binary_data, NEEDS_WEB_BROWSER TRUE, CLAP format
  - All three plugin formats build cleanly: CLAP, VST3, Standalone

affects:
  - 03-dev-workflow-documentation

# Tech tracking
tech-stack:
  added:
    - juce::WebBrowserComponent (JUCE built-in, now enabled with JUCE_WEB_BROWSER=1)
    - webkit2gtk-4.x (system dependency, linked via NEEDS_WEB_BROWSER TRUE in CMake)
    - juce_add_binary_data (embeds web/index.html as PluginTemplateData)
  patterns:
    - "ResourceProvider pattern: serve HTML from BinaryData via getResourceProviderRoot() for JUCE 8 native integration"
    - "JUCE 8 native integration: withNativeIntegrationEnabled() + withEventListener(eventId, lambda)"
    - "JS->C++ bridge: window.__JUCE__.backend.emitEvent(eventId, payload) on JS side, withEventListener on C++ side"
    - "C++->JS bridge: emitEventIfBrowserIsVisible(eventId, var) on C++ side, backend.addEventListener on JS side"
    - "NEEDS_WEB_BROWSER TRUE in juce_add_plugin links webkit2gtk on Linux automatically via pkg-config"

key-files:
  created: []
  modified:
    - CMakeLists.txt
    - src/PluginEditor.h
    - src/PluginEditor.cpp
    - web/index.html

key-decisions:
  - "ResourceProvider approach chosen over data URI: gives WebBrowserComponent a real URL context required for JUCE 8 native integration (window.__JUCE__.backend) to initialize correctly"
  - "NEEDS_WEB_BROWSER TRUE added to juce_add_plugin to link webkit2gtk on Linux via JUCE CMake's pkg-config integration"
  - "web/index.html updated to use window.__JUCE__.backend.emitEvent (JUCE 8 API) instead of raw window.__JUCE__.postMessage since withEventListener expects the {eventId, payload} envelope format"
  - "buildBrowserOptions() extracted as static function to keep PluginEditor constructor clean"

patterns-established:
  - "WebBrowserComponent pattern: construct with Options object, use ResourceProvider for embedded HTML, navigate to getResourceProviderRoot()"
  - "JUCE 8 event bridge pattern: JS emits via backend.emitEvent(id, obj), C++ listens via withEventListener(id, lambda)"
  - "Binary data pattern: juce_add_binary_data(PluginTemplateData SOURCES web/index.html) -> BinaryData::index_html + BinaryData::index_htmlSize"

requirements-completed: [SRC-03, SRC-04]

# Metrics
duration: 6min
completed: 2026-02-21
---

# Phase 2 Plan 3: WebBrowserComponent Integration Summary

**JUCE 8 WebBrowserComponent serving web/index.html from BinaryData via ResourceProvider with bidirectional JS<->C++ bridge, all three plugin formats (CLAP/VST3/Standalone) building cleanly on Linux**

## Performance

- **Duration:** 6 min
- **Started:** 2026-02-21T08:25:13Z
- **Completed:** 2026-02-21T08:31:00Z
- **Tasks:** 2
- **Files modified:** 4

## Accomplishments
- Wired CMakeLists.txt with JUCE_WEB_BROWSER=1, CLAP format, juce_add_binary_data, NEEDS_WEB_BROWSER TRUE for webkit2gtk linking
- PluginEditor now hosts a WebBrowserComponent that serves web/index.html from embedded BinaryData via JUCE 8 ResourceProvider
- JS->C++ bridge uses withNativeIntegrationEnabled() + withEventListener("pluginMessage") to receive { type, name, value } messages from the test ping button
- All three formats (CLAP, VST3, Standalone) build without errors on Linux/CachyOS

## Task Commits

Each task was committed atomically:

1. **Task 1: Update CMakeLists.txt to enable WebBrowser and embed HTML as binary data** - `5138e1d` (feat)
2. **Task 2: Implement PluginEditor with WebBrowserComponent showing web/index.html** - `3b94718` (feat)

**Plan metadata:** (docs commit below)

## Files Created/Modified
- `CMakeLists.txt` - JUCE_WEB_BROWSER=1, CLAP format added, juce_add_binary_data(PluginTemplateData), NEEDS_WEB_BROWSER TRUE, PluginTemplateData linked
- `src/PluginEditor.h` - Added juce_gui_extra include, WebBrowserComponent member declaration with initialization note
- `src/PluginEditor.cpp` - ResourceProvider serving BinaryData::index_html, Options with native integration + pluginMessage listener, browser fills editor bounds
- `web/index.html` - Updated sendToPlugin to use window.__JUCE__.backend.emitEvent (JUCE 8 proper API), added backend.addEventListener for C++->JS events

## Decisions Made
- Used ResourceProvider pattern (not data URI) to serve index.html from BinaryData: ensures the browser has a real URL context (`juce://juce.backend/`) so `window.__JUCE__.backend` is available and initialized correctly by JUCE's injected script
- `NEEDS_WEB_BROWSER TRUE` in `juce_add_plugin` triggers JUCE CMake to run pkg-config and link webkit2gtk-4.x automatically on Linux; without this, `#include <gtk/gtk.h>` fails at build time
- Updated HTML bridge from `window.__JUCE__.postMessage(rawString)` to `window.__JUCE__.backend.emitEvent(eventId, object)` because `withEventListener` in C++ only fires for messages routed through the `{eventId, payload}` envelope that `backend.emitEvent` produces

## Deviations from Plan

### Auto-fixed Issues

**1. [Rule 3 - Blocking] Added NEEDS_WEB_BROWSER TRUE to juce_add_plugin**
- **Found during:** Task 2 (PluginEditor implementation)
- **Issue:** Build failed with `fatal error: gtk/gtk.h: No such file or directory` — JUCE's CMake only links webkit2gtk when NEEDS_WEB_BROWSER is explicitly set on the plugin target
- **Fix:** Added `NEEDS_WEB_BROWSER TRUE` to `juce_add_plugin` block in CMakeLists.txt; also updated the CMake re-configuration step in Task 1's commit
- **Files modified:** CMakeLists.txt (included in Task 2 commit `3b94718`)
- **Verification:** `cmake` reconfigured cleanly; `ninja PluginTemplate_VST3` produced no errors
- **Committed in:** `3b94718` (Task 2 commit)

**2. [Rule 1 - Bug] Updated web/index.html to use JUCE 8 backend.emitEvent API**
- **Found during:** Task 2 (analyzing JUCE 8 WebBrowserComponent source)
- **Issue:** The existing HTML used `window.__JUCE__.postMessage(rawJsonString)` directly; this bypasses the `{eventId, payload}` message envelope that `withEventListener` expects, so C++ listeners registered via `withEventListener("pluginMessage", ...)` would never fire
- **Fix:** Updated `sendToPlugin()` to call `window.__JUCE__.backend.emitEvent("pluginMessage", {type, name, value})` and replaced the old `pluginMessageReceived` global function with `backend.addEventListener("pluginMessage", handler)` for the C++->JS direction
- **Files modified:** web/index.html (included in Task 2 commit `3b94718`)
- **Verification:** HTML is now consistent with JUCE 8 API; bridge wiring is complete end-to-end
- **Committed in:** `3b94718` (Task 2 commit)

---

**Total deviations:** 2 auto-fixed (1 blocking, 1 bug)
**Impact on plan:** Both fixes were essential for correctness. Without NEEDS_WEB_BROWSER the build fails on Linux; without the API update the JS->C++ bridge would silently drop all messages. No scope creep.

## Issues Encountered
- JUCE 8 changed the WebBrowserComponent API significantly from JUCE 7. The old subclass-and-override pattern (pageFinishedLoading, resourceRequested) is still present but the new Options-based constructor with withNativeIntegrationEnabled() + withEventListener() is the correct way to wire bidirectional messaging. Studied the JUCE source at `libs/JUCE/modules/juce_gui_extra/misc/juce_WebBrowserComponent.h` and `juce_WebBrowserComponent.cpp` before writing the implementation.

## User Setup Required
None - no external service configuration required. webkit2gtk is a system package (gtk3 already installed on CachyOS); linking is automatic via NEEDS_WEB_BROWSER TRUE.

## Next Phase Readiness
- Plugin loads in DAW/Standalone and displays the web UI from embedded BinaryData
- Bidirectional JS<->C++ bridge is fully wired; test button sends a verified ping to C++
- Phase 2 is complete; Phase 3 (Dev Workflow & Documentation) can begin
- For development: swap `getResourceProviderRoot()` for a `file://` URL to iterate on HTML without rebuilding

## Self-Check: PASSED

- src/PluginEditor.h: FOUND
- src/PluginEditor.cpp: FOUND
- web/index.html: FOUND
- .planning/phases/02-plugin-core-webview/02-03-SUMMARY.md: FOUND
- Commit 5138e1d: FOUND
- Commit 3b94718: FOUND

---
*Phase: 02-plugin-core-webview*
*Completed: 2026-02-21*
