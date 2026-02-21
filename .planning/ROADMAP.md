# JUCE Plugin Template — Roadmap

**Project:** JUCE Plugin Template
**Defined:** 2026-02-21
**Status:** Draft → Planning

---

## Phases

- [ ] **Phase 1: Build System Foundation** - CMake, submodules, and compiling targets ready
- [ ] **Phase 2: Plugin Core & WebView** - Processor, editor, state management, and JS↔C++ bridge
- [ ] **Phase 3: Dev Workflow & Documentation** - Install scripts, .gitignore, and quickstart guide

---

## Phase Details

### Phase 1: Build System Foundation

**Goal:** Developers can clone the repo with `--recursive` and compile CLAP, VST3, and Standalone targets immediately without manual setup.

**Depends on:** Nothing (foundation phase)

**Requirements:** BUILD-01, BUILD-02, BUILD-03, BUILD-04

**Success Criteria** (what must be TRUE):
1. Repository clones with `git clone --recursive <url>` and all submodules are present at libs/JUCE and libs/clap-juce-extensions
2. CMakeLists.txt exists and builds all three targets (CLAP, VST3, Standalone) without configuration errors
3. Build defaults are set to general audio effect (IS_SYNTH=FALSE, IS_MIDI_EFFECT=FALSE)
4. Developer can run `cmake && ninja` from fresh clone and produce compiled binaries for all targets

**Plans:** 1 plan

Plans:
- [ ] 01-01-PLAN.md — Initialize submodules, write CMakeLists.txt, verify all targets compile

---

### Phase 2: Plugin Core & WebView

**Goal:** Plugin has functional AudioProcessor with parameter management, state save/restore, an Editor with WebBrowserComponent, and a minimal HTML scaffold with bi-directional JS↔C++ message passing ready for customization.

**Depends on:** Phase 1

**Requirements:** SRC-01, SRC-02, SRC-03, SRC-04, WEB-01, WEB-02

**Success Criteria** (what must be TRUE):
1. PluginProcessor.h/cpp includes AudioProcessorValueTreeState with at least one example parameter for developers to reference
2. Plugin correctly saves and restores state (developer can set a parameter, save, close, reopen, and parameter value persists)
3. PluginEditor.h/cpp initializes and displays a WebBrowserComponent without crashing
4. web/index.html loads successfully and displays a minimal scaffold with inline comments explaining the JS↔C++ bridge message format (type/name/value JSON)
5. Developer can call a test function from web interface that triggers C++ callback (verify bidirectional communication is wired)

**Plans:** TBD

---

### Phase 3: Dev Workflow & Documentation

**Goal:** Developers have clear instructions and tools to install the plugin into their DAW, verify it compiles cleanly, and understand the complete setup-to-test cycle.

**Depends on:** Phase 1, Phase 2

**Requirements:** DEV-01, DEV-02, DEV-03

**Success Criteria** (what must be TRUE):
1. Linux install script (install/install-linux.sh) copies compiled CLAP binary to ~/.clap/ and VST3 binary to ~/.vst3/
2. .gitignore prevents build artifacts, CMake caches, and IDE metadata from being tracked (covers build/, cmake-build-*/, .cache/, .DS_Store, compile_commands.json)
3. README contains a step-by-step quickstart that a new developer can follow: clone → rename plugin → build → install → load in Bitwig and confirm audio I/O works

**Plans:** TBD

---

## Progress

| Phase | Plans Complete | Status | Completed |
|-------|----------------|--------|-----------|
| 1. Build System Foundation | 0/1 | Planned | — |
| 2. Plugin Core & WebView | 0/? | Not started | — |
| 3. Dev Workflow & Documentation | 0/? | Not started | — |

---

## Coverage

**Total v1 requirements:** 13
**Mapped to phases:** 13
**Unmapped:** 0 ✓

| Requirement | Phase | Status |
|-------------|-------|--------|
| BUILD-01 | Phase 1 | Pending |
| BUILD-02 | Phase 1 | Pending |
| BUILD-03 | Phase 1 | Pending |
| BUILD-04 | Phase 1 | Pending |
| SRC-01 | Phase 2 | Pending |
| SRC-02 | Phase 2 | Pending |
| SRC-03 | Phase 2 | Pending |
| SRC-04 | Phase 2 | Pending |
| WEB-01 | Phase 2 | Pending |
| WEB-02 | Phase 2 | Pending |
| DEV-01 | Phase 3 | Pending |
| DEV-02 | Phase 3 | Pending |
| DEV-03 | Phase 3 | Pending |

---

*Roadmap created: 2026-02-21*
