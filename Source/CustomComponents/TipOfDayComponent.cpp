/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.2.0

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
  */

//[Headers] You can add your own extra header files here...
#include "stdafx.h"
//[/Headers]

#include "TipOfDayComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
#include <base/CoreContext.h>
#include "AuViD/Utility/SettingsConstants.h"
//[/MiscUserDefs]

//==============================================================================
TipOfDayComponent::TipOfDayComponent() : currentTipNumber(0)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible(nextButton = new TextButton("Next"));
    nextButton->addListener(this);
	nextButton->setLookAndFeel(&tblnf);
	nextButton->setColour(TextButton::buttonColourId, Colour(0xFF2C91D8));
	nextButton->setColour(TextButton::buttonOnColourId, Colour(0xFF2C91D8));
	nextButton->setColour(TextButton::textColourOnId, Colour(0xFFFFFFFF));
	nextButton->setColour(TextButton::textColourOffId, Colour(0xFFFFFFFF));

    addAndMakeVisible(previousButton = new TextButton("Previous"));
    previousButton->addListener(this);
	previousButton->setLookAndFeel(&tblnf);
	previousButton->setColour(TextButton::buttonColourId, Colour(0xFF2C91D8));
	previousButton->setColour(TextButton::buttonOnColourId, Colour(0xFF2C91D8));
	previousButton->setColour(TextButton::textColourOnId, Colour(0xFFFFFFFF));
	previousButton->setColour(TextButton::textColourOffId, Colour(0xFFFFFFFF));

    addAndMakeVisible(tipDescriptionTextEditor = new TextEditor("new text editor"));
    tipDescriptionTextEditor->setMultiLine(true);
    tipDescriptionTextEditor->setReturnKeyStartsNewLine(true);
    tipDescriptionTextEditor->setReadOnly(true);
    tipDescriptionTextEditor->setScrollbarsShown(true);
    tipDescriptionTextEditor->setCaretVisible(true);
    tipDescriptionTextEditor->setPopupMenuEnabled(false);
    tipDescriptionTextEditor->setColour(TextEditor::textColourId, Colour(0xfffffdfd));
    tipDescriptionTextEditor->setColour(TextEditor::backgroundColourId, Colour(0xff364760));
    tipDescriptionTextEditor->setColour(CaretComponent::caretColourId, Colours::white);

    addAndMakeVisible(featureNameLabel = new Label("new label",
        TRANS("AuviD:\n")));
    featureNameLabel->setFont(Font("Ubuntu-B", 19.00f, Font::bold));
    featureNameLabel->setJustificationType(Justification::centredLeft);
    featureNameLabel->setEditable(false, false, false);
    featureNameLabel->setColour(Label::textColourId, Colours::white);
    featureNameLabel->setColour(Label::backgroundColourId, Colour(0x00000000));

    addAndMakeVisible(linkButton = new HyperlinkButton(TRANS("Know More..."),
        URL("http://sp-wiki:8080/display/ATP/AuViD+v9+User+Guide")));
    linkButton->setTooltip(TRANS("Go to detailed user guide for "));
    linkButton->setFont(Font(14.0f), true, Justification::left);
    linkButton->setColour(HyperlinkButton::textColourId, Colours::white);    
    
    addAndMakeVisible(showTipToggleButton = new ToggleButton());
    showTipToggleButton->addListener(this);
    showTipToggleButton->setButtonText("Never show on startup (Access from Help menu)");
    bool toggleState;
    CONFIG_MANAGER()->get(SettingsConstants::AU_ShowTipBool, toggleState);
    showTipToggleButton->setToggleState(toggleState, false);
    showTipToggleButton->setColour(ToggleButton::textColourId, Colours::white);
	showTipToggleButton->setLookAndFeel(&tbnlnf);

    addAndMakeVisible(imageComponent = new ImageComponent("tip Image"));
    imageComponent->setEnabled(true);
    imageComponent->setAlwaysOnTop(true);

    //[UserPreSize]
    CONFIG_MANAGER()->get(SettingsConstants::Au_LastTipNumber, currentTipNumber);
    String dataFilePath = RESOURCEPATH + File::separatorString + "TipsData.xml";
    File tipDataFile(dataFilePath);
    loadTipDataFromFile(tipDataFile);
    //[/UserPreSize]
    setSize(490, 350);
    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

TipOfDayComponent::~TipOfDayComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]
    currentTipNumber = currentTipNumber + 1;
    if (currentTipNumber >= tip_arr.size())
        currentTipNumber = 0;
    CONFIG_MANAGER()->set(SettingsConstants::Au_LastTipNumber, currentTipNumber);
    tip_arr.clear();
    nextButton = nullptr;
    previousButton = nullptr;
    tipDescriptionTextEditor = nullptr;
    featureNameLabel = nullptr;
    linkButton = nullptr;
    showTipToggleButton = nullptr;
    imageComponent = nullptr;

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void TipOfDayComponent::paint(Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll(Colours::white);
	g.setColour(Colour(0xFF263446));
    g.fillRect(-4, -4, 505, 355);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}
