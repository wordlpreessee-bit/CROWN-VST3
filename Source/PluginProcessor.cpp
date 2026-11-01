#include "PluginProcessor.h"
#include "PluginEditor.h"

CROWDAudioProcessor::CROWDAudioProcessor()
 : AudioProcessor(BusesProperties().withInput("Input", juce::AudioChannelSet::stereo(), true)
                                  .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
   parameters(*this, nullptr, "PARAMETERS", makeLayout()) {}

juce::AudioProcessorValueTreeState::ParameterLayout CRO WDAudioProcessor_dummy();
juce::AudioProcessorValueTreeState::ParameterLayout CROWDAudioProcessor::makeLayout()
{
    using P = juce::AudioProcessorValueTreeState;
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> p;
    p.push_back(std::make_unique<juce::AudioParameterFloat>("voices","Voices",juce::NormalisableRange<float>(2,8,1),4));
    p.push_back(std::make_unique<juce::AudioParameterFloat>("spread","Spread",0.0f,100.0f,65.0f));
    p.push_back(std::make_unique<juce::AudioParameterFloat>("detune","Detune",0.0f,25.0f,8.0f));
    p.push_back(std::make_unique<juce::AudioParameterFloat>("humanize","Humanize",0.0f,100.0f,35.0f));
    p.push_back(std::make_unique<juce::AudioParameterFloat>("width","Width",0.0f,100.0f,80.0f));
    p.push_back(std::make_unique<juce::AudioParameterFloat>("delay","Delay",juce::NormalisableRange<float>(8.0f,35.0f,0.1f),18.0f));
    p.push_back(std::make_unique<juce::AudioParameterFloat>("mix","Mix",0.0f,100.0f,45.0f));
    return {p.begin(),p.end()};
}

void CROWDAudioProcessor::prepareToPlay(double sr, int)
{
    currentSampleRate = sr;
    delayBuffer.setSize(2, (int)(sr * 0.12) + 512);
    delayBuffer.clear();
    writePosition = 0;
}
void CROWDAudioProcessor::releaseResources() { delayBuffer.clear(); }

void CROWDAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    auto channels = getTotalNumOutputChannels();
    auto samples = buffer.getNumSamples();
    for (int c=getTotalNumInputChannels(); c<channels; ++c) buffer.clear(c,0,samples);
    const int maxDelay = delayBuffer.getNumSamples();
    const int voices = (int)parameters.getRawParameterValue("voices")->load();
    const float spread = parameters.getRawParameterValue("spread")->load()/100.0f;
    const float detune = parameters.getRawParameterValue("detune")->load()/100.0f;
    const float human = parameters.getRawParameterValue("humanize")->load()/100.0f;
    const float width = parameters.getRawParameterValue("width")->load()/100.0f;
    const float baseMs = parameters.getRawParameterValue("delay")->load();
    const float mix = parameters.getRawParameterValue("mix")->load()/100.0f;
    for (int n=0; n<samples; ++n)
    {
        float in[2] = { buffer.getSample(0,n), channels>1 ? buffer.getSample(1,n) : buffer.getSample(0,n) };
        float wet[2] = {0.0f,0.0f};
        for (int c=0;c<2;++c) delayBuffer.setSample(c,writePosition,in[c]);
        for (int v=0;v<voices;++v)
        {
            float pos = voices==1 ? 0.0f : (float)v/(float)(voices-1)*2.0f-1.0f;
            float phase = (float)(writePosition/currentSampleRate) * (0.17f + 0.03f*v) * juce::MathConstants<float>::twoPi;
            float wobble = std::sin(phase + v*1.71f) * (1.0f + human*3.0f);
            float ms = baseMs + pos*spread*0.12f + wobble*(0.3f+detune*2.0f);
            int d = juce::jlimit(1,maxDelay-1,(int)(ms*currentSampleRate/1000.0));
            int read = (writePosition-d+maxDelay)%maxDelay;
            float pan = pos*width*0.85f;
            float sampleL = delayBuffer.getSample(0,read);
            float sampleR = delayBuffer.getSample(1,read);
            wet[0] += (sampleL*(1.0f-pan)+sampleR*0.08f) / voices;
            wet[1] += (sampleR*(1.0f+pan)+sampleL*0.08f) / voices;
        }
        for (int c=0;c<2;++c) buffer.setSample(c,n,in[c]*(1.0f-mix)+wet[c]*mix);
        writePosition = (writePosition+1)%maxDelay;
    }
}
juce::AudioProcessorEditor* CROWDAudioProcessor::createEditor() { return new juce::GenericAudioProcessorEditor(*this); }
void CROWDAudioProcessor::getStateInformation(juce::MemoryBlock& dest) { auto s=parameters.copyState(); std::unique_ptr<juce::XmlElement> xml(s.createXml()); copyXmlToBinary(*xml,dest); }
void CROWDAudioProcessor::setStateInformation(const void* data,int size) { std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data,size)); if(xml && xml->hasTagName(parameters.state.getType())) parameters.replaceState(juce::ValueTree::fromXml(*xml)); }
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new CROWDAudioProcessor(); }
