#pragma once
#include "PluginProcessor.h"

class CROWD editorDummy;
class CROWDAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit CROWDAudioProcessorEditor(CROWDAudioProcessor&);
    void paint(juce::Graphics&) override;
    void resized() override;
private:
    CROWDAudioProcessor& audioProcessor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CROWDAudioProcessorEditor)
};
