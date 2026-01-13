#pragma once

#include "PluginProcessor.h"

//==============================================================================
class GainProcessorAudioProcessorEditor final : public juce::AudioProcessorEditor,
                                                public juce::Slider::Listener
{
public:
    explicit GainProcessorAudioProcessorEditor (GainProcessorAudioProcessor&);
    ~GainProcessorAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged (juce::Slider* slider) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GainProcessorAudioProcessor& processorRef;

    juce::Slider gainSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainProcessorAudioProcessorEditor)
};
