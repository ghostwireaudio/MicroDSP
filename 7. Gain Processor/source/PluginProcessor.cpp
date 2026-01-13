#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
// Set up the input/output buses for the plugin.
GainProcessorAudioProcessor::GainProcessorAudioProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
{
}

// Clean up any resources (none needed here).
GainProcessorAudioProcessor::~GainProcessorAudioProcessor()
{
}

//==============================================================================
// Return the plugin name shown to the host.
const juce::String GainProcessorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

// Tell the host whether this plugin accepts MIDI input.
bool GainProcessorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

// Tell the host whether this plugin produces MIDI output.
bool GainProcessorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

// Tell the host whether this is a MIDI-only effect.
bool GainProcessorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

// No effect tail for this simple gain plugin.
double GainProcessorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

// We only report a single program/preset.
int GainProcessorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

// The only program is always index 0.
int GainProcessorAudioProcessor::getCurrentProgram()
{
    return 0;
}

// Change program (not used in this simple plugin).
void GainProcessorAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

// Return the program name (empty in this example).
const juce::String GainProcessorAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

// Rename a program (not used here).
void GainProcessorAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
// Prepare for playback (e.g., set up DSP objects).
void GainProcessorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::ignoreUnused (sampleRate, samplesPerBlock);
}

// Free any resources when playback stops.
void GainProcessorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

// Check whether the requested bus layout is supported.
bool GainProcessorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}

// Process audio and MIDI data for each block.
void GainProcessorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Apply gain to every sample.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        juce::ignoreUnused (channelData);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
            // Multiply by rawVolume (linear gain).
            channelData[sample] = buffer.getSample(channel, sample) * rawVolume;
        }
    }
}

//==============================================================================
// This plugin has a custom editor (UI).
bool GainProcessorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

// Create the editor (UI) instance.
juce::AudioProcessorEditor* GainProcessorAudioProcessor::createEditor()
{
    return new GainProcessorAudioProcessorEditor (*this);
}

//==============================================================================
// Save plugin state (not used in this example).
void GainProcessorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused (destData);
}

// Restore plugin state (not used in this example).
void GainProcessorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused (data, sizeInBytes);
}

//==============================================================================
// Factory function used by the host to create the plugin.
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GainProcessorAudioProcessor();
}