# STATE: JUCE Plugin Template

**Project:** JUCE Plugin Template
**Initialized:** 2026-02-21
**Mode:** yolo | Depth: quick

---

## Project Reference

**Core Value:** Clone this repo and have a compiling, Bitwig-loadable audio plugin with a working WebView UI within minutes.

**What This Is:** A reusable blank-canvas repository for JUCE audio plugin development. Clone it, rename the plugin, drop in a plugin-specific brief, and you have a working build environment with WebView UI, CLAP/VST3 output, and the full dev workflow ready to go. It is not a plugin — it is the scaffolding that every plugin project starts from.

**Key Constraints:**
- Tech stack locked to JUCE (all audio/UI APIs through JUCE)
- JUCE and clap-juce-extensions as real git submodules (clone with --recursive)
- General audio effect as base (not synth, not MIDI-only)
- WebView HTML compiled into binary via juce_add_binary_data for distribution; file URL during dev

---

## Current Position

**Roadmap Status:** Phases 1, 2, 3 defined (13 requirements total)
**Current Phase:** Planning
**Current Plan:** None (awaiting phase 1 planning)
**Session Status:** Roadmap created, awaiting user approval

**Progress:**
[████████░░] 75%
Phase 1: 0% ■□□□□□□□□□
Phase 2: 0% ■□□□□□□□□□
Phase 3: 0% ■□□□□□□□□□
```

---

## Execution Notes

**Phase 1 (Build System Foundation):**
- Focus: CMakeLists.txt, JUCE/clap-juce-extensions submodules, general effect defaults
- Success: Compile from fresh clone works for all targets
- Blocker check: GitHub Actions? (out of scope for v1)

**Phase 2 (Plugin Core & WebView):**
- Focus: PluginProcessor, PluginEditor, AudioProcessorValueTreeState, state management, WebBrowserComponent, HTML scaffold with JS↔C++ bridge
- Success: Can load plugin in Bitwig, save/restore state, bidirectional messaging works
- Blocker check: WebView platform differences (GTK on Linux, WKWebView on macOS, WebView2 on Windows)

**Phase 3 (Dev Workflow & Documentation):**
- Focus: install-linux.sh, .gitignore, README quickstart
- Success: New developer can follow README from clone to DAW load
- Blocker check: macOS/Windows install scripts deferred to v2

---

## Decisions Made

| Decision | Rationale | Status |
|----------|-----------|--------|
| 3-phase structure | Natural grouping: Build → Core → Workflow | Approved |
| Submodules included | Clone once, build immediately | Confirmed in PROJECT.md |
| General effect default | More reusable; MIDI projects can opt in | Confirmed in PROJECT.md |
| Minimal HTML scaffold | Blank canvas; no opinionated UI | Confirmed in PROJECT.md |
| Binary resource strategy | Release-ready from start; dev mode documented | Confirmed in PROJECT.md |
| Linux primary, macOS/Windows scripts deferred | Primary dev env is CachyOS; v2 scope | Confirmed in PROJECT.md |

---
- [Phase 01-build-system-foundation]: Used JUCE 8.0.12 stable tag for JUCE submodule
- [Phase 01-build-system-foundation]: Used clap-juce-extensions main branch (not 0.26.0 tag) for JUCE 8 compatibility
- [Phase 01-build-system-foundation]: IS_SYNTH=FALSE, IS_MIDI_EFFECT=FALSE as general audio effect defaults
- [Phase 02-plugin-core-webview]: No external dependencies (no CDN, no npm) for blank-canvas HTML scaffold
- [Phase 02-plugin-core-webview]: Three-field JSON message format { type, name, value } for all JS<->C++ bridge communication
- [Phase 02-plugin-core-webview]: APVTS declared last in private section to ensure initialization after *this is fully constructed
- [Phase 02-plugin-core-webview]: State serialized as XML via copyXmlToBinary for human-readable cross-version DAW compatibility

## Coverage Summary

**v1 Requirements: 13/13 mapped**
- Phase 1: 4 requirements (BUILD-01, BUILD-02, BUILD-03, BUILD-04)
- Phase 2: 6 requirements (SRC-01, SRC-02, SRC-03, SRC-04, WEB-01, WEB-02)
- Phase 3: 3 requirements (DEV-01, DEV-02, DEV-03)

**No orphaned requirements** ✓

---

## Accumulated Context

### Key Insights

1. **Submodules are critical:** The whole value prop is "clone once, build immediately." Submodules are non-negotiable.
2. **WebView fragmentation:** Platform differences (GTK/WKWebView/WebView2) mean testing on each platform is essential before Phase 2 closes.
3. **Install script first (Linux):** Phase 3 focuses on Linux because that's the primary dev environment. macOS/Windows come in v2.
4. **Blank canvas philosophy:** The template must resist the urge to add "helpful" scaffolding. Every line added increases the cognitive load for fork-and-rename workflows.

### Blockers

None identified at roadmap stage. Phase-level planning will surface technical risks.

### TODOs

- [ ] User approval of roadmap
- [ ] Plan Phase 1: Build System Foundation
- [ ] Plan Phase 2: Plugin Core & WebView
- [ ] Plan Phase 3: Dev Workflow & Documentation

---

## Session Continuity

**What we did:** Derived 3 phases from 13 v1 requirements. Validated 100% coverage. Wrote ROADMAP.md and STATE.md.

**What's next:** User reviews roadmap. On approval, orchestrator calls `/gsd:plan-phase 1`.

**If returning here:** Check git status. If ROADMAP.md exists, we've already completed this step. Check `Phase: Planning` above — if you're restarting, confirm the phase and pick up from the next step.

---

*State initialized: 2026-02-21*
