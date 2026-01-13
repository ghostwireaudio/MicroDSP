#pragma once

#include "PluginProcessor.h"

//==============================================================================
// Main editor (UI) class for the plugin.
class GainProcessorAudioProcessorEditor final : public juce::AudioProcessorEditor,
                                                public juce::Slider::Listener
{
public:
    // Create the editor and connect it to the processor.
    explicit GainProcessorAudioProcessorEditor (GainProcessorAudioProcessor&);
    ~GainProcessorAudioProcessorEditor() override;

    //==============================================================================
    // Draw the UI.
    void paint (juce::Graphics&) override;
    // Position UI components.
    void resized() override;
    // React to slider value changes.
    void sliderValueChanged (juce::Slider* slider) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GainProcessorAudioProcessor& processorRef;

    // Slider that controls the gain.
    juce::Slider gainSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainProcessorAudioProcessorEditor)
};