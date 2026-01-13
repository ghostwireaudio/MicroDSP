#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GainProcessorAudioProcessorEditor::GainProcessorAudioProcessorEditor (GainProcessorAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (200, 400);
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    gainSlider.setRange(-40.0, 0.0);
    gainSlider.setValue(-1.0);
    gainSlider.addListener(this);
    addAndMakeVisible(gainSlider);
}

GainProcessorAudioProcessorEditor::~GainProcessorAudioProcessorEditor()
{
}

//==============================================================================
void GainProcessorAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

}

void GainProcessorAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    gainSlider.setBounds(getLocalBounds());
}

void GainProcessorAudioProcessorEditor::sliderValueChanged(juce::Slider* slider) {
    if (slider == &gainSlider) {
        processorRef.rawVolume = pow(10, gainSlider.getValue() / 20);
    }
}
