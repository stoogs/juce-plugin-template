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

> Required because wiping `.git` also destroys `.gitmodules`.

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

## Troubleshooting

**`add_subdirectory given source "libs/clap-juce-extensions" which is not an existing directory`**
→ Submodule is missing. Run:
```bash
git submodule add https://github.com/free-audio/clap-juce-extensions.git libs/clap-juce-extensions
git submodule update --init --recursive
```

**`CMakeCache.txt` path mismatch error**
→ Stale build cache from the template. Run `rm -rf build` then re-run cmake.

**`libs/JUCE` is empty after cloning**
→ You cloned without `--recursive`. Run `git submodule update --init --recursive`.
