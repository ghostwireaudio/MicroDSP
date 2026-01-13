/*
  ==============================================================================

   This file is part of the JUCE framework.
   Copyright (c) Raw Material Software Limited

   JUCE is an open source framework subject to commercial or open source
   licensing.

   By downloading, installing, or using the JUCE framework, or combining the
   JUCE framework with any other source code, object code, content or any other
   copyrightable work, you agree to the terms of the JUCE End User Licence
   Agreement, and all incorporated terms including the JUCE Privacy Policy and
   the JUCE Website Terms of Service, as applicable, which will bind you. If you
   do not agree to the terms of these agreements, we will not license the JUCE
   framework to you, and you must discontinue the installation or download
   process and cease use of the JUCE framework.

   JUCE End User Licence Agreement: https://juce.com/legal/juce-8-licence/
   JUCE Privacy Policy: https://juce.com/juce-privacy-policy
   JUCE Website Terms of Service: https://juce.com/juce-website-terms-of-service/

   Or:

   You may also use this code under the terms of the AGPLv3:
   https://www.gnu.org/licenses/agpl-3.0.en.html

   THE JUCE FRAMEWORK IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL
   WARRANTIES, WHETHER EXPRESSED OR IMPLIED, INCLUDING WARRANTY OF
   MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, ARE DISCLAIMED.

  ==============================================================================
*/

namespace juce
{

GainProcessorFormat::GainProcessorFormat() {}
GainProcessorFormat::~GainProcessorFormat() {}

std::unique_ptr<GainProcessorInstance> GainProcessorFormat::createInstanceFromDescription (const PluginDescription& desc,
                                                                                       double initialSampleRate,
                                                                                       int initialBufferSize)
{
    String errorMessage;
    return createInstanceFromDescription (desc, initialSampleRate, initialBufferSize, errorMessage);
}

std::unique_ptr<GainProcessorInstance> GainProcessorFormat::createInstanceFromDescription (const PluginDescription& desc,
                                                                                       double initialSampleRate,
                                                                                       int initialBufferSize,
                                                                                       String& errorMessage)
{
    if (MessageManager::getInstance()->isThisTheMessageThread()
          && requiresUnblockedMessageThreadDuringCreation (desc))
    {
        errorMessage = NEEDS_TRANS ("This plug-in cannot be instantiated synchronously");
        return {};
    }

    WaitableEvent finishedSignal;
    std::unique_ptr<GainProcessorInstance> instance;

    auto callback = [&] (std::unique_ptr<GainProcessorInstance> p, const String& error)
    {
       errorMessage = error;
       instance = std::move (p);
       finishedSignal.signal();
    };

    if (! MessageManager::getInstance()->isThisTheMessageThread())
        createPluginInstanceAsync (desc, initialSampleRate, initialBufferSize, std::move (callback));
    else
        createPluginInstance (desc, initialSampleRate, initialBufferSize, std::move (callback));

    finishedSignal.wait();
    return instance;
}

struct GainProcessorFormat::AsyncCreateMessage final : public Message
{
    AsyncCreateMessage (const PluginDescription& d, double sr, int size, PluginCreationCallback call)
        : desc (d), sampleRate (sr), bufferSize (size), callbackToUse (std::move (call))
    {
    }

    PluginDescription desc;
    double sampleRate;
    int bufferSize;
    PluginCreationCallback callbackToUse;
};

void GainProcessorFormat::createPluginInstanceAsync (const PluginDescription& description,
                                                   double initialSampleRate, int initialBufferSize,
                                                   PluginCreationCallback callback)
{
    jassert (callback != nullptr);
    postMessage (new AsyncCreateMessage (description, initialSampleRate, initialBufferSize, std::move (callback)));
}

void GainProcessorFormat::handleMessage (const Message& message)
{
    if (auto m = dynamic_cast<const AsyncCreateMessage*> (&message))
        createPluginInstance (m->desc, m->sampleRate, m->bufferSize, std::move (m->callbackToUse));
}

} // namespace juce
