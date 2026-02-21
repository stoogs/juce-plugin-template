# JUCE Plugin Development Environment — Base Brief

## Purpose

This document defines a reusable development environment, project template, and build workflow for audio plugin development. It is designed to be cloned as a starting point for any new plugin project. Plugin-specific briefs (features, UI, DSP logic) are dropped in alongside this base.

**This brief does NOT change between projects.** It covers toolchain, project structure, build system, testing workflow, and cross-platform packaging. If using GSD, feed this into `/gsd:new-project` or `/gsd:map-codebase` as foundational context before any plugin-specific brief.

---

## Developer Profile

- **Primary OS:** CachyOS (Arch Linux, rolling release, cutting-edge packages)
- **Primary DAW:** Bitwig Studio (Linux-native, first-class CLAP support)
- **Secondary targets:** macOS (AU + VST3), Windows (VST3)
- **Preferred plugin format:** CLAP (primary), VST3 (cross-platform), AU (macOS only)
- **UI approach:** WebView-based (HTML/CSS/JS rendered inside JUCE's WebBrowserComponent)
- **Build system:** CMake
- **Language:** C++17 minimum (C++20 preferred if JUCE version supports it)
- **AI tooling:** Claude Code with GSD (spec-driven development)

---

## System Dependencies (CachyOS / Arch)

### Required Packages

```bash
# Core build tools
sudo pacman -S base-devel cmake ninja gcc clang git

# JUCE dependencies (X11, graphics, fonts)
sudo pacman -S libx11 libxrandr libxinerama libxcursor libxcomposite libxext
sudo pacman -S freetype2 fontconfig mesa libgl

# Audio backends
sudo pacman -S alsa-lib alsa-utils
sudo pacman -S jack2  # or pipewire-jack if using PipeWire

# WebView (required for WebBrowserComponent on Linux)
sudo pacman -S webkit2gtk-4.1 gtk4 libsoup3

# Optional but recommended
sudo pacman -S curl libcurl-gnutls  # for network features
sudo pacman -S pkg-config           # CMake dependency resolution
```

### Verify Installation

```bash
# Check WebKitGTK is available
pkg-config --modversion webkit2gtk-4.1

# Check JUCE can find audio
aplay -l  # ALSA devices
jack_lsp  # JACK ports (if running)
```

### CachyOS-Specific Notes

- **Rolling release means bleeding-edge packages.** GCC, GTK, and WebKitGTK versions will often be ahead of what JUCE officially tests against. This is usually fine but watch for deprecation warnings, especially in GTK4 APIs.
- **PipeWire is default on CachyOS.** JACK applications (including JUCE standalone builds) work through `pipewire-jack`. No extra configuration needed.
- **Wayland vs X11:** CachyOS may default to Wayland. JUCE's Wayland support is limited. Run DAW and standalone plugins under X11 or XWayland for reliable behaviour. If using Hyprland or Sway, ensure XWayland is enabled.
- **Kernel:** CachyOS uses performance-tuned kernels. The `linux-cachyos` kernel includes low-latency patches suitable for audio work. No need for a separate RT kernel.

---

## Project Template Structure

Every plugin project follows this layout:

```
plugin-name/
├── CMakeLists.txt                 # Root build configuration
├── README.md                      # Plugin overview
├── docs/
│   ├── base-brief.md              # This file (symlinked or copied)
│   └── plugin-brief.md            # Plugin-specific features/spec
├── libs/
│   ├── JUCE/                      # JUCE framework (git submodule)
│   └── clap-juce-extensions/      # CLAP wrapper (git submodule)
├── src/
│   ├── PluginProcessor.h
│   ├── PluginProcessor.cpp
│   ├── PluginEditor.h
│   ├── PluginEditor.cpp
│   └── (plugin-specific source files)
├── web/                           # WebView UI assets
│   └── index.html                 # Main UI (self-contained HTML)
├── resources/                     # Icons, presets, metadata
│   ├── icon.png
│   └── (platform-specific resources)
├── build/                         # Build output (gitignored)
├── install/                       # Local install scripts
│   └── install-linux.sh
└── .gitignore
```

### Git Submodule Setup

```bash
# Initialize a new plugin project
mkdir plugin-name && cd plugin-name
git init

# Add JUCE and CLAP extensions as submodules
git submodule add https://github.com/juce-framework/JUCE.git libs/JUCE
git submodule add https://github.com/free-audio/clap-juce-extensions.git libs/clap-juce-extensions

# Pin to known-good versions (update these periodically)
cd libs/JUCE && git checkout master && cd ../..
cd libs/clap-juce-extensions && git checkout main && cd ../..

git submodule update --init --recursive
```

### .gitignore

```gitignore
build/
cmake-build-*/
.cache/
.idea/
*.user
compile_commands.json

# OS
.DS_Store
Thumbs.db

# GSD planning files (optional — GSD recommends tracking these)
# .planning/
```

---

## CMake Template

This is the base `CMakeLists.txt` that every project starts from. Replace `PluginName`, IDs, and source lists per project.

```cmake
cmake_minimum_required(VERSION 3.22)
project(PluginName VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── JUCE ──
add_subdirectory(libs/JUCE)

# ── CLAP extensions ──
add_subdirectory(libs/clap-juce-extensions EXCLUDE_FROM_ALL)

# ── Plugin target ──
juce_add_plugin(PluginName
    COMPANY_NAME           "YourCompany"
    PLUGIN_MANUFACTURER_CODE Ycmp
    PLUGIN_CODE             Plgn
    FORMATS                 VST3 AU Standalone    # CLAP added below via extensions
    PRODUCT_NAME            "Plugin Name"
    
    IS_SYNTH                FALSE                 # TRUE for instruments
    NEEDS_MIDI_INPUT        TRUE
    NEEDS_MIDI_OUTPUT       TRUE
    IS_MIDI_EFFECT          TRUE                  # TRUE for MIDI-only plugins
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE

    # Plugin categories
    VST3_CATEGORIES         "Tools"               # Adjust per plugin
    AU_MAIN_TYPE            "aumi"                # kAudioUnitType_MIDIProcessor

    # Bundle ID (macOS)
    BUNDLE_ID               "com.yourcompany.pluginname"

    # Icon
    # ICON_BIG              "resources/icon.png"
)

# ── Source files ──
target_sources(PluginName PRIVATE
    src/PluginProcessor.cpp
    src/PluginEditor.cpp
    # Add plugin-specific sources here
)

# ── JUCE modules ──
target_link_libraries(PluginName
    PRIVATE
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_plugin_client
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_core
        juce::juce_data_structures
        juce::juce_events
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra           # Required for WebBrowserComponent
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

# ── Compile definitions ──
target_compile_definitions(PluginName
    PUBLIC
        JUCE_WEB_BROWSER=1             # Enable WebBrowserComponent
        JUCE_USE_CURL=0                 # Disable CURL (not needed for WebView)
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_DISPLAY_SPLASH_SCREEN=0   # No JUCE splash (requires license compliance)
        JUCE_MODAL_LOOPS_PERMITTED=0
)

# ── CLAP wrapper ──
clap_juce_extensions_plugin(
    TARGET PluginName
    CLAP_ID "com.yourcompany.pluginname"
    CLAP_FEATURES "note-effect" "utility"   # Adjust per plugin
    # CLAP_PROCESS_EVENTS_RESOLUTION_SAMPLES 64  # Optional: sample-accurate events
)

# ── WebView assets: embed HTML into binary or copy to resources ──
# Option A: Binary resource (compiled into plugin, no external files)
juce_add_binary_data(PluginAssets
    SOURCES
        web/index.html
)
target_link_libraries(PluginName PRIVATE PluginAssets)

# Option B: Copy to build output (external file, easier to iterate)
# add_custom_command(TARGET PluginName POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy
#         ${CMAKE_SOURCE_DIR}/web/index.html
#         $<TARGET_FILE_DIR:PluginName>/web/index.html
# )
```

### Build Commands

```bash
# Configure (first time)
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build --config Release

# Build specific format only
cmake --build build --target PluginName_CLAP
cmake --build build --target PluginName_VST3
cmake --build build --target PluginName_Standalone

# Debug build (for gdb/lldb)
cmake -B build-debug -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build-debug
```

---

## WebView Integration Pattern

### PluginEditor Base Pattern

Every WebView-based plugin editor follows this structure:

```cpp
// PluginEditor.h
#pragma once
#include <juce_gui_extra/juce_gui_extra.h>
#include "PluginProcessor.h"

class PluginEditor : public juce::AudioProcessorEditor
{
public:
    explicit PluginEditor(PluginProcessor&);
    ~PluginEditor() override;

    void resized() override;

private:
    PluginProcessor& processor;
    juce::WebBrowserComponent webView;

    void setupWebView();
    void sendToJS(const juce::String& functionName, const juce::var& data);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
};
```

### Loading HTML

Two strategies for getting the HTML into the WebView:

**Strategy A: Binary Resource (recommended for distribution)**
```cpp
// HTML compiled into binary via juce_add_binary_data
#include "BinaryData.h"

void PluginEditor::setupWebView()
{
    auto html = juce::String::fromUTF8(BinaryData::index_html, BinaryData::index_htmlSize);
    webView.goToURL("data:text/html;charset=utf-8," + juce::URL::addEscapeChars(html, true));
}
```

**Strategy B: File URL (recommended during development)**
```cpp
void PluginEditor::setupWebView()
{
    auto htmlFile = juce::File::getSpecialLocation(juce::File::currentExecutableFile)
                        .getParentDirectory().getChildFile("web/index.html");
    webView.goToURL(htmlFile.getFullPathName());
}
```

Use Strategy B during development for hot-reload (edit HTML, reopen plugin window). Switch to Strategy A for release builds.

### JS ↔ C++ Message Bridge

JUCE's WebBrowserComponent supports bidirectional communication. The exact API depends on JUCE version — research the current `WebBrowserComponent` API during Phase 1 of any project, as this has evolved across JUCE 7 and 8.

**General pattern:**

```
C++ → JS:  webView.evaluateJavascript("window.handleMessage(" + jsonString + ")");
JS → C++:  window.__JUCE__.backend.emitEvent("eventName", data);
           // or via page navigation interception as fallback
```

**Message format standard (use across all plugins):**

```json
{
    "type": "parameterChange",
    "name": "strumSpeed",
    "value": 42
}
```

```json
{
    "type": "midiNoteOn",
    "note": 60,
    "velocity": 100,
    "channel": 1
}
```

Always use JSON for the bridge. It's debuggable, extensible, and consistent across platforms.

---

## Plugin Processor Base Pattern

### Parameter Setup

```cpp
juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // Example patterns — replace per plugin
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "paramId", "Param Name", juce::NormalisableRange<float>(0.0f, 1.0f), 0.5f));

    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        "modeId", "Mode Name", juce::StringArray{"Off", "On", "Auto"}, 0));

    params.push_back(std::make_unique<juce::AudioParameterBool>(
        "toggleId", "Toggle Name", false));

    return { params.begin(), params.end() };
}
```

### State Save/Restore

```cpp
void PluginProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void PluginProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml && xml->hasTagName(parameters.state.getType()))
        parameters.replaceState(juce::ValueTree::fromXml(*xml));
}
```

### MIDI Processing Pattern

```cpp
void PluginProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    buffer.clear();  // MIDI effect — no audio processing

    juce::MidiBuffer processedMidi;

    for (const auto metadata : midiMessages)
    {
        auto message = metadata.getMessage();
        int samplePosition = metadata.samplePosition;

        if (message.isNoteOn())
        {
            int note = message.getNoteNumber();
            int velocity = message.getVelocity();

            // Route to appropriate handler based on note zone
            // Plugin-specific logic here
        }
        else if (message.isNoteOff())
        {
            // Handle note off
        }
        else
        {
            // Pass through CC, pitch bend, etc.
            processedMidi.addEvent(message, samplePosition);
        }
    }

    midiMessages.swapWith(processedMidi);
}
```

---

## Testing Workflow

### Local Install Script (Linux)

```bash
#!/bin/bash
# install/install-linux.sh
# Copies built plugins to standard scan paths

PLUGIN_NAME="PluginName"
BUILD_DIR="build"

# CLAP
mkdir -p ~/.clap
cp -r "${BUILD_DIR}/${PLUGIN_NAME}_artefacts/Release/CLAP/${PLUGIN_NAME}.clap" ~/.clap/

# VST3
mkdir -p ~/.vst3
cp -r "${BUILD_DIR}/${PLUGIN_NAME}_artefacts/Release/VST3/${PLUGIN_NAME}.vst3" ~/.vst3/

echo "Installed ${PLUGIN_NAME} to ~/.clap/ and ~/.vst3/"
echo "Rescan plugins in Bitwig: Settings → Plug-in Locations → Rescan"
```

### Bitwig Testing Checklist

Run through this for every plugin build:

1. **Plugin loads:** Bitwig scans and lists the plugin (CLAP preferred)
2. **UI renders:** WebView displays correctly, no blank/white panel
3. **MIDI input:** Play notes on controller, verify plugin receives them
4. **MIDI output:** Downstream instruments receive processed MIDI
5. **Parameters:** Automate a parameter in Bitwig, verify UI updates
6. **State recall:** Save Bitwig project, close, reopen — plugin state restores
7. **Multiple instances:** Load 2+ instances, verify independent state
8. **CPU usage:** Check Bitwig's CPU meter during playback

### Bitwig Plugin Scan Paths

```
CLAP:  ~/.clap/
VST3:  ~/.vst3/
```

If Bitwig doesn't see the plugin after copying, go to:
`Settings → Plug-in Locations → Rescan`

### Standalone Testing

For quick iteration without loading a DAW:

```bash
./build/PluginName_artefacts/Release/Standalone/PluginName
```

The standalone build includes a basic MIDI keyboard and audio output. Useful for testing WebView rendering and basic MIDI logic without Bitwig overhead.

---

## Cross-Platform Build Notes

### macOS

```bash
# Requires Xcode command line tools
cmake -B build -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Outputs: .component (AU), .vst3, .clap
# Install paths: ~/Library/Audio/Plug-Ins/Components/ (AU)
#                ~/Library/Audio/Plug-Ins/VST3/ (VST3)
#                ~/Library/Audio/Plug-Ins/CLAP/ (CLAP)
```

- WebView uses WKWebView (always available on macOS 11+)
- AU requires code signing for distribution
- Universal binary (arm64 + x86_64): add `-DCMAKE_OSX_ARCHITECTURES="arm64;x86_64"`

### Windows

```bash
# Requires Visual Studio 2022 or Build Tools
cmake -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release

# Outputs: .vst3, .clap
# Install paths: C:\Program Files\Common Files\VST3\ (VST3)
#                C:\Program Files\Common Files\CLAP\ (CLAP)
```

- WebView uses WebView2 (Edge Chromium), auto-bundled by JUCE
- WebView2 requires the Edge WebView2 Runtime (pre-installed on Windows 10+)

---

## Coding Conventions

### Naming

- **Classes:** PascalCase (`ChordEngine`, `PluginProcessor`)
- **Methods:** camelCase (`processBlock`, `getChordNotes`)
- **Members:** camelCase with no prefix (`strumSpeed`, `activeModifier`)
- **Constants:** UPPER_SNAKE (`MAX_VOICES`, `MIDI_CHANNEL`)
- **Files:** PascalCase matching class name (`ChordEngine.h`, `ChordEngine.cpp`)
- **Parameters:** camelCase ID, Title Case display name (`"strumSpeed"`, `"Strum Speed"`)

### Code Style

- Braces on same line (`if (x) {`)
- 4-space indentation
- `#pragma once` for header guards
- `const` and `override` everywhere applicable
- Prefer `std::unique_ptr` and RAII, avoid raw `new`/`delete`
- No `using namespace juce;` in headers (qualify with `juce::`)

### JUCE-Specific

- Never block the audio thread (no allocations, no locks, no file I/O in `processBlock`)
- Use `juce::AudioProcessorValueTreeState` for all parameters
- Use `juce::MessageManager::callAsync` for UI updates from audio thread
- Prefer `juce::Timer` for UI refresh, not polling in `processBlock`

---

## Dependency Version Tracking

Track pinned versions in the project README or a `VERSIONS.md`:

```markdown
| Dependency | Version | Updated |
|-----------|---------|---------|
| JUCE | 8.x.x | YYYY-MM-DD |
| clap-juce-extensions | latest main | YYYY-MM-DD |
| WebKitGTK (system) | x.x.x | rolling |
| CMake | 3.22+ | system |
| GCC | 14.x | system |
```

Update JUCE and clap-juce-extensions submodules periodically:

```bash
cd libs/JUCE && git pull origin master && cd ../..
cd libs/clap-juce-extensions && git pull origin main && cd ../..
git add libs/ && git commit -m "chore: update JUCE and CLAP extensions"
```

---

## GSD Integration

When using GSD with this base:

1. **First project:** Run `/gsd:new-project`, provide both this brief and the plugin-specific brief. GSD will generate PROJECT.md, REQUIREMENTS.md, ROADMAP.md.

2. **Subsequent projects:** Clone the template repo, drop in a new plugin brief, run `/gsd:new-project`. Reference this base brief as "the dev environment is already defined, see docs/base-brief.md" — GSD will skip environment questions and focus on plugin-specific planning.

3. **Phase 1 of every project** is always the same: scaffold from this template, verify WebView loads in Bitwig, confirm CLAP/VST3 build. GSD can auto-execute this phase since it's identical across projects.

4. **`/gsd:map-codebase`** should be run on the template repo once to establish baseline architecture understanding. GSD will then know the project structure, CMake setup, and WebView pattern for all future phases.

---

## Template Repository Quickstart

To create a new plugin project from this template:

```bash
# Clone template
git clone --recursive https://github.com/youruser/juce-plugin-template.git new-plugin-name
cd new-plugin-name

# Remove template git history, start fresh
rm -rf .git
git init
git submodule update --init --recursive

# Rename plugin
# 1. Update CMakeLists.txt: PluginName → YourPlugin
# 2. Update CLAP_ID, BUNDLE_ID, COMPANY_NAME
# 3. Rename src/ files if desired
# 4. Drop plugin-specific brief into docs/plugin-brief.md
# 5. Drop UI HTML into web/index.html

# Verify build
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Test
./build/YourPlugin_artefacts/Release/Standalone/YourPlugin

# Install to Bitwig
bash install/install-linux.sh
```

---

## Troubleshooting

### WebView shows blank/white panel
- Check WebKitGTK is installed: `pkg-config --modversion webkit2gtk-4.1`
- Verify `JUCE_WEB_BROWSER=1` in CMake definitions
- Check `juce_gui_extra` is linked
- Try loading a simple `<h1>Hello</h1>` to isolate HTML vs WebView issues

### Plugin not found in Bitwig
- Verify file is in `~/.clap/` or `~/.vst3/`
- Check file permissions (`chmod 755`)
- Rescan in Bitwig settings
- Check Bitwig's plugin crash log: `~/.BitwigStudio/log/`

### Build fails on CachyOS
- Update submodules: `git submodule update --init --recursive`
- Check GCC version compatibility with JUCE: `gcc --version`
- If WebKitGTK headers not found: `sudo pacman -S webkit2gtk-4.1`
- If GTK4 conflicts: JUCE may still expect GTK3 — check JUCE release notes

### Audio thread assertions
- Never call `new`, `malloc`, `std::vector::push_back`, or any allocating function in `processBlock`
- Pre-allocate buffers in constructor or `prepareToPlay`
- Use lock-free queues (`juce::AbstractFifo`) for audio↔UI communication
