# JUCE Plugin Template

## What This Is

A reusable blank-canvas repository for JUCE audio plugin development. Clone it, rename the plugin, drop in a plugin-specific brief, and you have a working build environment with WebView UI, CLAP/VST3 output, and the full dev workflow ready to go. It is not a plugin — it is the scaffolding that every plugin project starts from.

## Core Value

A developer should be able to clone this repo and have a compiling, Bitwig-loadable audio plugin with a working WebView UI within minutes, without any boilerplate decisions.

## Requirements

### Validated

(None yet — ship to validate)

### Active

- [ ] Repo builds to CLAP, VST3, and Standalone targets out of the box
- [ ] JUCE and clap-juce-extensions included as git submodules
- [ ] CMakeLists.txt is a general audio effect template (IS_SYNTH=FALSE, IS_MIDI_EFFECT=FALSE)
- [ ] WebView UI enabled with minimal scaffold HTML (JS↔C++ bridge wired, nothing else)
- [ ] Binary resource strategy: HTML compiled into binary via juce_add_binary_data
- [ ] PluginProcessor and PluginEditor base files present with standard JUCE patterns
- [ ] AudioProcessorValueTreeState parameter management in place
- [ ] State save/restore (getStateInformation/setStateInformation) implemented
- [ ] Linux install script for ~/.clap/ and ~/.vst3/ paths
- [ ] .gitignore covers build/, cmake-build-*/, .cache/, .DS_Store, etc.
- [ ] README documents quickstart: clone → rename → build → install → test in Bitwig

### Out of Scope

- Plugin-specific DSP logic — each project adds its own
- Plugin-specific UI — web/index.html is a minimal scaffold only
- macOS/Windows install scripts — documented in README, not scripted (Linux primary)
- CI/CD pipeline — manual build workflow only for now
- Preset management — plugin-specific concern

## Context

- Primary dev environment: CachyOS (Arch Linux), Bitwig Studio, PipeWire/JACK via pipewire-jack
- Secondary targets: macOS (AU + VST3), Windows (VST3) — CMake handles cross-platform
- WebView: WebBrowserComponent with WebKitGTK on Linux, WKWebView on macOS, WebView2 on Windows
- Plugin format priority: CLAP first (Bitwig native), then VST3, then AU (macOS only)
- Build system: CMake with Ninja generator on Linux
- C++ standard: C++17 minimum, prefer C++20

## Constraints

- **Tech stack**: JUCE framework — all audio/UI APIs must go through JUCE
- **Submodules**: JUCE and clap-juce-extensions are real submodules (clone with --recursive)
- **WebView HTML**: Binary resource strategy (juce_add_binary_data) for distribution; file URL during dev
- **Audio thread**: No allocations, locks, or file I/O in processBlock — pre-allocate everything
- **Plugin type**: General audio effect as base (not MIDI-only, not synth)

## Key Decisions

| Decision | Rationale | Outcome |
|----------|-----------|---------|
| Submodules included (not just documented) | Clone once, build immediately — no manual setup step | — Pending |
| General audio effect default (not MIDI) | More re-usable base; MIDI-specific projects can opt in | — Pending |
| Minimal scaffold HTML (not Hello World UI) | Template should be blank canvas, not opinionated starting UI | — Pending |
| Binary resource strategy as default | Release-ready from the start; dev mode documented as alternative | — Pending |

---
*Last updated: 2026-02-21 after initialization*