void TipOfDayComponent::loadTipDataFromFile(const File & file)
{
    if (file.exists())
    {
        String enable;
        XmlDocument document(file);
        ScopedPointer<XmlElement> mainXmlElement;
        mainXmlElement = document.getDocumentElement();
        if (mainXmlElement != 0L)
        {
            forEachXmlChildElement(*mainXmlElement, child)
            {
                if (child->hasTagName("Data"))
                {
                    forEachXmlChildElement(*child, infoChild)
                    {
                        // process and store them in some data structure to use later
                        if (infoChild->hasTagName("Information"))
                        {
                            Tips tip;
                            tip.description = infoChild->getStringAttribute("info", String::empty);
                            String imagePath = ICONRESOURCEPATH + "TipImages" + File::separatorString + 
                                infoChild->getStringAttribute("image", String::empty);
                            tip.image = ImageCache::getFromFile(imagePath);
                            tip.featureName = infoChild->getStringAttribute("label", String::empty);
                            tip.htmlLink = infoChild->getStringAttribute("link", String::empty);
                            tip_arr.push_back(tip);
                        }
                    }
                }
            }
        }
        if (tip_arr.empty())
        {
            // close window only // TO DO
        }
        displayTipData(currentTipNumber);
    }
}

void TipOfDayComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]
    previousButton->setBounds(330, 320, 70, 24);
    nextButton->setBounds(402, 320, 70, 24);
    tipDescriptionTextEditor->setBounds(21, 195, 450, 115);
    featureNameLabel->setBounds(16, 7, 317, 18);
    linkButton->setBounds(385, 8, 87, 20);
    showTipToggleButton->setBounds(16, 320, 300, 24);
    imageComponent->setBounds(21, 35, 450, 150);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void TipOfDayComponent::displayTipData(int tipNumber)
{
    if (!tip_arr.empty() && tipNumber < tip_arr.size())
    {
        imageComponent->setImage(tip_arr[tipNumber].image, RectanglePlacement::doNotResize);
        featureNameLabel->setText(tip_arr[tipNumber].featureName, dontSendNotification);
        tipDescriptionTextEditor->setText(tip_arr[tipNumber].description);
        linkButton->setURL(tip_arr[tipNumber].htmlLink);
    }
}

void TipOfDayComponent::buttonClicked(Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == nextButton)
    {
        //[UserButtonCode_textButton] -- add your button handler code here..
        currentTipNumber++;
        if (currentTipNumber > tip_arr.size() - 1)
        {
            currentTipNumber = 0;
            displayTipData(currentTipNumber);
        }
        else
            displayTipData(currentTipNumber);
        //[/UserButtonCode_textButton]
    }
    else if (buttonThatWasClicked == previousButton)
    {
        //[UserButtonCode_textButton2] -- add your button handler code here..
        currentTipNumber--;
        if (currentTipNumber < 0)
        {
            currentTipNumber = tip_arr.size() - 1;
            displayTipData(currentTipNumber);
        }
        else
            displayTipData(currentTipNumber);
        //[/UserButtonCode_textButton2]
    }

    else if (buttonThatWasClicked == showTipToggleButton)
    {
        //[UserButtonCode_toggleButton] -- add your button handler code here..
        CONFIG_MANAGER()->set(SettingsConstants::AU_ShowTipBool, showTipToggleButton->getToggleState());
        //[/UserButtonCode_toggleButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

    BEGIN_JUCER_METADATA

    <JUCER_COMPONENT documentType="Component" className="TipComponent" componentName=""
    parentClasses="public Component" constructorParams="" variableInitialisers=""
    snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.325"
    fixedSize="0" initialWidth="550" initialHeight="350">
    <BACKGROUND backgroundColour="ffffffff">
    <RECT pos="-4 -4 500 350" fill="solid: ff2a394f" hasStroke="0"/>
    </BACKGROUND>
    <TEXTBUTTON name="new button" id="19aa69ffae962ef5" memberName="textButton"
    virtualName="" explicitFocusOrder="0" pos="381 309 88 24" buttonText="new button"
    connectedEdges="0" needsCallback="1" radioGroupId="0"/>
    <TEXTBUTTON name="new button" id="a564a67bab32f90b" memberName="textButton2"
    virtualName="" explicitFocusOrder="0" pos="282 309 90 24" buttonText="new button"
    connectedEdges="0" needsCallback="1" radioGroupId="0"/>
    <tipinformation name="new text editor" id="29bdc00fb848fe14" memberName="tipinformation"
    virtualName="" explicitFocusOrder="0" pos="18 201 454 96" textcol="fffffdfd"
    bkgcol="ff364760" caretcol="ffffffff" initialText="hghgdfhgfh"
    multiline="1" retKeyStartsLine="1" readonly="1" scrollbars="1"
    caret="0" popupmenu="0"/>
    <LABEL name="new label" id="4834c19380325cb8" memberName="tiplabel" virtualName=""
    explicitFocusOrder="0" pos="3 7 317 24" textCol="ffcbc8c8" edTextCol="ff000000"
    edBkgCol="0" labelText="AuviD:&#10;" editableSingleClick="0"
    editableDoubleClick="0" focusDiscardsChanges="0" fontname="Copperplate Gothic"
    fontsize="35" bold="1" italic="0" justification="33"/>
    <linkButton name="new hyperlink" id="4fb0f4aa2a1aaf35" memberName="linkButton"
    virtualName="" explicitFocusOrder="0" pos="321 7 150 24" tooltip="http://www.juce.com"
    buttonText="new hyperlink" connectedEdges="0" needsCallback="0"
    radioGroupId="0" url="http://www.juce.com"/>
    <TOGGLEBUTTON name="new toggle button" id="6d5962f11d7397e5" memberName="toggleButton"
    virtualName="" explicitFocusOrder="0" pos="13 308 147 24" buttonText="new toggle button"
    connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
    <GENERICCOMPONENT name="new component" id="891512f1d495b0b3" memberName="component"
    virtualName="" explicitFocusOrder="0" pos="21 43 450 149" class="Component"
    params=""/>
    </JUCER_COMPONENT>

    END_JUCER_METADATA
    */
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
