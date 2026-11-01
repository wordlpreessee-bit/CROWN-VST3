#pragma once
#include <JuceHeader.h>

class CROWDAudioProcessor : public juce::AudioProcessor
{
public:
    CROWDAudioProcessor();
    ~CROWDAudioProcessor() override = default;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }
    const juce::String getName() const override { return "CROWD"; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 2.0; }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}
    void getStateInformation(juce::MemoryBlock&) override;
    void setStateInformation(const void*, int) override;

    juce::AudioProcessorValueTreeState parameters;
private:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();
    juce::AudioBuffer<float> delayBuffer;
    int writePosition = 0;
    double currentSampleRate = 44100.0;
    juce::AudioProcessorValueTreeState::ParameterLayout makeLayout();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CROWDAudioProcessor)
};
