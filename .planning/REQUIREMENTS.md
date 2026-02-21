# Requirements: JUCE Plugin Template

**Defined:** 2026-02-21
**Core Value:** Clone this repo and have a compiling, Bitwig-loadable audio plugin with a working WebView UI within minutes.

## v1 Requirements

### Build System

- [x] **BUILD-01**: CMakeLists.txt compiles CLAP, VST3, and Standalone targets out of the box
- [x] **BUILD-02**: JUCE added as git submodule at libs/JUCE
- [x] **BUILD-03**: clap-juce-extensions added as git submodule at libs/clap-juce-extensions
- [x] **BUILD-04**: General audio effect defaults (IS_SYNTH=FALSE, IS_MIDI_EFFECT=FALSE)

### Source Files

- [ ] **SRC-01**: PluginProcessor.h/cpp with AudioProcessorValueTreeState parameter management
- [ ] **SRC-02**: State save/restore (getStateInformation/setStateInformation) implemented
- [ ] **SRC-03**: PluginEditor.h/cpp with WebBrowserComponent wired up
- [ ] **SRC-04**: HTML loaded via binary resource strategy (juce_add_binary_data)

### WebView UI

- [ ] **WEB-01**: web/index.html minimal scaffold with JS↔C++ bridge handlers in place
- [ ] **WEB-02**: Message format standard (JSON type/name/value) documented in HTML comments

### Dev Workflow

- [ ] **DEV-01**: install/install-linux.sh copies CLAP to ~/.clap/ and VST3 to ~/.vst3/
- [ ] **DEV-02**: .gitignore covers build/, cmake-build-*/, .cache/, .DS_Store, compile_commands.json
- [ ] **DEV-03**: README documents quickstart: clone → rename → build → install → test in Bitwig

## v2 Requirements

### Platform Scripts

- **PLAT-01**: install-macos.sh copies AU/VST3/CLAP to macOS install paths
- **PLAT-02**: install-windows.bat copies VST3/CLAP to Windows install paths

### CI/CD

- **CI-01**: GitHub Actions workflow that builds on push and verifies compilation

## Out of Scope

| Feature | Reason |
|---------|--------|
| Plugin-specific DSP | Each plugin adds its own — template must stay blank |
| Styled UI | web/index.html is scaffold only, not an opinionated starting UI |
| Preset management | Plugin-specific concern, not template-level |
| Notarization/code signing | macOS distribution concern, not part of base template |

## Traceability

| Requirement | Phase | Status |
|-------------|-------|--------|
| BUILD-01 | Phase 1 | Complete |
| BUILD-02 | Phase 1 | Complete |
| BUILD-03 | Phase 1 | Complete |
| BUILD-04 | Phase 1 | Complete |
| SRC-01 | Phase 2 | Pending |
| SRC-02 | Phase 2 | Pending |
| SRC-03 | Phase 2 | Pending |
| SRC-04 | Phase 2 | Pending |
| WEB-01 | Phase 2 | Pending |
| WEB-02 | Phase 2 | Pending |
| DEV-01 | Phase 3 | Pending |
| DEV-02 | Phase 3 | Pending |
| DEV-03 | Phase 3 | Pending |

**Coverage:**
- v1 requirements: 13 total
- Mapped to phases: 13
- Unmapped: 0 ✓

---

*Requirements defined: 2026-02-21*
*Last updated: 2026-02-21 after roadmap creation*
