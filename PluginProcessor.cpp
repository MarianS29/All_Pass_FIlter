/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
APFAudioProcessor::APFAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    UserParams[freq]=1000.0f; 
    UserParams[banda] = 1000.0f;
    UIUpdateFlag=true; // se cere update de la interfața grafică
}

APFAudioProcessor::~APFAudioProcessor()
{
}

//==============================================================================
const juce::String APFAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool APFAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool APFAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool APFAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double APFAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int APFAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int APFAudioProcessor::getCurrentProgram()
{
    return 0;
}

void APFAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String APFAudioProcessor::getProgramName (int index)
{
    return {};
}

void APFAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void APFAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void APFAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool APFAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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
#endif

void APFAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
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

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
        double pi = 3.14;
        double fs = juce::AudioProcessor::getSampleRate();
        double c = (tan(pi*UserParams[banda]/fs) - 1) / (tan(pi*UserParams[banda]/fs) + 1);
        double d = -cos(2*pi*UserParams[freq]/fs);
        
        double x1 = 0, x2 = 0, x3 = 0;
        
        const int value = buffer.getNumSamples();
        //double xh[(const int)value];

        for (int j = 0; j < buffer.getNumSamples(); j++)
        {
            
            x3 = channelData[j] - d * (1 - c) * x2 + c * x1;
            channelData[j] = -c*x3 + d*(1-c)*x2 + x1;
            x2 = x3;
            x1 = x2;
            //channelData[j] = g*channelData[j];
        }
    }
}

//==============================================================================
bool APFAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* APFAudioProcessor::createEditor()
{
    return new APFAudioProcessorEditor (this);
}

//==============================================================================
void APFAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    XmlElement root("Root");
    XmlElement *el;
    
    el = root.createNewChildElement("Freq");
    el->addTextElement(String(UserParams[freq]));
    
    el = root.createNewChildElement("banda");
    el->addTextElement(String(UserParams[banda]));
    
    copyXmlToBinary(root,destData);
}

void APFAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    static std::unique_ptr< XmlElement > pRoot = getXmlFromBinary(data, sizeInBytes);
    if(pRoot!=NULL)
    {
        forEachXmlChildElement((*pRoot),pChild)
        {
            if(pChild->hasTagName("Freq"))
            {
                String text = pChild->getAllSubText();
                setParameter(freq,text.getFloatValue());
            }
            else if(pChild->hasTagName("banda"))
            {
                String text = pChild->getAllSubText();
                setParameter(banda,text.getFloatValue());
            }
        }
        UIUpdateFlag=true; // cere update de la interfata grafica
    }

}

int APFAudioProcessor::getNumParameters()
{   
    return totalNumParam;
}

float APFAudioProcessor::getParameter (int index)
{
    switch(index)
        {
            case freq:
                return UserParams[freq];
            case banda:
                return UserParams[banda];
            default: return 10.0f; // index invalid
        }
}

void APFAudioProcessor::setParameter (int index, float newValue)
{
    switch(index)
        {
            case freq:
                UserParams[freq]=newValue;
            case banda:
                UserParams[banda]=newValue;
                break;
            default: return;
        }
        UIUpdateFlag=true;//Request UI update -- Some OSX hosts use alternate editors, this updates ours
}


const String APFAudioProcessor::getParameterName (int index)
{
 switch(index)
    {
        case freq: return "Freq";
            break;
        case banda: return "banda";
            break;
        default:return String();
    }
}

const String APFAudioProcessor::getParameterText (int index)
    {
        if (index>=0 && index<totalNumParam)
            return String(UserParams[index]);
        else return String();
    }

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new APFAudioProcessor();
}
