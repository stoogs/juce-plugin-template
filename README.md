# JUCE Plugin Template

A base template for new JUCE plugins with CLAP support via clap-juce-extensions.

---

## Starting a New Plugin

### 1. Clone with all submodules

```bash
git clone --recursive https://github.com/stoogs/juce-plugin-template.git MyNewPlugin
cd MyNewPlugin
```

### 2. Wipe the template's git history

```bash
rm -rf .git
git init
```

### 3. Re-register submodules

> The submodule folders exist on disk but aren't valid git repos after wiping `.git`.
> Remove them first, then re-add — git won't re-download everything, it uses local cache.

```bash
rm -rf libs/JUCE libs/clap-juce-extensions
git submodule add https://github.com/juce-framework/JUCE.git libs/JUCE
git submodule add https://github.com/free-audio/clap-juce-extensions.git libs/clap-juce-extensions
git submodule update --init --recursive
```

### 4. Initial commit

```bash
git add .
git commit -m "init: new plugin from template"
```

### 5. Push to a new remote

```bash
git remote add origin https://github.com/stoogs/MyNewPlugin.git
git push -u origin master
```

---

## Test the Build

```bash
# Wipe any stale cache first (important if build/ came from the template)
rm -rf build

# Configure
cmake -B build -DCMAKE_BUILD_TYPE=Debug

# Build
cmake --build build --config Debug -- -j$(nproc)
```

Built artefacts will appear in `build/MyNewPlugin_artefacts/Debug/`.

---

## .gitignore

Make sure your `.gitignore` includes the following — especially `build/`, which contains
large compiled binaries that will break GitHub pushes:

```gitignore
# Build
build/
cmake-build-*/

# IDE
.vscode/
.idea/
*.swp

# OS
.DS_Store
Thumbs.db

# JUCE
JuceLibraryCode/
*.jucer.bak

# Compiled outputs
*.o
*.a
*.so
*.dylib
*.dll
*.exe
```

---

## Troubleshooting

**`already exists and is not a valid git repo`**
→ Submodule folders exist on disk but aren't registered. Remove and re-add:
```bash
rm -rf libs/JUCE libs/clap-juce-extensions
git submodule add https://github.com/juce-framework/JUCE.git libs/JUCE
git submodule add https://github.com/free-audio/clap-juce-extensions.git libs/clap-juce-extensions
git submodule update --init --recursive
```

**`add_subdirectory given source "libs/clap-juce-extensions" which is not an existing directory`**
→ Submodule is missing entirely. Run:
```bash
git submodule add https://github.com/free-audio/clap-juce-extensions.git libs/clap-juce-extensions
git submodule update --init --recursive
```

**`CMakeCache.txt` path mismatch error**
→ Stale build cache from the template. Run `rm -rf build` then re-run cmake.

**Push rejected with large file errors**
→ `build/` was committed. Make sure `build/` is in `.gitignore`, then wipe and restart git history:
```bash
rm -rf .git
git init
rm -rf libs/JUCE libs/clap-juce-extensions
git submodule add https://github.com/juce-framework/JUCE.git libs/JUCE
git submodule add https://github.com/free-audio/clap-juce-extensions.git libs/clap-juce-extensions
git submodule update --init --recursive
git add .
git commit -m "init: clean start"
git remote add origin https://github.com/stoogs/MyNewPlugin.git
git push origin master --force
```
