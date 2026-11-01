cmake_minimum_required(VERSION 3.22)
project(CROWD VERSION 1.0.0)

include(FetchContent)
FetchContent_Declare(juce
  GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
  GIT_TAG 8.0.6
  GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(juce)

juce_add_plugin(CROWD
  COMPANY_NAME "CROWD Audio"
  BUNDLE_ID "com.crowdaudio.crowd"
  PLUGIN_MANUFACTURER_CODE Crwd
  PLUGIN_CODE Cr01
  FORMATS VST3
  PRODUCT_NAME "CROWD"
  IS_SYNTH FALSE
  NEEDS_MIDI_INPUT FALSE
  NEEDS_MIDI_OUTPUT FALSE
  IS_MIDI_EFFECT FALSE
  EDITOR_WANTS_KEYBOARD_FOCUS FALSE
  COPY_PLUGIN_AFTER_BUILD FALSE)

target_sources(CROWD PRIVATE
  Source/PluginProcessor.cpp
  Source/PluginProcessor.h
  Source/PluginEditor.cpp
  Source/PluginEditor.h)

target_compile_features(CROWD PRIVATE cxx_std_17)
target_link_libraries(CROWD PRIVATE
  juce::juce_audio_utils
  juce::juce_dsp
  PUBLIC
  juce::juce_recommended_config_flags
  juce::juce_recommended_lto_flags
  juce::juce_recommended_warning_flags)
target_compile_definitions(CROWD PUBLIC
  JUCE_WEB_BROWSER=0
  JUCE_USE_CURL=0
  JUCE_VST3_CAN_REPLACE_VST2=0)
