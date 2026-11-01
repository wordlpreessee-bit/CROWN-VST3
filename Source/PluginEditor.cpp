#include "PluginEditor.h"

CROWDAudioProcessorEditor::CROWDAudioProcessorEditor(CROWDAudioProcessor& p)
 : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(520,360);
}
void CROWDAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff111116));
    g.setColour(juce::Colour(0xfff04b55));
    g.setFont(juce::Font(juce::FontOptions(32.0f,juce::Font::bold)));
    g.drawText("CROWD",20,18,480,48,juce::Justification::centred);
    g.setColour(juce::Colours::white.withAlpha(0.7f));
    g.setFont(juce::Font(14.0f));
    g.drawText("VOCAL DOUBLER  /  MULTI-VOICE EFFECT",20,65,480,24,juce::Justification::centred);
    g.setColour(juce::Colours::white.withAlpha(0.45f));
    g.drawText("Use the controls below to widen and thicken a vocal.",20,100,480,24,juce::Justification::centred);
}
void CROWDAudioProcessorEditor::resized() {}
