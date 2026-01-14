#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
// Build the UI and set up the gain slider.
GainProcessorAudioProcessorEditor::GainProcessorAudioProcessorEditor (GainProcessorAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (200, 400);
    // Configure the gain slider appearance and range.
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    gainSlider.setRange(-40.0, 6.0, 0.01);
    gainSlider.setValue(0.0);
    // Listen to slider changes and show it on screen.
    gainSlider.addListener(this);
    addAndMakeVisible(gainSlider);
}

// Clean up UI resources (none needed here).
GainProcessorAudioProcessorEditor::~GainProcessorAudioProcessorEditor()
{
}

//==============================================================================
// Paint the background.
void GainProcessorAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

}

// Layout the slider to fill the editor area.
void GainProcessorAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor.
    gainSlider.setBounds(getLocalBounds());
}

// When the slider changes, update the processor's gain.
void GainProcessorAudioProcessorEditor::sliderValueChanged(juce::Slider* slider) {
    if (slider == &gainSlider) {
        // Convert dB to linear gain and store it in the processor.
        processorRef.gain = juce::Decibels::decibelsToGain(static_cast<float>(gainSlider.getValue()));
    }
}