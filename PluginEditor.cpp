/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 7.0.3

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "PluginEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
APFAudioProcessorEditor::APFAudioProcessorEditor (APFAudioProcessor* ownerFilter)
    : AudioProcessorEditor(ownerFilter)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    freqSlider.reset (new juce::Slider ("Slider frecventq"));
    addAndMakeVisible (freqSlider.get());
    freqSlider->setRange (500, 10000, 10);
    freqSlider->setSliderStyle (juce::Slider::LinearHorizontal);
    freqSlider->setTextBoxStyle (juce::Slider::TextBoxRight, false, 70, 20);
    freqSlider->addListener (this);

    freqSlider->setBounds (104, 16, 184, 32);

    FreqLabel.reset (new juce::Label ("Lavel frecventa",
                                      TRANS("Frecventa de taiere [Hz]\n")));
    addAndMakeVisible (FreqLabel.get());
    FreqLabel->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    FreqLabel->setJustificationType (juce::Justification::centredLeft);
    FreqLabel->setEditable (false, false, false);
    FreqLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    FreqLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    FreqLabel->setBounds (8, 16, 104, 30);

    band_slider.reset (new juce::Slider ("Banda"));
    addAndMakeVisible (band_slider.get());
    band_slider->setRange (500, 10000, 10);
    band_slider->setSliderStyle (juce::Slider::LinearHorizontal);
    band_slider->setTextBoxStyle (juce::Slider::TextBoxRight, false, 60, 20);
    band_slider->addListener (this);

    band_slider->setBounds (112, 160, 176, 24);

    band__label.reset (new juce::Label ("Label Banda",
                                        TRANS("Banda [Hz]")));
    addAndMakeVisible (band__label.get());
    band__label->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    band__label->setJustificationType (juce::Justification::centredLeft);
    band__label->setEditable (false, false, false);
    band__label->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    band__label->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    band__label->setBounds (16, 160, 88, 24);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (300, 200);


    //[Constructor] You can add your own custom stuff here..
    getProcessor()->RequestUIUpdate();
    startTimer(200);
    //[/Constructor]
}

APFAudioProcessorEditor::~APFAudioProcessorEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    freqSlider = nullptr;
    FreqLabel = nullptr;
    band_slider = nullptr;
    band__label = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void APFAudioProcessorEditor::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff394590));

    {
        int x = 14, y = 65, width = 272, height = 71;
        juce::Colour fillColour = juce::Colour (0xffa5322a);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void APFAudioProcessorEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void APFAudioProcessorEditor::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    APFAudioProcessor* ourProcessor = getProcessor();
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == freqSlider.get())
    {
        //[UserSliderCode_freqSlider] -- add your slider handling code here..
        ourProcessor->setParameter(APFAudioProcessor::freq, freqSlider->getValue());
        //[/UserSliderCode_freqSlider]
    }
    else if (sliderThatWasMoved == band_slider.get())
    {
        //[UserSliderCode_band_slider] -- add your slider handling code here..
        ourProcessor->setParameter(APFAudioProcessor::banda, band_slider->getValue());
        //[/UserSliderCode_band_slider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void APFAudioProcessorEditor::timerCallback()
{
    APFAudioProcessor* ourProcessor = getProcessor();
    //se schimba datele dorite intre elementele interfetei grafice si modulul “ourProcessor”
    if (ourProcessor->NeedsUIUpdate())
    {
        freqSlider->setValue(ourProcessor->getParameter(APFAudioProcessor::freq), juce::dontSendNotification);
        ourProcessor->ClearUIUpdateFlag();
    }
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="APFAudioProcessorEditor"
                 componentName="" parentClasses="public AudioProcessorEditor, public Timer"
                 constructorParams="APFAudioProcessor* ownerFilter" variableInitialisers="AudioProcessorEditor(ownerFilter)&#10;"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="300" initialHeight="200">
  <BACKGROUND backgroundColour="ff394590">
    <RECT pos="14 65 272 71" fill="solid: ffa5322a" hasStroke="0"/>
  </BACKGROUND>
  <SLIDER name="Slider frecventq" id="b081820688ea24a6" memberName="freqSlider"
          virtualName="" explicitFocusOrder="0" pos="104 16 184 32" min="500.0"
          max="10000.0" int="10.0" style="LinearHorizontal" textBoxPos="TextBoxRight"
          textBoxEditable="1" textBoxWidth="70" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="Lavel frecventa" id="4e00c06eaf3918e3" memberName="FreqLabel"
         virtualName="" explicitFocusOrder="0" pos="8 16 104 30" edTextCol="ff000000"
         edBkgCol="0" labelText="Frecventa de taiere [Hz]&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="Banda" id="ac78e2a07a20e21" memberName="band_slider" virtualName=""
          explicitFocusOrder="0" pos="112 160 176 24" min="500.0" max="10000.0"
          int="10.0" style="LinearHorizontal" textBoxPos="TextBoxRight"
          textBoxEditable="1" textBoxWidth="60" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="Label Banda" id="3562f9915db6b362" memberName="band__label"
         virtualName="" explicitFocusOrder="0" pos="16 160 88 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Banda [Hz]" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

