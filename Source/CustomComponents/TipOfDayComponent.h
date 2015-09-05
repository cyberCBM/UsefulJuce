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

#ifndef __JUCE_HEADER_14FA11D3F26DF9C8__
#define __JUCE_HEADER_14FA11D3F26DF9C8__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include <vector>
//[/Headers]

//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class TipOfDayComponent  : public Component,
                      public ButtonListener
{
public:
    //==============================================================================
    TipOfDayComponent ();
    ~TipOfDayComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	struct Tips
	{
		String	description;
		String	featureName;
		Image	image;
		String	htmlLink;
	};
	void loadTipDataFromFile(const File & file);  // loading all tip into vector
    void displayTipData(int tipNumber);// display data in window
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    int currentTipNumber;
    std::vector<Tips> tip_arr;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<TextButton> nextButton;
    ScopedPointer<TextButton> previousButton;
    ScopedPointer<TextEditor> tipDescriptionTextEditor;
    ScopedPointer<Label> featureNameLabel;
	ScopedPointer<HyperlinkButton> linkButton;
    ScopedPointer<ToggleButton> showTipToggleButton;
	ScopedPointer<ImageComponent> imageComponent;
	ToggleButtonLookAndFeel tbnlnf;
	TextButtonLookAndFeel tblnf;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TipOfDayComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_14FA11D3F26DF9C8__
