#pragma once // Only include this header once.

#include <juce_audio_processors/juce_audio_processors.h>

//==============================================================================
// Main audio processor class (the "engine" of the plugin).
class GainProcessorAudioProcessor final : public juce::AudioProcessor
{
public:
    //==============================================================================
    // Constructor and destructor.
    GainProcessorAudioProcessor();
    ~GainProcessorAudioProcessor() override;

    //==============================================================================
    // Called before audio playback starts.
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    // Called when playback stops; use it to free resources.
    void releaseResources() override;

    // Checks if the host's channel layout is supported (mono/stereo).
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    // Main audio callback: process the audio buffer.
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    // Create and report the plugin's editor (UI).
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    // Plugin name shown to the host.
    const juce::String getName() const override;

    // MIDI capability flags.
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    // Tail length for effects like reverb (not used here).
    double getTailLengthSeconds() const override;

    //==============================================================================
    // Preset/program handling (simple single-program setup).
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    // Save and restore plugin state (not used in this example).
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // The current gain value (linear scale, not dB).
    float gain;

private:
    //==============================================================================
    // Prevent copying and add leak detection in debug builds.
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainProcessorAudioProcessor)
};