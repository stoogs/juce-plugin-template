cmake_minimum_required(VERSION 3.22)
project(PluginTemplate VERSION 1.0.0)

add_subdirectory(libs/JUCE)
add_subdirectory(libs/clap-juce-extensions)

juce_add_binary_data(PluginTemplateData
    SOURCES
        web/index.html
)

juce_add_plugin(PluginTemplate
    COMPANY_NAME "YourCompany"
    PLUGIN_MANUFACTURER_CODE Mfrc
    PLUGIN_CODE Plug
    FORMATS CLAP VST3 Standalone
    IS_SYNTH FALSE
    IS_MIDI_EFFECT FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    NEEDS_WEB_BROWSER TRUE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
    VST3_CATEGORIES "Fx"
    PLUGIN_NAME "Plugin Template"
)

target_sources(PluginTemplate PRIVATE
    src/PluginProcessor.cpp
    src/PluginProcessor.h
    src/PluginEditor.cpp
    src/PluginEditor.h
)

target_compile_definitions(PluginTemplate
    PUBLIC
    JUCE_WEB_BROWSER=1
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(PluginTemplate
    PRIVATE
    PluginTemplateData
    juce::juce_audio_utils
    juce::juce_audio_processors
    juce::juce_core
    juce::juce_data_structures
    juce::juce_events
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra
    PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)

clap_juce_extensions_plugin(
    TARGET PluginTemplate
    CLAP_ID "com.yourcompany.plugintemplate"
    CLAP_FEATURES audio-effect
    CLAP_MANUAL_URL "https://yourcompany.com"
    CLAP_SUPPORT_URL "https://yourcompany.com"
)
